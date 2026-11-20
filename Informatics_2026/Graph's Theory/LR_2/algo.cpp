// =====================================================================
//  algo.cpp — АЛГОРИТМЫ (бывший graph_algo.cpp в ЛР1).
//
//  Здесь три алгоритма задания №2:
//    часть 1 — Форд-Фалкерсон (максимальный поток);
//    часть 2 — Дейкстра и A* с эвристикой Чебышева (поиск пути);
//    часть 3 — раскраска в 2 цвета и алгоритм Куна (паросочетание).
//
//  Никакого чтения файлов здесь нет: структуры приходят готовыми из core.cpp.
// =====================================================================

#include "core.h"

#include <algorithm>
#include <chrono>
#include <climits>
#include <cmath>
#include <cstdlib>
#include <functional>
#include <limits>
#include <queue>

// =====================================================================
//  ЧАСТЬ 1. Максимальный поток — Форд-Фалкерсон
// =====================================================================

namespace {

// Поиск увеличивающего пути в остаточной сети обходом в ширину.
//
// Остаточная пропускная способность дуги u -> v — это
// сколько ещё можно по ней пропустить: cap[u][v] - flow[u][v].
// Если путь до стока нашёлся, в parent[] остаются предшественники
// вершин этого пути.
bool bfs_augment(const Network& net, const std::vector<std::vector<long long>>& flow,
                 std::vector<int>& parent) {
    std::fill(parent.begin(), parent.end(), -1);

    std::vector<bool> visited(net.n + 1, false);
    std::queue<int> q;

    q.push(net.source);
    visited[net.source] = true;

    while (!q.empty()) {
        const int u = q.front();
        q.pop();

        for (int v = 1; v <= net.n; ++v) {
            if (visited[v]) {
                continue;
            }
            if (net.cap[u][v] - flow[u][v] <= 0) {
                continue;   // по этой дуге уже некуда пропускать поток
            }

            visited[v] = true;
            parent[v] = u;

            if (v == net.sink) {
                return true;   // дошли до стока — путь найден
            }
            q.push(v);
        }
    }

    return false;   // пути нет, значит поток уже максимальный
}

} // namespace

long long ford_fulkerson(const Network& net, std::vector<std::vector<long long>>& flow) {
    flow.assign(net.n + 1, std::vector<long long>(net.n + 1, 0));

    std::vector<int> parent(net.n + 1, -1);
    long long max_flow = 0;

    // Пока в остаточной сети есть путь из источника в сток — пускаем по нему поток.
    while (bfs_augment(net, flow, parent)) {
        // 1. Смотрим, сколько можно пропустить по найденному пути:
        //    это минимум остаточных пропускных способностей вдоль пути.
        long long push = LLONG_MAX;
        for (int v = net.sink; v != net.source; v = parent[v]) {
            const int u = parent[v];
            push = std::min(push, net.cap[u][v] - flow[u][v]);
        }

        // 2. Увеличиваем поток вдоль пути и уменьшаем его в обратную сторону.
        //    Отрицательное значение flow[v][u] как раз и означает, что
        //    по встречной дуге поток можно "отменить".
        for (int v = net.sink; v != net.source; v = parent[v]) {
            const int u = parent[v];
            flow[u][v] += push;
            flow[v][u] -= push;
        }

        max_flow += push;
    }

    return max_flow;
}

// =====================================================================
//  ЧАСТЬ 2. Поиск пути — Дейкстра и A* с эвристикой Чебышева
// =====================================================================

namespace {

// Стоимость перехода между соседними клетками — модуль разности высот.
// Если высоты равны, шаг всё равно чего-то стоит, поэтому минимум 1.
// Именно из-за этого "минимум 1" эвристика Чебышева никогда не завышает
// длину пути, и A* находит тот же ответ, что и Дейкстра.
double step_cost(const HeightMap& map, int r1, int c1, int r2, int c2) {
    const int diff = std::abs(map.h[r1][c1] - map.h[r2][c2]);
    return diff > 0 ? static_cast<double>(diff) : 1.0;
}

// Эвристика Чебышева: max(|dr|, |dc|).
// Это минимальное число шагов по диагонали и по прямой до цели.
double chebyshev(int r, int c, int goal_r, int goal_c) {
    return static_cast<double>(std::max(std::abs(r - goal_r), std::abs(c - goal_c)));
}

} // namespace

