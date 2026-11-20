// =====================================================================
//  main.cpp — ИСПОЛНИТЕЛЬСКИЙ файл (как main.cpp в ЛР1).
//
//  Что делает:
//    1) разбирает аргументы командной строки;
//    2) вызывает нужную часть задания;
//    3) записывает результат в файл.
//
//  Запуск (первый аргумент — номер части задания):
//
//    lr2.exe 1 data/net.txt
//    lr2.exe 1 data/net.txt -o out_flow.txt
//
//    lr2.exe 2 data/map.txt -a 1 1 -b 8 8
//    lr2.exe 2 data/map.txt -a 1 1 -b 8 8 -o out_path.txt
//
//    lr2.exe 3 data/bip.txt
//    lr2.exe 3 data/bip.txt -o out_match.txt
//
//  Ключ -o задаёт имя выходного файла. Если его нет — пишем в ans.txt
//  (именно этого требует задание).
//  Координаты в ключах -a и -b нумеруются с 1: сначала строка, потом столбец.
//
//  Если запустить программу вообще без аргументов — откроется меню.
//  Это удобно для показа работы на защите.
// =====================================================================

#include "core.h"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#endif

namespace {

// Имя файла результата по умолчанию (требование задания).
const char* const kDefaultOutput = "ans.txt";

// ---------------------------------------------------------------
//  Печать результата поиска пути (используется в части 2)
// ---------------------------------------------------------------
void print_path_result(std::ostream& out, const PathResult& r) {
    out << "--- " << r.name << " ---\n";

    if (r.length < 0) {
        out << "Путь не найден.\n\n";
        return;
    }

    out << "Длина найденного пути: " << r.length << "\n";
    out << "Процент просмотренных вершин: " << r.visited_percent << "%\n";
    out << "Время выполнения: " << r.time_ms << " мс\n";
    out << "Вершины, составляющие путь (" << r.path.size() << " шт.):\n  ";

    for (std::size_t i = 0; i < r.path.size(); ++i) {
        out << "(" << r.path[i].first + 1 << "," << r.path[i].second + 1 << ")";
        if (i + 1 < r.path.size()) {
            out << " -> ";
        }
    }
    out << "\n\n";
}

// ---------------------------------------------------------------
//  ЧАСТЬ 1. Максимальный поток
// ---------------------------------------------------------------
int run_part1(const std::string& inputFile, const std::string& outputFile) {
    Network net;
    if (!load_network(inputFile, net)) {
        std::cout << "Ошибка: не удалось прочитать транспортную сеть из файла "
                  << inputFile << "\n";
        return 1;
    }

    std::vector<std::vector<long long>> flow;
    const long long max_flow = ford_fulkerson(net, flow);

    std::ofstream out(outputFile);
    if (!out.is_open()) {
        std::cout << "Ошибка: не удалось открыть файл " << outputFile << " на запись\n";
        return 1;
    }

    out << "Источник: " << net.source << "\n";
    out << "Сток: " << net.sink << "\n";
    out << "Суммарная величина потока: " << max_flow << "\n\n";
    out << "Рёбра сети (дуга, поток, пропускная способность):\n";

    for (int u = 1; u <= net.n; ++u) {
        for (int v = 1; v <= net.n; ++v) {
            if (!net.is_edge(u, v)) {
                continue;
            }

            // Отрицательный поток — это отмена потока по встречной дуге,
            // в отчёте такое значение показывать не нужно.
            long long f = flow[u][v];
            if (f < 0) {
                f = 0;
            }

            out << "  " << u << " -> " << v
                << "   поток = " << f
                << "   пропускная способность = " << net.cap[u][v] << "\n";
        }
    }

    out.close();

    std::cout << "Часть 1 готова. Максимальный поток = " << max_flow
              << ". Результат записан в " << outputFile << "\n";
    return 0;
}

// ---------------------------------------------------------------
//  ЧАСТЬ 2. Поиск пути: Дейкстра и A* с эвристикой Чебышева
// ---------------------------------------------------------------
int run_part2(const std::string& inputFile, int r1, int c1, int r2, int c2,
              const std::string& outputFile) {
    HeightMap map;
    if (!load_map(inputFile, map)) {
        std::cout << "Ошибка: не удалось прочитать карту из файла " << inputFile << "\n";
        return 1;
    }

    // В файле координаты с 1, внутри программы — с 0.
    if (!map.inside(r1 - 1, c1 - 1) || !map.inside(r2 - 1, c2 - 1)) {
        std::cout << "Ошибка: координаты выходят за карту. "
                  << "Строки: 1.." << map.rows
                  << ", столбцы: 1.." << map.cols << "\n";
        return 1;
    }

    const PathResult dijkstra = find_path(map, r1 - 1, c1 - 1, r2 - 1, c2 - 1, false);
    const PathResult astar = find_path(map, r1 - 1, c1 - 1, r2 - 1, c2 - 1, true);

    std::ofstream out(outputFile);
    if (!out.is_open()) {
        std::cout << "Ошибка: не удалось открыть файл " << outputFile << " на запись\n";
        return 1;
    }

    out << "Карта: " << map.rows << " x " << map.cols << "\n";
    out << "Начало пути: (" << r1 << "," << c1 << ")\n";
    out << "Конец пути: (" << r2 << "," << c2 << ")\n\n";

    print_path_result(out, dijkstra);
    print_path_result(out, astar);

    out.close();

    // Короткая сводка в консоль.
    std::cout << "Часть 2 готова.\n";
    for (const PathResult& r : { dijkstra, astar }) {
        std::cout << "  " << r.name << ": ";
        if (r.length < 0) {
            std::cout << "путь не найден\n";
        } else {
            std::cout << "длина " << r.length
                      << ", просмотрено " << r.visited_percent << "%\n";
        }
    }
    std::cout << "Результат записан в " << outputFile << "\n";

    return 0;
}

// ---------------------------------------------------------------
//  ЧАСТЬ 3. Двудольность и максимальное паросочетание
// ---------------------------------------------------------------
int run_part3(const std::string& inputFile, const std::string& outputFile) {
    BipGraph g;
    if (!load_bipgraph(inputFile, g)) {
        std::cout << "Ошибка: не удалось прочитать граф из файла " << inputFile << "\n";
        return 1;
    }

    std::vector<int> color;
    const bool bipartite = is_bipartite(g, color);

    std::vector<std::pair<int, int>> matching;
    const int matching_size = bipartite ? max_matching(g, color, matching) : 0;

    std::ofstream out(outputFile);
    if (!out.is_open()) {
        std::cout << "Ошибка: не удалось открыть файл " << outputFile << " на запись\n";
        return 1;
    }

    out << "Граф является двудольным: " << (bipartite ? "да" : "нет") << "\n";

    if (bipartite) {
        // В задании эти два пункта названы по-разному, но число рёбер
        // паросочетания и есть его размер, поэтому числа совпадают.
        out << "Число паросочетаний: " << matching_size << "\n";
        out << "Размер максимального паросочетания: " << matching_size << "\n";
        out << "Рёбра, входящие в паросочетание:\n";

        for (const std::pair<int, int>& e : matching) {
            out << "  " << e.first << " - " << e.second << "\n";
        }
    }

    out.close();

    std::cout << "Часть 3 готова. Двудольный: " << (bipartite ? "да" : "нет");
    if (bipartite) {
        std::cout << ", размер максимального паросочетания = " << matching_size;
    }
    std::cout << ". Результат записан в " << outputFile << "\n";

    return 0;
}

// ---------------------------------------------------------------
//  Меню — работает, если программу запустили без аргументов
// ---------------------------------------------------------------
int run_menu() {
    while (true) {
        std::cout << "\n=== Задание №2 (на «4») ===\n"
                     " 1 - часть 1: максимальный поток (Форд-Фалкерсон)\n"
                     " 2 - часть 2: путь (Дейкстра и A* с эвристикой Чебышева)\n"
                     " 3 - часть 3: двудольность и максимальное паросочетание\n"
                     " 0 - выход\n"
                     "Ваш выбор: ";

        int choice = -1;
        if (!(std::cin >> choice)) {
            break;   // ввод закончился
        }
        if (choice == 0) {
            break;
        }

        std::string file;
        if (choice == 1 || choice == 3) {
            std::cout << "Имя входного файла: ";
            std::cin >> file;
            if (choice == 1) {
                run_part1(file, kDefaultOutput);
            } else {
                run_part3(file, kDefaultOutput);
            }
        } else if (choice == 2) {
            int r1 = 0, c1 = 0, r2 = 0, c2 = 0;
            std::cout << "Имя входного файла: ";
            std::cin >> file;
            std::cout << "Начало пути (строка столбец): ";
            std::cin >> r1 >> c1;
            std::cout << "Конец пути (строка столбец): ";
            std::cin >> r2 >> c2;
            run_part2(file, r1, c1, r2, c2, kDefaultOutput);
        } else {
            std::cout << "Нет такого пункта меню\n";
        }
    }
    return 0;
}

// ---------------------------------------------------------------
//  Печать краткой подсказки по запуску
// ---------------------------------------------------------------
void print_usage() {
    std::cout << "Запуск:\n"
                 "  lr2.exe 1 <файл сети>  [-o <файл результата>]\n"
                 "  lr2.exe 2 <файл карты> -a <строка> <столбец> -b <строка> <столбец> [-o <файл>]\n"
                 "  lr2.exe 3 <файл графа> [-o <файл результата>]\n"
                 "Без аргументов открывается меню.\n";
}

} // namespace

