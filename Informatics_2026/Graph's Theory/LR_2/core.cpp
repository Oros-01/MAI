// =====================================================================
//  core.cpp — ЯДРО: чтение исходных данных из файлов.
//
//  Здесь нет ни одного алгоритма: только превращение текстового файла
//  в одну из трёх структур из core.h. Всё остальное — в algo.cpp.
//
//  Соответствие с ЛР1: это бывший graph.cpp, где жилось чтение
//  графа из файла со списками смежности.
// =====================================================================

#include "core.h"

#include <algorithm>
#include <fstream>
#include <sstream>

namespace {

// Общий помощник для всех трёх форматов.
//
// Читает файл и превращает его в таблицу чисел:
//   одна строка файла  ->  один вектор чисел.
// Пустые строки пропускаются, всё после символа '#' — комментарий.
//
// Именно из-за этой функции три части не дублируют код чтения.
bool read_numbers(const std::string& fileName, std::vector<std::vector<long long>>& table) {
    std::ifstream fin(fileName);
    if (!fin.is_open()) {
        return false;
    }

    table.clear();
    std::string line;

    while (std::getline(fin, line)) {
        // Отрезаем комментарий, если он есть в строке.
        const std::size_t comment = line.find('#');
        if (comment != std::string::npos) {
            line = line.substr(0, comment);
        }

        std::istringstream iss(line);
        std::vector<long long> row;
        long long value = 0;
        while (iss >> value) {
            row.push_back(value);
        }

        if (!row.empty()) {
            table.push_back(row);
        }
    }

    return !table.empty();
}

} // namespace

// =====================================================================
//  ЧАСТЬ 1. Транспортная сеть
// =====================================================================

bool load_network(const std::string& fileName, Network& net) {
    std::vector<std::vector<long long>> table;
    if (!read_numbers(fileName, table)) {
        return false;
    }

    // Первая строка — "<источник> <сток>", дальше матрица N x N.
    if (table.size() < 2 || table[0].size() < 2) {
        return false;
    }

    net.source = static_cast<int>(table[0][0]);
    net.sink = static_cast<int>(table[0][1]);

    const std::vector<std::vector<long long>> body(table.begin() + 1, table.end());
    net.n = static_cast<int>(body.size());

    // Проверяем, что тело файла — действительно квадратная матрица.
    for (const std::vector<long long>& row : body) {
        if (static_cast<int>(row.size()) != net.n) {
            return false;
        }
    }

    net.cap.assign(net.n + 1, std::vector<long long>(net.n + 1, 0));
    for (int u = 1; u <= net.n; ++u) {
        for (int v = 1; v <= net.n; ++v) {
            net.cap[u][v] = body[u - 1][v - 1];
        }
    }

    if (net.source < 1 || net.source   > net.n ||
        net.sink < 1 || net.sink > net.n ||
        net.source == net.sink) {
        return false;
    }

    return true;
}

// =====================================================================
//  ЧАСТЬ 2. Карта высот
// =====================================================================
  
bool load_map(const std::string& fileName, HeightMap& map) {
    std::vector<std::vector<long long>> table;
    if (!read_numbers(fileName, table)) {
        return false;
    }

    map.rows = static_cast<int>(table.size());
    map.cols = 0;
    for (const std::vector<long long>& row : table) {
        map.cols = std::max(map.cols, static_cast<int>(row.size()));
    }
    if (map.rows == 0 || map.cols == 0) {
        return false;
    }

    // Сразу заполняем нулями: ноль означает стену, поэтому короткие
    // строки файла автоматически становятся стенами.
    map.h.assign(map.rows, std::vector<int>(map.cols, 0));

    for (int r = 0; r < map.rows; ++r) {
        for (int c = 0; c < static_cast<int>(table[r].size()); ++c) {
            map.h[r][c] = static_cast<int>(table[r][c]);
        }
    }

    return true;
}

// =====================================================================
//  ЧАСТЬ 3. Двудольный граф
// =====================================================================

bool load_bipgraph(const std::string& fileName, BipGraph& g) {
    std::vector<std::vector<long long>> table;
    if (!read_numbers(fileName, table)) {
        return false;
    }

    // Определяем формат: если первая строка состоит из одного числа N,
    // а дальше идёт ровно N строк по N чисел — это матрица смежности.
    bool is_matrix = false;
    int matrix_size = 0;

    if (table[0].size() == 1 && table[0][0] > 0) {
        matrix_size = static_cast<int>(table[0][0]);
        if (static_cast<int>(table.size()) == matrix_size + 1) {
            is_matrix = true;
            for (int i = 1; i <= matrix_size; ++i) {
                if (static_cast<int>(table[i].size()) != matrix_size) {
                    is_matrix = false;
                    break;
                }
            }
        }
    }

    if (is_matrix) {
        // Формат 1: матрица смежности. Ненулевое значение = есть ребро.
        g.n = matrix_size;
        g.adj.assign(g.n + 1, std::vector<int>());

        for (int u = 1; u <= g.n; ++u) {
            for (int v = 1; v <= g.n; ++v) {
                if (table[u][v - 1] != 0) {
                    g.adj[u].push_back(v);
                }
            }
        }
        return true;
    }

    // Формат 2: список рёбер, по паре вершин "u v" в строке.
    // Если в строке есть третий столбец — это вес, для двудольности он не нужен.
    int max_vertex = 0;
    for (const std::vector<long long>& row : table) {
        for (std::size_t k = 0; k < 2 && k < row.size(); ++k) {
            max_vertex = std::max(max_vertex, static_cast<int>(row[k]));
        }
    }
    if (max_vertex <= 0) {
        return false;
    }

    g.n = max_vertex;
    g.adj.assign(g.n + 1, std::vector<int>());

    for (const std::vector<long long>& row : table) {
        if (row.size() < 2) {
            continue;
        }
        const int u = static_cast<int>(row[0]);
        const int v = static_cast<int>(row[1]);
        if (u == v || u < 1 || u > g.n || v < 1 || v > g.n) {
            continue;
        }

        // Граф неориентированный, поэтому ребро добавляем в обе стороны.
        g.adj[u].push_back(v);
        g.adj[v].push_back(u);
    }

    return true;
}
