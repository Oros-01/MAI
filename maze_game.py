# py -3.12 maze_game.py
# -*- coding: utf-8 -*-
"""
Игра "Лабиринт" — симуляция навигации мобильного робота в неизвестной среде.

Идея:
    Лабиринт существует целиком и сразу (сгенерирован алгоритмом поиска
    в глубину), но игроку он не виден. Робот "видит" стены только в
    радиусе R вокруг себя (как дальность сенсора / лидара). Всё, что
    попало в радиус обзора хотя бы раз, запоминается и остаётся видимым
    навсегда (аналог построения карты в SLAM).

    Задача — доехать из точки A (старт) в точку B (финиш) до истечения
    времени. Столкновение со стеной не убивает робота, а просто не
    даёт сквозь неё проехать (как и должно быть у реального робота).

Управление:
    Стрелки            — движение робота
    Левая кнопка мыши  — кнопки интерфейса
    Esc                — выход из игры

Запуск:
    python maze_game.py
"""

import sys
import os
import json
import random
import math
import pygame

# ---------------------------------------------------------------------------
# НАСТРОЙКИ
# ---------------------------------------------------------------------------

# Пресеты сложности: (столбцов, строк)
DIFFICULTIES = {
    "easy":   {"label": "Легко",  "size": (14, 12)},
    "medium": {"label": "Средне", "size": (22, 16)},
    "hard":   {"label": "Сложно", "size": (24, 24)},
}
DIFFICULTY_ORDER = ["easy", "medium", "hard"]

TIME_OPTIONS = [30, 60, 90]  # секунды, доступные варианты лимита времени

# Таблица рекордов сохраняется рядом со скриптом и переживает перезапуск игры
LEADERBOARD_FILE = os.path.join(os.path.dirname(os.path.abspath(__file__)), "leaderboard.json")
LEADERBOARD_MAX = 10

# Область, в которую всегда вписывается лабиринт независимо от сложности
# (при более крупном лабиринте клетки становятся мельче, но окно не меняется)
MAZE_AREA_W = 700
MAZE_AREA_H = 520

WALL_THICKNESS = 5
VISION_RADIUS_CELLS = 1.0    # радиус обзора робота R (в клетках)

ROBOT_MOVE_TIME = 0.10       # сколько секунд робот едет от клетки до клетки

SIDEBAR_WIDTH = 300
MARGIN = 24

WINDOW_W = MAZE_AREA_W + SIDEBAR_WIDTH + MARGIN * 3
WINDOW_H = MAZE_AREA_H + MARGIN * 2

FPS = 60

# Цвета
COLOR_BG = (18, 20, 26)
COLOR_PANEL = (28, 31, 40)
COLOR_FOG = (10, 11, 15)
COLOR_FLOOR_SEEN = (46, 50, 61)
COLOR_FLOOR_VISIBLE = (63, 70, 87)
COLOR_WALL = (146, 200, 255)
COLOR_WALL_DIM = (70, 92, 110)
COLOR_GRID_BORDER = (90, 96, 110)
COLOR_ROBOT = (255, 176, 59)
COLOR_START = (90, 220, 140)
COLOR_END = (240, 90, 90)
COLOR_TEXT = (232, 235, 240)
COLOR_TEXT_DIM = (150, 156, 168)
COLOR_BUTTON = (55, 105, 180)
COLOR_BUTTON_HOVER = (75, 130, 210)
COLOR_BUTTON_SELECTED = (90, 200, 140)
COLOR_BUTTON_START = (90, 200, 140)
COLOR_BUTTON_START_HOVER = (110, 220, 160)
COLOR_WIN = (90, 220, 140)
COLOR_LOSE = (240, 90, 90)

DIRS = {
    "N": (0, -1),
    "S": (0, 1),
    "E": (1, 0),
    "W": (-1, 0),
}
OPPOSITE = {"N": "S", "S": "N", "E": "W", "W": "E"}


# ---------------------------------------------------------------------------
# ТАБЛИЦА РЕКОРДОВ (сохраняется в JSON-файл рядом со скриптом)
# ---------------------------------------------------------------------------

def load_leaderboard():
    """Загружает сохранённые рекорды с диска. Если файла нет или он
    повреждён — просто начинаем с пустой таблицы, не роняя игру."""
    try:
        with open(LEADERBOARD_FILE, "r", encoding="utf-8") as f:
            data = json.load(f)
        if isinstance(data, list):
            return data
    except (OSError, json.JSONDecodeError):
        pass
    return []