int main(int argc, char** argv) {
#ifdef _WIN32
    // Чтобы русские буквы правильно выводились в консоли Windows.
    SetConsoleOutputCP(65001);
#endif

    // Без аргументов — меню.
    if (argc < 2) {
        return run_menu();
    }

    const int part = std::atoi(argv[1]);
    if (part < 1 || part > 3) {
        print_usage();
        return 1;
    }
    if (argc < 3) {
        print_usage();
        return 1;
    }

    const std::string inputFile = argv[2];
    std::string outputFile = kDefaultOutput;

    // Координаты для части 2: -1 значит "не заданы".
    int r1 = -1, c1 = -1, r2 = -1, c2 = -1;

    for (int i = 3; i < argc; ++i) {
        const std::string key = argv[i];

        if (key == "-o" && i + 1 < argc) {
            outputFile = argv[++i];
        } else if (key == "-a" && i + 2 < argc) {
            r1 = std::atoi(argv[++i]);
            c1 = std::atoi(argv[++i]);
        } else if (key == "-b" && i + 2 < argc) {
            r2 = std::atoi(argv[++i]);
            c2 = std::atoi(argv[++i]);
        } else {
            std::cout << "Ошибка: непонятный параметр " << key << "\n";
            print_usage();
            return 1;
        }
    }

    if (part == 1) {
        return run_part1(inputFile, outputFile);
    }
    if (part == 3) {
        return run_part3(inputFile, outputFile);
    }

    // Часть 2: обе точки обязательны.
    if (r1 < 1 || c1 < 1 || r2 < 1 || c2 < 1) {
        std::cout << "Ошибка: для части 2 нужны ключи -a и -b с координатами\n";
        print_usage();
        return 1;
    }
    return run_part2(inputFile, r1, c1, r2, c2, outputFile);
}
