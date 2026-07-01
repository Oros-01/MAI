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
    Стрелки       — движение робота
    Левая кнопка мыши — кнопки интерфейса (Новая игра / Выход)
    Esc           — выход из игры

Запуск:
    python maze_game.py
"""

import sys
import random
import math
import pygame

# ---------------------------------------------------------------------------
# НАСТРОЙКИ (при желании их можно менять — влияет на сложность игры)
# ---------------------------------------------------------------------------

COLS, ROWS = 28, 24         # размер лабиринта в клетках
CELL_SIZE = 42               # размер одной клетки в пикселях
WALL_THICKNESS = 5

VISION_RADIUS_CELLS = 1.0    # радиус обзора робота R (в клетках)
TIME_LIMIT_SECONDS = 60     # время на прохождение

ROBOT_MOVE_TIME = 0.10       # сколько секунд робот едет от клетки до клетки
ROBOT_RADIUS = CELL_SIZE // 3

SIDEBAR_WIDTH = 300
MARGIN = 24

MAZE_PIXEL_W = COLS * CELL_SIZE
MAZE_PIXEL_H = ROWS * CELL_SIZE

WINDOW_W = MAZE_PIXEL_W + SIDEBAR_WIDTH + MARGIN * 3
WINDOW_H = MAZE_PIXEL_H + MARGIN * 2

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
COLOR_ROBOT_GLOW = (255, 176, 59)
COLOR_START = (90, 220, 140)
COLOR_END = (240, 90, 90)
COLOR_TEXT = (232, 235, 240)
COLOR_TEXT_DIM = (150, 156, 168)
COLOR_BUTTON = (55, 105, 180)
COLOR_BUTTON_HOVER = (75, 130, 210)
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
# ЛАБИРИНТ
# ---------------------------------------------------------------------------

class Maze:
    """Хранит структуру лабиринта и умеет генерировать его алгоритмом
    поиска в глубину (recursive backtracker)."""

    def __init__(self, cols, rows):
        self.cols = cols
        self.rows = rows
        # На старте у каждой клетки есть все 4 стены
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
            # Прорубаем проход между текущей и соседней клеткой
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

    def all_wall_segments(self):
        """Возвращает список всех уникальных сегментов стен (для подсчёта
        прогресса исследования). Каждая внутренняя стена считается один
        раз, внешние стены — по границе поля."""
        segments = []
        for (c, r), w in self.walls.items():
            for d in ("N", "W"):
                if w[d]:
                    segments.append(((c, r), d))
            # Восточная и южная стены учитываются только если это
            # граница поля (иначе их посчитает сосед через N/W)
            if w["E"] and c == self.cols - 1:
                segments.append(((c, r), "E"))
            if w["S"] and r == self.rows - 1:
                segments.append(((c, r), "S"))
        return segments


# ---------------------------------------------------------------------------
# ТУМАН ВОЙНЫ / ПАМЯТЬ КАРТЫ (аналог построения карты в SLAM)
# ---------------------------------------------------------------------------

class FogOfWar:
    """Отслеживает, какие клетки лабиринта робот уже когда-либо видел.
    Однажды увиденное остаётся на карте навсегда — робот "запоминает"
    исследованную территорию."""

    def __init__(self, maze, radius_cells):
        self.maze = maze
        self.radius = radius_cells
        self.seen_cells = set()          # когда-либо увиденные клетки
        self.currently_visible = set()   # видимые прямо сейчас

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

class Robot:
    """Робот двигается по клеткам (дискретно), но визуально плавно
    "доезжает" от одной клетки до другой — это проще и надёжнее
    реализовать и объяснить в отчёте, чем непрерывную физику, при этом
    выглядит как живое движение."""

    def __init__(self, maze, start_cell):
        self.maze = maze
        self.cell = start_cell
        self.from_px = self._cell_to_px(start_cell)
        self.to_px = self.from_px
        self.progress_t = 1.0  # 1.0 = стоим на месте (анимация завершена)
        self.bump_timer = 0.0  # маленькая "тряска" при ударе о стену

    @staticmethod
    def _cell_to_px(cell):
        c, r = cell
        return (
            c * CELL_SIZE + CELL_SIZE / 2,
            r * CELL_SIZE + CELL_SIZE / 2,
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
        """Пытается сдвинуться в направлении direction. Если там стена —
        робот остаётся на месте (столкновение просто блокирует движение)."""
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


def ease_out(t):
    return 1 - (1 - t) ** 2


# ---------------------------------------------------------------------------
# ИНТЕРФЕЙС: КНОПКА
# ---------------------------------------------------------------------------

class Button:
    def __init__(self, rect, text, font):
        self.rect = pygame.Rect(rect)
        self.text = text
        self.font = font
        self.hovered = False

    def draw(self, surface):
        color = COLOR_BUTTON_HOVER if self.hovered else COLOR_BUTTON
        pygame.draw.rect(surface, color, self.rect, border_radius=8)
        label = self.font.render(self.text, True, COLOR_TEXT)
        surface.blit(
            label,
            label.get_rect(center=self.rect.center),
        )

    def handle_mouse_move(self, pos):
        self.hovered = self.rect.collidepoint(pos)

    def is_clicked(self, pos):
        return self.rect.collidepoint(pos)


# ---------------------------------------------------------------------------
# ОСНОВНОЙ КЛАСС ИГРЫ
# ---------------------------------------------------------------------------

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

        panel_x = MARGIN * 2 + MAZE_PIXEL_W
        self.btn_new_game = Button(
            (panel_x, WINDOW_H - 130, SIDEBAR_WIDTH - MARGIN, 46),
            "Новая игра", self.font_medium,
        )
        self.btn_exit = Button(
            (panel_x, WINDOW_H - 70, SIDEBAR_WIDTH - MARGIN, 46),
            "Выход", self.font_medium,
        )

        self.maze_origin = (MARGIN, MARGIN)
        self.panel_x = panel_x

        self.reset_game()

    # ------------------------------------------------------------------
    def reset_game(self):
        self.maze = Maze(COLS, ROWS)
        self.robot = Robot(self.maze, self.maze.start)
        self.fog = FogOfWar(self.maze, VISION_RADIUS_CELLS)
        self.fog.update(self.robot.cell)

        self.state = STATE_PLAYING
        self.timer_started = False
        self.time_left = float(TIME_LIMIT_SECONDS)
        self.result_time_used = 0.0

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
                self.result_time_used = TIME_LIMIT_SECONDS - self.time_left

    # ------------------------------------------------------------------
    # ОТРИСОВКА
    # ------------------------------------------------------------------
    def cell_rect(self, cell):
        c, r = cell
        ox, oy = self.maze_origin
        return pygame.Rect(
            ox + c * CELL_SIZE, oy + r * CELL_SIZE, CELL_SIZE, CELL_SIZE
        )

    def draw_maze(self, surface):
        ox, oy = self.maze_origin

        # Фон поля (туман по умолчанию)
        field_rect = pygame.Rect(ox, oy, MAZE_PIXEL_W, MAZE_PIXEL_H)
        pygame.draw.rect(surface, COLOR_FOG, field_rect)

        # Пол — только для уже увиденных клеток
        for cell in self.fog.seen_cells:
            rect = self.cell_rect(cell)
            color = (
                COLOR_FLOOR_VISIBLE
                if cell in self.fog.currently_visible
                else COLOR_FLOOR_SEEN
            )
            pygame.draw.rect(surface, color, rect)

        # Точки A и B — подписаны, если клетка уже видна
        for label, cell, color in (
            ("A", self.maze.start, COLOR_START),
            ("B", self.maze.end, COLOR_END),
        ):
            if cell in self.fog.seen_cells:
                rect = self.cell_rect(cell)
                pygame.draw.circle(surface, color, rect.center, CELL_SIZE // 4)
                text = self.font_small.render(label, True, (20, 20, 20))
                surface.blit(text, text.get_rect(center=rect.center))

        # Стены — рисуем только те, что относятся к увиденным клеткам
        for cell in self.fog.seen_cells:
            c, r = cell
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

        # Радиус обзора робота
        rx, ry = self.robot.pixel_pos
        rx, ry = ox + rx, oy + ry
        vision_surf = pygame.Surface((MAZE_PIXEL_W, MAZE_PIXEL_H), pygame.SRCALPHA)
        pygame.draw.circle(
            vision_surf, (255, 220, 140, 35),
            (int(rx - ox), int(ry - oy)),
            int(VISION_RADIUS_CELLS * CELL_SIZE),
        )
        pygame.draw.circle(
            vision_surf, (255, 220, 140, 120),
            (int(rx - ox), int(ry - oy)),
            int(VISION_RADIUS_CELLS * CELL_SIZE), 2,
        )
        surface.blit(vision_surf, (ox, oy))

        # Небольшая тряска при ударе о стену
        shake = 0
        if self.robot.bump_timer > 0:
            shake = int(math.sin(self.robot.bump_timer * 90) * 4)

        pygame.draw.circle(
            surface, COLOR_ROBOT, (int(rx) + shake, int(ry)), ROBOT_RADIUS
        )
        pygame.draw.circle(
            surface, (30, 30, 30), (int(rx) + shake, int(ry)), ROBOT_RADIUS, 2
        )

        # Рамка поля
        pygame.draw.rect(surface, COLOR_GRID_BORDER, field_rect, 2)

    def draw_panel(self, surface):
        panel_rect = pygame.Rect(
            self.panel_x, MARGIN, SIDEBAR_WIDTH - MARGIN, MAZE_PIXEL_H
        )
        pygame.draw.rect(surface, COLOR_PANEL, panel_rect, border_radius=10)

        x = self.panel_x + 20
        y = MARGIN + 20

        title = self.font_title.render("Навигация робота", True, COLOR_TEXT)
        surface.blit(title, (x, y))
        y += 42

        # Таймер
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

        # Прогресс исследования
        progress = self.fog.progress()
        prog_label = self.font_small.render(
            f"Исследовано карты: {progress * 100:.0f}%", True, COLOR_TEXT
        )
        surface.blit(prog_label, (x, y))
        y += 26
        bar_rect = pygame.Rect(x, y, SIDEBAR_WIDTH - MARGIN - 40, 14)
        pygame.draw.rect(surface, (50, 54, 64), bar_rect, border_radius=7)
        fill_rect = pygame.Rect(x, y, int(bar_rect.width * progress), 14)
        pygame.draw.rect(surface, COLOR_START, fill_rect, border_radius=7)
        y += 40

        # Легенда
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

        y += 10
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
            y += 22

        # Результат
        if self.state == STATE_WON:
            y += 10
            t1 = self.font_medium.render("Победа!", True, COLOR_WIN)
            surface.blit(t1, (x, y))
            y += 30
            t2 = self.font_small.render(
                f"Время: {self.result_time_used:.1f} сек", True, COLOR_TEXT
            )
            surface.blit(t2, (x, y))
        elif self.state == STATE_LOST:
            y += 10
            t1 = self.font_medium.render("Время вышло", True, COLOR_LOSE)
            surface.blit(t1, (x, y))

        self.btn_new_game.draw(surface)
        self.btn_exit.draw(surface)

    def draw(self):
        self.screen.fill(COLOR_BG)
        self.draw_maze(self.screen)
        self.draw_panel(self.screen)
        pygame.display.flip()

    # ------------------------------------------------------------------
    def run(self):
        running = True
        while running:
            dt = self.clock.tick(FPS) / 1000.0
            mouse_pos = pygame.mouse.get_pos()
            self.btn_new_game.handle_mouse_move(mouse_pos)
            self.btn_exit.handle_mouse_move(mouse_pos)

            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    running = False
                elif event.type == pygame.KEYDOWN:
                    if event.key == pygame.K_ESCAPE:
                        running = False
                elif event.type == pygame.MOUSEBUTTONDOWN and event.button == 1:
                    if self.btn_new_game.is_clicked(mouse_pos):
                        self.reset_game()
                    elif self.btn_exit.is_clicked(mouse_pos):
                        running = False
                    elif not self.timer_started and pygame.Rect(
                        *self.maze_origin, MAZE_PIXEL_W, MAZE_PIXEL_H
                    ).collidepoint(mouse_pos):
                        # клик по полю тоже запускает таймер (по ТЗ)
                        self.timer_started = True

            self.update(dt)
            self.draw()

        pygame.quit()
        sys.exit()


if __name__ == "__main__":
    Game().run()