def save_leaderboard(records):
    try:
        with open(LEADERBOARD_FILE, "w", encoding="utf-8") as f:
            json.dump(records, f, ensure_ascii=False, indent=2)
    except OSError:
        pass  # отсутствие прав на запись не должно ломать игру


def add_leaderboard_record(records, time_used, difficulty_label, time_limit):
    """Добавляет новый результат, сортирует по времени прохождения (лучшее —
    наименьшее) и оставляет только LEADERBOARD_MAX лучших записей."""
    records = list(records)
    records.append({
        "time": round(time_used, 2),
        "difficulty": difficulty_label,
        "time_limit": time_limit,
    })
    records.sort(key=lambda r: r["time"])
    records = records[:LEADERBOARD_MAX]
    save_leaderboard(records)
    return records


# ---------------------------------------------------------------------------
# ЛАБИРИНТ
# ---------------------------------------------------------------------------

class Maze:
    """Хранит структуру лабиринта и умеет генерировать его алгоритмом
    поиска в глубину (recursive backtracker)."""

    def __init__(self, cols, rows):
        self.cols = cols
        self.rows = rows
        self.walls = {
            (c, r): {"N": True, "S": True, "E": True, "W": True}
            for c in range(cols)
            for r in range(rows)
        }
        self.start = (0, 0)
        self.end = (cols - 1, rows - 1)
        self._generate()

    def in_bounds(self, cell):
        c, r = cell
        return 0 <= c < self.cols and 0 <= r < self.rows

    def _generate(self):
        stack = [self.start]
        visited = {self.start}

        while stack:
            c, r = stack[-1]
            neighbours = []
            for d, (dc, dr) in DIRS.items():
                nc, nr = c + dc, r + dr
                if self.in_bounds((nc, nr)) and (nc, nr) not in visited:
                    neighbours.append((d, (nc, nr)))

            if not neighbours:
                stack.pop()
                continue

            d, (nc, nr) = random.choice(neighbours)
            self.walls[(c, r)][d] = False
            self.walls[(nc, nr)][OPPOSITE[d]] = False
            visited.add((nc, nr))
            stack.append((nc, nr))

    def has_wall(self, cell, direction):
        return self.walls[cell][direction]

    def can_move(self, cell, direction):
        if not self.has_wall(cell, direction):
            dc, dr = DIRS[direction]
            target = (cell[0] + dc, cell[1] + dr)
            return self.in_bounds(target)
        return False


# ---------------------------------------------------------------------------
# ТУМАН ВОЙНЫ / ПАМЯТЬ КАРТЫ (аналог построения карты в SLAM)
# ---------------------------------------------------------------------------

class FogOfWar:
    """Отслеживает, какие клетки лабиринта робот уже когда-либо видел.
    Однажды увиденное остаётся на карте навсегда."""

    def __init__(self, maze, radius_cells):
        self.maze = maze
        self.radius = radius_cells
        self.seen_cells = set()
        self.currently_visible = set()

    def update(self, robot_cell):
        cx, cy = robot_cell
        self.currently_visible.clear()
        r = math.ceil(self.radius)
        for dc in range(-r, r + 1):
            for dr in range(-r, r + 1):
                cell = (cx + dc, cy + dr)
                if not self.maze.in_bounds(cell):
                    continue
                if math.hypot(dc, dr) <= self.radius:
                    self.currently_visible.add(cell)
        self.seen_cells |= self.currently_visible

    def progress(self):
        total = self.maze.cols * self.maze.rows
        return len(self.seen_cells) / total if total else 0.0


# ---------------------------------------------------------------------------
# РОБОТ
# ---------------------------------------------------------------------------

def ease_out(t):
    return 1 - (1 - t) ** 2