PathResult find_path(const HeightMap& map, int r1, int c1, int r2, int c2, bool use_astar) {
    PathResult result;
    result.name = use_astar ? "A* (эвристика Чебышева)" : "Дейкстра";

    const double INF = std::numeric_limits<double>::infinity();
    const int total = map.rows * map.cols;

    // Если старт или финиш — стена, искать нечего.
    if (!map.passable(r1, c1) || !map.passable(r2, c2)) {
        return result;   // length остаётся -1
    }

    const int start = map.id(r1, c1);
    const int finish = map.id(r2, c2);

    const auto begin = std::chrono::steady_clock::now();

    std::vector<double> dist(total, INF);   // dist[i] — лучшая известная длина до клетки i
    std::vector<int> parent(total, -1);     // parent[i] — откуда пришли в i
    std::vector<bool> closed(total, false); // клетка уже обработана окончательно

    // В очереди лежат пары (оценка f, номер клетки). Первой берётся меньшая f.
    using Item = std::pair<double, int>;
    std::priority_queue<Item, std::vector<Item>, std::greater<Item>> open;

    dist[start] = 0.0;
    open.push({ use_astar ? chebyshev(r1, c1, r2, c2) : 0.0, start });

    // 8 направлений: 4 по сторонам и 4 по диагонали.
    const int dr[8] = { -1, 1, 0, 0, -1, -1, 1, 1 };
    const int dc[8] = { 0, 0, -1, 1, -1, 1, -1, 1 };

    int visited = 0;   // сколько клеток просмотрели

    while (!open.empty()) {
        const int u = open.top().second;
        open.pop();

        if (closed[u]) {
            continue;   // эту клетку уже обработали через более короткий путь
        }
        closed[u] = true;
        ++visited;

        if (u == finish) {
            break;   // дошли до цели
        }

        const int ur = u / map.cols;
        const int uc = u % map.cols;

        for (int k = 0; k < 8; ++k) {
            const int vr = ur + dr[k];
            const int vc = uc + dc[k];

            if (!map.inside(vr, vc) || !map.passable(vr, vc)) {
                continue;
            }

            const int v = map.id(vr, vc);
            const double candidate = dist[u] + step_cost(map, ur, uc, vr, vc);

            if (candidate < dist[v]) {
                dist[v] = candidate;
                parent[v] = u;

                // Собственно разница между алгоритмами: у Дейкстры добавка 0,
                // у A* — эвристика Чебышева.
                const double h = use_astar ? chebyshev(vr, vc, r2, c2) : 0.0;
                open.push({ candidate + h, v });
            }
        }
    }

    const auto end = std::chrono::steady_clock::now();
    result.time_ms = std::chrono::duration<double, std::milli>(end - begin).count();

    if (total > 0) {
        result.visited_percent = 100.0 * visited / total;
    }

    if (dist[finish] == INF) {
        return result;   // пути нет, length остаётся -1
    }

    result.length = dist[finish];

    // Восстанавливаем путь по parent[] от финиша к старту и разворачиваем.
    for (int v = finish; v != -1; v = parent[v]) {
        result.path.push_back({ v / map.cols, v % map.cols });
    }
    std::reverse(result.path.begin(), result.path.end());

    return result;
}

// =====================================================================
//  ЧАСТЬ 3. Двудольность и максимальное паросочетание
// =====================================================================

bool is_bipartite(const BipGraph& g, std::vector<int>& color) {
    // -1 — вершина ещё не покрашена, 0 и 1 — два цвета.
    color.assign(g.n + 1, -1);

    std::queue<int> q;

    for (int start = 1; start <= g.n; ++start) {
        if (color[start] != -1) {
            continue;
        }

        // Красим первую вершину очередной компоненты связности.
        color[start] = 0;
        q.push(start);

        while (!q.empty()) {
            const int u = q.front();
            q.pop();

            for (int v : g.adj[u]) {
                if (color[v] == -1) {
                    color[v] = 1 - color[u];   // сосед всегда другого цвета
                    q.push(v);
                } else if (color[v] == color[u]) {
                    return false;   // два соседа одного цвета — граф не двудольный
                }
            }
        }
    }

    return true;
}

namespace {

// Попытка найти увеличивающую цепь из левой вершины u — это и есть алгоритм Куна.
//
// Логика: идём по соседям v. Если v свободна — забираем её.
// Если v занята вершиной match_right[v], пробуем "перевесить" эту вершину
// на другую соседку (рекурсивно). Получилось — забираем v себе.
bool try_kuhn(int u, const BipGraph& g, std::vector<int>& match_right, std::vector<bool>& used) {
    if (used[u]) {
        return false;   // в этой попытке вершину уже смотрели
    }
    used[u] = true;

    for (int v : g.adj[u]) {
        if (match_right[v] == -1 || try_kuhn(match_right[v], g, match_right, used)) {
            match_right[v] = u;
            return true;
        }
    }

    return false;
}

} // namespace

int max_matching(const BipGraph& g, const std::vector<int>& color,
                 std::vector<std::pair<int, int>>& matching) {
    matching.clear();

    // Без раскраски мы не знаем, какая доля левая, а какая правая.
    if (static_cast<int>(color.size()) != g.n + 1) {
        return 0;
    }

    // match_right[v] — какая левая вершина заняла правую вершину v (-1 = свободна).
    std::vector<int> match_right(g.n + 1, -1);
    int size = 0;

    // Перебираем все вершины левой доли — это вершины цвета 0.
    for (int u = 1; u <= g.n; ++u) {
        if (color[u] != 0) {
            continue;
        }

        std::vector<bool> used(g.n + 1, false);
        if (try_kuhn(u, g, match_right, used)) {
            ++size;   // нашли увеличивающую цепь — паросочетание выросло на 1
        }
    }

    // Собираем найденные рёбра для вывода в файл.
    for (int v = 1; v <= g.n; ++v) {
        if (match_right[v] != -1) {
            matching.push_back({ match_right[v], v });
        }
    }

    return size;
}
