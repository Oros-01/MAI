//Здесь мы реализуем Беллман-Форд, Приму и BFS потому что легче найти и прочитать
//и потому что лаба моя.
// graph_algo.cpp
// Здесь живут алгоритмы на графе. Они объявлены как методы класса Graph
// (см. graph.h), но реализация вынесена в отдельный файл, чтобы graph.cpp
// не раздувался.

#include "graph.h"

#include <queue>      // для BFS (std::queue)
#include <algorithm>  // для std::sort
#include <climits>    // для LLONG_MAX (понадобится в Беллмане-Форде)

// ---------------------------------------------------------------
// Связность и компоненты связности
// ---------------------------------------------------------------

bool Graph::is_connected() const {
    return connected_components().size() == 1;
}

std::vector<std::vector<int>> Graph::connected_components() const {
    std::vector<std::vector<int>> result;

    int n = (int)exists_.size();
    std::vector<bool> visited(n, false);

    for (int start = 0; start < n; start++) {
        if (!exists_[start]) continue;
        if (visited[start])  continue;

        std::vector<int> component;
        std::queue<int> q;
        q.push(start);
        visited[start] = true;

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            component.push_back(u);

            for (int v = 0; v < n; v++) {
                if (!exists_[v]) continue;
                if (visited[v])  continue;
                if (linked_undirected(u, v)) {
                    visited[v] = true;
                    q.push(v);
                }
            }
        }

        std::sort(component.begin(), component.end());
        result.push_back(component);
    }
    return result;
}


// ---------------------------------------------------------------
// Беллман-Форд: кратчайшие пути от одной вершины до всех остальных
// ---------------------------------------------------------------

bool Graph::bellman_ford(int start, std::vector<long long>& dist, std::vector<int>& prev) const {
    int n = (int)exists_.size();
    const long long INF = 1000000000000000000LL; // большое число (1e18)

    dist.assign(n, INF);
    prev.assign(n, -1);

    if (!has_vertex(start)) {
        return false; // нет такой вершины
    }

    dist[start] = 0;

    // Собираем все дуги/рёбра. Для неориентированного графа нужно обе стороны.
    std::vector<Edge> edges;
    for (int i = 0; i < n; i++) {
        if (!exists_[i]) continue;
        for (int j = 0; j < n; j++) {
            if (!exists_[j]) continue;
            if (is_edge_[i][j]) {
                Edge e;
                e.from = i;
                e.to = j;
                e.weight = weight_[i][j];
                edges.push_back(e);
            }
        }
    }

    // Основной цикл: делаем n-1 проход по всем рёбрам
    int V = size();
    for (int step = 0; step < V - 1; step++) {
        bool changed = false;
        for (size_t k = 0; k < edges.size(); k++) {
            int u = edges[k].from;
            int v = edges[k].to;
            long long w = edges[k].weight;

            if (dist[u] != INF && dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                prev[v] = u;
                changed = true;
            }
        }
        if (!changed) break; // если ничего не улучшилось — можно выходить раньше
    }

    // Проверка на цикл отрицательного веса (n-й проход)
    for (size_t k = 0; k < edges.size(); k++) {
        int u = edges[k].from;
        int v = edges[k].to;
        long long w = edges[k].weight;

        if (dist[u] != INF && dist[u] + w < dist[v]) {
            return false; // нашли цикл отрицательного веса
        }
    }
    return true;
}

std::vector<int> Graph::restore_path(int start, int finish, const std::vector<int>& prev) const {
    std::vector<int> path;
    if (start == finish) {
        path.push_back(start);
        return path;
    }
    int cur = finish;
    while (cur != -1 && cur != start) {
        path.push_back(cur);
        cur = prev[cur];
    }
    if (cur != start) {
        return std::vector<int>(); // пути нет
    }
    path.push_back(start);
    std::reverse(path.begin(), path.end());
    return path;
}

// ---------------------------------------------------------------
// Прим: минимальное остовное дерево
// ---------------------------------------------------------------

std::vector<Edge> Graph::prim() const {
    std::vector<Edge> result;
    int n = (int)exists_.size();
    if (n == 0) return result;

    std::vector<bool> in_tree(n, false); // какие вершины уже в остове

    // Ищем первую существующую вершину, чтобы начать
    int start = -1;
    for (int i = 0; i < n; i++) {
        if (exists_[i]) {
            start = i;
            break;
        }
    }
    if (start == -1) return result; // нет вершин

    in_tree[start] = true;
    int tree_size = 1;
    int total_vertices = size();

    // Пока в остове не все вершины
    while (tree_size < total_vertices) {
        long long best_weight = -1; // -1 значит "ещё не нашли"
        int best_u = -1, best_v = -1;

        // Ищем ребро минимального веса из остова наружу
        for (int u = 0; u < n; u++) {
            if (!exists_[u] || !in_tree[u]) continue;

            for (int v = 0; v < n; v++) {
                if (!exists_[v] || in_tree[v]) continue;
                if (!linked_undirected(u, v)) continue; // ребро между u и v (без учёта направления)

                long long w = weight_[u][v];
                if (!is_edge_[u][v]) {
                    w = weight_[v][u]; // если дуга v->u, берём вес оттуда
                }

                if (best_weight == -1 || w < best_weight) {
                    best_weight = w;
                    best_u = u;
                    best_v = v;
                }
            }
        }

        if (best_u == -1) {
            // Остов кончился — значит, граф несвязный, начинаем новую компоненту.
            // Ищем любую вершину, которой ещё нет в остове.
            int next_start = -1;
            for (int i = 0; i < n; i++) {
                if (exists_[i] && !in_tree[i]) {
                    next_start = i;
                    break;
                }
            }
            if (next_start == -1) break; // все вершины в остове (такого не должно случиться, но на всякий)
            in_tree[next_start] = true;
            tree_size++;
            continue;
        }

        // Добавляем найденное ребро в остов
        Edge e;
        e.from = best_u;
        e.to = best_v;
        e.weight = best_weight;
        result.push_back(e);

        in_tree[best_v] = true;
        tree_size++;
    }

    return result;
}