class Robot:
    """Робот двигается по клеткам (дискретно), но визуально плавно
    "доезжает" от одной клетки до другой."""

    def __init__(self, maze, start_cell, cell_size):
        self.maze = maze
        self.cell = start_cell
        self.cell_size = cell_size
        self.from_px = self._cell_to_px(start_cell)
        self.to_px = self.from_px
        self.progress_t = 1.0
        self.bump_timer = 0.0

    def _cell_to_px(self, cell):
        c, r = cell
        return (
            c * self.cell_size + self.cell_size / 2,
            r * self.cell_size + self.cell_size / 2,
        )

    @property
    def is_moving(self):
        return self.progress_t < 1.0

    @property
    def pixel_pos(self):
        t = ease_out(self.progress_t)
        x = self.from_px[0] + (self.to_px[0] - self.from_px[0]) * t
        y = self.from_px[1] + (self.to_px[1] - self.from_px[1]) * t
        return x, y

    def try_move(self, direction):
        if self.is_moving:
            return False
        if self.maze.can_move(self.cell, direction):
            dc, dr = DIRS[direction]
            new_cell = (self.cell[0] + dc, self.cell[1] + dr)
            self.from_px = self._cell_to_px(self.cell)
            self.to_px = self._cell_to_px(new_cell)
            self.cell = new_cell
            self.progress_t = 0.0
            return True
        else:
            self.bump_timer = 0.15
            return False

    def update(self, dt):
        if self.is_moving:
            self.progress_t = min(1.0, self.progress_t + dt / ROBOT_MOVE_TIME)
        if self.bump_timer > 0:
            self.bump_timer = max(0.0, self.bump_timer - dt)


# ---------------------------------------------------------------------------
# ИНТЕРФЕЙС: КНОПКА
# ---------------------------------------------------------------------------

class Button:
    def __init__(self, rect, text, font,
                 base_color=COLOR_BUTTON, hover_color=COLOR_BUTTON_HOVER,
                 selected_color=COLOR_BUTTON_SELECTED):
        self.rect = pygame.Rect(rect)
        self.text = text
        self.font = font
        self.hovered = False
        self.selected = False
        self.base_color = base_color
        self.hover_color = hover_color
        self.selected_color = selected_color

    def draw(self, surface):
        if self.selected:
            color = self.selected_color
        elif self.hovered:
            color = self.hover_color
        else:
            color = self.base_color
        pygame.draw.rect(surface, color, self.rect, border_radius=8)
        if self.selected:
            pygame.draw.rect(surface, COLOR_TEXT, self.rect, 2, border_radius=8)
        label = self.font.render(self.text, True, COLOR_TEXT)
        surface.blit(label, label.get_rect(center=self.rect.center))

    def handle_mouse_move(self, pos):
        self.hovered = self.rect.collidepoint(pos)

    def is_clicked(self, pos):
        return self.rect.collidepoint(pos)


# ---------------------------------------------------------------------------
# ОСНОВНОЙ КЛАСС ИГРЫ
# ---------------------------------------------------------------------------

STATE_MENU = "menu"
STATE_PLAYING = "playing"
STATE_WON = "won"
STATE_LOST = "lost"


class Game:
    def __init__(self):
        pygame.init()
        pygame.display.set_caption("Лабиринт — навигация робота в тумане войны")
        self.screen = pygame.display.set_mode((WINDOW_W, WINDOW_H))
        self.clock = pygame.time.Clock()

        self.font_small = pygame.font.SysFont("segoeui", 18)
        self.font_medium = pygame.font.SysFont("segoeui", 24, bold=True)
        self.font_large = pygame.font.SysFont("segoeui", 46, bold=True)
        self.font_title = pygame.font.SysFont("segoeui", 22, bold=True)
        self.font_huge = pygame.font.SysFont("segoeui", 34, bold=True)

        self.maze_origin = (MARGIN, MARGIN)  # верхний левый угол области лабиринта
        self.panel_x = MARGIN * 2 + MAZE_AREA_W

        # Настройки, выбираемые в меню
        self.selected_difficulty = "medium"
        self.selected_time = 60

        # Таблица рекордов — загружается один раз при запуске программы
        self.leaderboard = load_leaderboard()

        self._build_menu_buttons()
        self._build_sidebar_buttons()

        self.state = STATE_MENU

    # ------------------------------------------------------------------
    def _build_menu_buttons(self):
        area_cx = self.maze_origin[0] + MAZE_AREA_W / 2

        self.diff_buttons = {}
        btn_w, btn_h, gap = 150, 54, 20
        total_w = btn_w * 3 + gap * 2
        start_x = area_cx - total_w / 2
        y = MARGIN + 190
        for i, key in enumerate(DIFFICULTY_ORDER):
            rect = (start_x + i * (btn_w + gap), y, btn_w, btn_h)
            self.diff_buttons[key] = Button(rect, DIFFICULTIES[key]["label"], self.font_medium)
        self.diff_buttons[self.selected_difficulty].selected = True

        self.time_buttons = {}
        btn_w2 = 150
        total_w2 = btn_w2 * 3 + gap * 2
        start_x2 = area_cx - total_w2 / 2
        y2 = y + btn_h + 70
        for i, t in enumerate(TIME_OPTIONS):
            rect = (start_x2 + i * (btn_w2 + gap), y2, btn_w2, btn_h)
            self.time_buttons[t] = Button(rect, f"{t} сек", self.font_medium)
        self.time_buttons[self.selected_time].selected = True

        start_w, start_h = 260, 64
        self.btn_start = Button(
            (area_cx - start_w / 2, y2 + btn_h + 90, start_w, start_h),
            "СТАРТ", self.font_medium,
            base_color=COLOR_BUTTON_START, hover_color=COLOR_BUTTON_START_HOVER,
        )

        self._menu_diff_label_y = y - 40
        self._menu_time_label_y = y2 - 40

    def _build_sidebar_buttons(self):
        panel_x = self.panel_x
        self.btn_new_game = Button(
            (panel_x, WINDOW_H - 190, SIDEBAR_WIDTH - MARGIN, 46),
            "Новая игра", self.font_medium,
        )
        self.btn_menu = Button(
            (panel_x, WINDOW_H - 130, SIDEBAR_WIDTH - MARGIN, 46),
            "Настройки", self.font_medium,
        )
        self.btn_exit = Button(
            (panel_x, WINDOW_H - 70, SIDEBAR_WIDTH - MARGIN, 46),
            "Выход", self.font_medium,
        )

    # ------------------------------------------------------------------
    def start_new_game(self):
        """Генерирует новый лабиринт с текущими настройками сложности
        и запускает игру."""
        cols, rows = DIFFICULTIES[self.selected_difficulty]["size"]
        self.cell_size = min(MAZE_AREA_W // cols, MAZE_AREA_H // rows)

        maze_px_w = cols * self.cell_size
        maze_px_h = rows * self.cell_size
        offset_x = MARGIN + (MAZE_AREA_W - maze_px_w) // 2
        offset_y = MARGIN + (MAZE_AREA_H - maze_px_h) // 2
        self.maze_origin = (offset_x, offset_y)
        self.maze_px_size = (maze_px_w, maze_px_h)

        self.maze = Maze(cols, rows)
        self.robot = Robot(self.maze, self.maze.start, self.cell_size)
        self.fog = FogOfWar(self.maze, VISION_RADIUS_CELLS)
        self.fog.update(self.robot.cell)

        self.state = STATE_PLAYING
        self.timer_started = False
        self.time_limit = self.selected_time
        self.time_left = float(self.selected_time)
        self.result_time_used = 0.0

    def go_to_menu(self):
        self.state = STATE_MENU

    # ------------------------------------------------------------------
    def handle_input_move(self):
        keys = pygame.key.get_pressed()
        direction = None
        if keys[pygame.K_UP]:
            direction = "N"
        elif keys[pygame.K_DOWN]:
            direction = "S"
        elif keys[pygame.K_LEFT]:
            direction = "W"
        elif keys[pygame.K_RIGHT]:
            direction = "E"
        if direction:
            if not self.timer_started:
                self.timer_started = True
            self.robot.try_move(direction)

    # ------------------------------------------------------------------
    def update(self, dt):
        if self.state == STATE_PLAYING:
            self.handle_input_move()
            self.robot.update(dt)
            self.fog.update(self.robot.cell)

            if self.timer_started and not self.robot.is_moving:
                self.time_left -= dt
                if self.time_left <= 0:
                    self.time_left = 0
                    self.state = STATE_LOST

            if self.robot.cell == self.maze.end:
                self.state = STATE_WON
                self.result_time_used = self.time_limit - self.time_left
                self.leaderboard = add_leaderboard_record(
                    self.leaderboard,
                    self.result_time_used,
                    DIFFICULTIES[self.selected_difficulty]["label"],
                    self.time_limit,
                )

    # ------------------------------------------------------------------
    # ОТРИСОВКА: МЕНЮ
    # ------------------------------------------------------------------
    def draw_menu(self, surface):
        area_cx = self.maze_origin[0] + MAZE_AREA_W / 2

        title = self.font_huge.render("Лабиринт", True, COLOR_TEXT)
        surface.blit(title, title.get_rect(center=(area_cx, MARGIN + 60)))

        subtitle_lines = [
            "Доберись из точки A в точку B, пока не закончилось время.",
        ]
        y = MARGIN + 105
        for line in subtitle_lines:
            t = self.font_small.render(line, True, COLOR_TEXT_DIM)
            surface.blit(t, t.get_rect(center=(area_cx, y)))
            y += 22

        diff_label = self.font_title.render("Сложность (размер лабиринта)", True, COLOR_TEXT)
        surface.blit(diff_label, diff_label.get_rect(center=(area_cx, self._menu_diff_label_y)))
        for btn in self.diff_buttons.values():
            btn.draw(surface)

        time_label = self.font_title.render("Время на прохождение", True, COLOR_TEXT)
        surface.blit(time_label, time_label.get_rect(center=(area_cx, self._menu_time_label_y)))
        for btn in self.time_buttons.values():
            btn.draw(surface)

        self.btn_start.draw(surface)

        hint = self.font_small.render("Управление в игре: стрелки", True, COLOR_TEXT_DIM)
        surface.blit(hint, hint.get_rect(center=(area_cx, self.btn_start.rect.bottom + 30)))

        self.draw_leaderboard_panel(surface)

    def draw_leaderboard_panel(self, surface):
        panel_rect = pygame.Rect(self.panel_x, MARGIN, SIDEBAR_WIDTH - MARGIN, MAZE_AREA_H)
        pygame.draw.rect(surface, COLOR_PANEL, panel_rect, border_radius=10)

        x = self.panel_x + 20
        y = MARGIN + 20

        title = self.font_title.render("Таблица рекордов", True, COLOR_TEXT)
        surface.blit(title, (x, y))
        y += 30

        subtitle = self.font_small.render("Топ-10 лучших результатов", True, COLOR_TEXT_DIM)
        surface.blit(subtitle, (x, y))
        y += 34

        if not self.leaderboard:
            empty = self.font_small.render("Рекордов пока нет —", True, COLOR_TEXT_DIM)
            surface.blit(empty, (x, y))
            y += 22
            empty2 = self.font_small.render("пройди лабиринт первым!", True, COLOR_TEXT_DIM)
            surface.blit(empty2, (x, y))
            return

        for i, record in enumerate(self.leaderboard, start=1):
            rank_color = COLOR_WIN if i <= 3 else COLOR_TEXT
            line1 = self.font_small.render(f"{i}. {record['time']:.2f} с", True, rank_color)
            surface.blit(line1, (x, y))
            y += 22

            meta_text = self.font_small.render(
                f"    {record['difficulty']} · лимит {record['time_limit']} с", True, COLOR_TEXT_DIM
            )
            surface.blit(meta_text, (x, y))
            y += 22

    def handle_menu_click(self, pos):
        for key, btn in self.diff_buttons.items():
            if btn.is_clicked(pos):
                self.selected_difficulty = key
                for k, b in self.diff_buttons.items():
                    b.selected = (k == key)
        for t, btn in self.time_buttons.items():
            if btn.is_clicked(pos):
                self.selected_time = t
                for tt, b in self.time_buttons.items():
                    b.selected = (tt == t)
        if self.btn_start.is_clicked(pos):
            self.start_new_game()

    # ------------------------------------------------------------------
    # ОТРИСОВКА: ИГРОВОЕ ПОЛЕ
    # ------------------------------------------------------------------
    def cell_rect(self, cell):
        c, r = cell
        ox, oy = self.maze_origin
        cs = self.cell_size
        return pygame.Rect(ox + c * cs, oy + r * cs, cs, cs)

    def draw_maze(self, surface):
        ox, oy = self.maze_origin
        maze_px_w, maze_px_h = self.maze_px_size
        cs = self.cell_size

        field_rect = pygame.Rect(ox, oy, maze_px_w, maze_px_h)
        pygame.draw.rect(surface, COLOR_FOG, field_rect)

        for cell in self.fog.seen_cells:
            rect = self.cell_rect(cell)
            color = (
                COLOR_FLOOR_VISIBLE
                if cell in self.fog.currently_visible
                else COLOR_FLOOR_SEEN
            )
            pygame.draw.rect(surface, color, rect)

        for label, cell, color in (
            ("A", self.maze.start, COLOR_START),
            ("B", self.maze.end, COLOR_END),
        ):
            if cell in self.fog.seen_cells:
                rect = self.cell_rect(cell)
                pygame.draw.circle(surface, color, rect.center, max(6, cs // 4))
                text = self.font_small.render(label, True, (20, 20, 20))
                surface.blit(text, text.get_rect(center=rect.center))

        for cell in self.fog.seen_cells:
            rect = self.cell_rect(cell)
            w = self.maze.walls[cell]
            dim = cell not in self.fog.currently_visible
            color = COLOR_WALL_DIM if dim else COLOR_WALL
            if w["N"]:
                pygame.draw.line(surface, color, rect.topleft, rect.topright, WALL_THICKNESS)
            if w["S"]:
                pygame.draw.line(surface, color, rect.bottomleft, rect.bottomright, WALL_THICKNESS)
            if w["W"]:
                pygame.draw.line(surface, color, rect.topleft, rect.bottomleft, WALL_THICKNESS)
            if w["E"]:
                pygame.draw.line(surface, color, rect.topright, rect.bottomright, WALL_THICKNESS)

        rx, ry = self.robot.pixel_pos
        rx, ry = ox + rx, oy + ry
        vision_surf = pygame.Surface((maze_px_w, maze_px_h), pygame.SRCALPHA)
        vision_px = int(VISION_RADIUS_CELLS * cs)
        pygame.draw.circle(vision_surf, (255, 220, 140, 35), (int(rx - ox), int(ry - oy)), vision_px)
        pygame.draw.circle(vision_surf, (255, 220, 140, 120), (int(rx - ox), int(ry - oy)), vision_px, 2)
        surface.blit(vision_surf, (ox, oy))

        shake = 0
        if self.robot.bump_timer > 0:
            shake = int(math.sin(self.robot.bump_timer * 90) * 4)

        robot_radius = max(6, cs // 3)
        pygame.draw.circle(surface, COLOR_ROBOT, (int(rx) + shake, int(ry)), robot_radius)
        pygame.draw.circle(surface, (30, 30, 30), (int(rx) + shake, int(ry)), robot_radius, 2)

        pygame.draw.rect(surface, COLOR_GRID_BORDER, field_rect, 2)

    def draw_panel(self, surface):
        panel_rect = pygame.Rect(self.panel_x, MARGIN, SIDEBAR_WIDTH - MARGIN, MAZE_AREA_H)
        pygame.draw.rect(surface, COLOR_PANEL, panel_rect, border_radius=10)

        x = self.panel_x + 20
        y = MARGIN + 20

        title = self.font_title.render("Навигация робота", True, COLOR_TEXT)
        surface.blit(title, (x, y))
        y += 34

        diff_text = self.font_small.render(
            f"Сложность: {DIFFICULTIES[self.selected_difficulty]['label']}", True, COLOR_TEXT_DIM
        )
        surface.blit(diff_text, (x, y))
        y += 34

        minutes = int(self.time_left) // 60
        seconds = int(self.time_left) % 60
        timer_color = COLOR_TEXT if self.time_left > 20 else COLOR_LOSE
        timer_text = self.font_large.render(f"{minutes:02d}:{seconds:02d}", True, timer_color)
        surface.blit(timer_text, (x, y))
        y += 60

        label = self.font_small.render(
            "время до истечения лимита" if self.timer_started else "таймер запустится при движении",
            True, COLOR_TEXT_DIM,
        )
        surface.blit(label, (x, y))
        y += 40

        progress = self.fog.progress()
        prog_label = self.font_small.render(f"Исследовано карты: {progress * 100:.0f}%", True, COLOR_TEXT)
        surface.blit(prog_label, (x, y))
        y += 26
        bar_rect = pygame.Rect(x, y, SIDEBAR_WIDTH - MARGIN - 40, 14)
        pygame.draw.rect(surface, (50, 54, 64), bar_rect, border_radius=7)
        fill_rect = pygame.Rect(x, y, int(bar_rect.width * progress), 14)
        pygame.draw.rect(surface, COLOR_START, fill_rect, border_radius=7)
        y += 40

        legend_items = [
            (COLOR_START, "A — старт"),
            (COLOR_END, "B — финиш"),
            (COLOR_ROBOT, "робот"),
        ]
        for color, text in legend_items:
            pygame.draw.circle(surface, color, (x + 8, y + 8), 8)
            t = self.font_small.render(text, True, COLOR_TEXT_DIM)
            surface.blit(t, (x + 24, y))
            y += 26

        y += 6
        hint_lines = [
            "Управление: стрелки",
            "Стены видны только в радиусе",
            "обзора и запоминаются навсегда.",
            "Столкновение со стеной не",
            "убивает — просто не даёт",
            "проехать сквозь неё.",
        ]
        for line in hint_lines:
            t = self.font_small.render(line, True, COLOR_TEXT_DIM)
            surface.blit(t, (x, y))
            y += 20

        if self.state == STATE_WON:
            y += 8
            t1 = self.font_medium.render("Победа!", True, COLOR_WIN)
            surface.blit(t1, (x, y))
            y += 30
            t2 = self.font_small.render(f"Время: {self.result_time_used:.1f} сек", True, COLOR_TEXT)
            surface.blit(t2, (x, y))
        elif self.state == STATE_LOST:
            y += 8
            t1 = self.font_medium.render("Время вышло", True, COLOR_LOSE)
            surface.blit(t1, (x, y))

        self.btn_new_game.draw(surface)
        self.btn_menu.draw(surface)
        self.btn_exit.draw(surface)

    def draw_result_overlay(self, surface):
        """Крупная надпись поверх игрового поля при победе/поражении —
        чтобы результат было невозможно не заметить."""
        ox, oy = self.maze_origin
        maze_px_w, maze_px_h = self.maze_px_size

        overlay = pygame.Surface((maze_px_w, maze_px_h), pygame.SRCALPHA)
        overlay.fill((10, 11, 15, 175))
        surface.blit(overlay, (ox, oy))

        cx = ox + maze_px_w / 2
        cy = oy + maze_px_h / 2

        if self.state == STATE_WON:
            main_text = "ПОБЕДА!"
            main_color = COLOR_WIN
            sub_text = f"Время прохождения: {self.result_time_used:.2f} сек"
        else:
            main_text = "ВРЕМЯ ВЫШЛО"
            main_color = COLOR_LOSE
            sub_text = "Попробуй ещё раз!"

        main_surf = self.font_huge.render(main_text, True, main_color)
        surface.blit(main_surf, main_surf.get_rect(center=(cx, cy - 24)))

        sub_surf = self.font_medium.render(sub_text, True, COLOR_TEXT)
        surface.blit(sub_surf, sub_surf.get_rect(center=(cx, cy + 24)))

        hint_surf = self.font_small.render(
            "«Новая игра» — ещё раз · «Настройки» — сменить уровень", True, COLOR_TEXT_DIM
        )
        surface.blit(hint_surf, hint_surf.get_rect(center=(cx, cy + 60)))

    def draw(self):
        self.screen.fill(COLOR_BG)
        if self.state == STATE_MENU:
            self.draw_menu(self.screen)
        else:
            self.draw_maze(self.screen)
            self.draw_panel(self.screen)
            if self.state in (STATE_WON, STATE_LOST):
                self.draw_result_overlay(self.screen)
        pygame.display.flip()

    # ------------------------------------------------------------------
    def run(self):
        running = True
        while running:
            dt = self.clock.tick(FPS) / 1000.0
            mouse_pos = pygame.mouse.get_pos()

            if self.state == STATE_MENU:
                for btn in list(self.diff_buttons.values()) + list(self.time_buttons.values()) + [self.btn_start]:
                    btn.handle_mouse_move(mouse_pos)
            else:
                self.btn_new_game.handle_mouse_move(mouse_pos)
                self.btn_menu.handle_mouse_move(mouse_pos)
                self.btn_exit.handle_mouse_move(mouse_pos)

            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    running = False
                elif event.type == pygame.KEYDOWN:
                    if event.key == pygame.K_ESCAPE:
                        running = False
                elif event.type == pygame.MOUSEBUTTONDOWN and event.button == 1:
                    if self.state == STATE_MENU:
                        self.handle_menu_click(mouse_pos)
                    else:
                        if self.btn_new_game.is_clicked(mouse_pos):
                            self.start_new_game()
                        elif self.btn_menu.is_clicked(mouse_pos):
                            self.go_to_menu()
                        elif self.btn_exit.is_clicked(mouse_pos):
                            running = False
                        elif self.state == STATE_PLAYING and not self.timer_started and pygame.Rect(
                            *self.maze_origin, *self.maze_px_size
                        ).collidepoint(mouse_pos):
                            self.timer_started = True

            self.update(dt)
            self.draw()

        pygame.quit()
        sys.exit()


if __name__ == "__main__":
    Game().run()