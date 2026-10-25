// Консольная программа для работы с графом.
//
// ШАГ 1 (сделан сейчас):
//   - создать пустой граф
//   - считать граф из файла
//   - вывести список вершин и список рёбер
//   - вывести рёбра, связанные с вершиной
//   - проверить, есть ли ребро, и узнать его вес
//
// Следующие шаги: удаление/добавление вершин и рёбер, связность,
// Беллман-Форд, остовное дерево (Прим).
#pragma once
#include <iostream>
#include <string>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#endif

#include "graph.h" //весь наш граф умещён в этой строчке и для компилятора здесь не 1 строчка, а  93!

using namespace std;


// Функции вывода
void print_vertices(const Graph& g) {
    vector<int> v = g.vertices();

    if (v.empty()) {
        cout << "В графе нет вершин.\n";
        return;
    }

    cout << "Всего вершин: " << v.size() << "\n";
    cout << "Вершины: ";
    for (size_t i = 0; i < v.size(); i++) {
        cout << v[i] << " ";
    }
    cout << "\n";
}

void print_edges(const Graph& g) {
    vector<Edge> e = g.list_of_edges();

    if (e.empty()) {
        cout << "В графе нет рёбер.\n";
        return;
    }

    cout << "Всего " << (g.isDirected() ? "дуг: " : "рёбер: ") << e.size() << "\n";
    for (size_t i = 0; i < e.size(); i++) {
        cout << "  " << e[i].from;
        cout << (g.isDirected() ? " -> " : " -- ");
        cout << e[i].to << "   вес = " << e[i].weight << "\n";
    }
}

void print_edges_of_vertex(const Graph& g, int v) {
    if (!g.has_vertex(v)) {
        cout << "Вершины " << v << " в графе нет.\n";
        return;
    }

    vector<Edge> e = g.list_of_edges(v);

    if (e.empty()) {
        cout << "У вершины " << v << " нет рёбер.\n";
        return;
    }

    if (g.isDirected()) {
        cout << "Дуги, выходящие из вершины " << v << ":\n";
    } else {
        cout << "Рёбра, связанные с вершиной " << v << ":\n";
    }

    for (size_t i = 0; i < e.size(); i++) {
        cout << "  " << e[i].from;
        cout << (g.isDirected() ? " -> " : " -- ");
        cout << e[i].to << "   вес = " << e[i].weight << "\n";
    }
}

// ---------------------------------------------------------------
// Чтение чисел с клавиатуры
// ---------------------------------------------------------------

// читает целое число. Если введено не число, возвращает false.
bool read_int(int& value) {
    if (!(cin >> value)) {
        if (cin.eof()) {
            return false;
        }
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Это не число, попробуйте ещё раз.\n";
        return false;
    }
    return true;
}

// спросим у пользователя, ориентированный граф или нет.
bool ask_directed() {
    int answer;
    cout << "Граф ориентированный? (1 - да, 0 - нет): ";
    while (!read_int(answer)) {
        cout << "Граф ориентированный? (1 - да, 0 - нет): ";
    }
    return answer == 1;
}

//мейн
// ---------------------------------------------------------------

int main() {
#ifdef _WIN32
    // Чтобы русские буквы правильно выводились в консоли Windows.
    SetConsoleOutputCP(65001);
#endif

    Graph g(false);   // пока пустой неориентированный граф

    cout << "Программа для работы с графом.\n";

    while (true) { //фишичный бесконечный цикл
        cout << "\n================= МЕНЮ =================\n";
        cout << " 1 - создать пустой граф\n";
        cout << " 2 - считать граф из файла\n";
        cout << " 3 - вывести список всех вершин\n";
        cout << " 4 - вывести список всех рёбер\n";
        cout << " 5 - вывести рёбра, связанные с вершиной\n";
        cout << " 6 - проверить наличие ребра и узнать его вес\n";
        cout << " 7 - удалить вершину\n";
        cout << " 8 - удалить ребро\n";
        cout << " 9 - проверить связность и вывести компоненты\n";
        cout << "10 - добавить вершину\n";
        cout << "11 - добавить ребро\n";
        cout << "12 - кратчайший путь между двумя вершинами (Беллман-Форд)\n";
        cout << "13 - расстояния от вершины до всех (Беллман-Форд)\n";
        cout << "14 - минимальное остовное дерево (Прим)\n";
        cout << " 0 - выход\n";
        cout << "Ваш выбор: ";

        int choice; //ветка выборов
        if (!read_int(choice)) {
            if (cin.eof()) {
                break;      // ввод закончился (например, файл был перенаправлен)
            }
            continue;
        }

        if (choice == 0) {
            cout << "Работа завершена.\n";
            break;
        }

        if (choice == 1) {
            bool directed = ask_directed();
            g = Graph(directed);
            cout << "Создан пустой " << (directed ? "ориентированный" : "неориентированный")
                 << " граф.\n";
        }
        else if (choice == 2) {
            string fileName;
            cout << "Имя файла (например data/graph1.txt): ";
            cin >> fileName;

            bool directed = ask_directed();

            // Читаем граф во временный объект, чтобы при ошибке
            // не потерять тот граф, который уже был в программе.
            Graph newGraph(directed);
            if (newGraph.loadFromFile(fileName)) {
                g = newGraph;
                cout << "Граф прочитан. Вершин: " << g.size() << ", "
                     << (g.isDirected() ? "дуг: " : "рёбер: ")
                     << g.list_of_edges().size() << "\n";
            } else {
                cout << "Не удалось прочитать граф из файла \"" << fileName
                     << "\". Старый граф не изменился.\n";
            }
        }
        else if (choice == 3) {
            print_vertices(g);
        }
        else if (choice == 4) {
            print_edges(g);
        }
        else if (choice == 5) {
            int v;
            cout << "Номер вершины: ";
            if (read_int(v)) {
                print_edges_of_vertex(g, v);
            }
        }
        else if (choice == 6) {
            int a, b;
            cout << "Номер первой вершины: ";
            if (!read_int(a)) {
                continue;
            }
            cout << "Номер второй вершины: ";
            if (!read_int(b)) {
                continue;
            }

            if (g.is_edge(a, b)) {
                cout << "Ребро " << a << " - " << b << " есть, его вес = "
                     << g.weight(a, b) << "\n";
            } else {
                cout << "Ребра " << a << " - " << b << " в графе нет.\n";
            }
        }
         else if (choice == 7) {
            int v;
            cout << "Номер вершины для удаления: ";
            if (!read_int(v)) {
                continue;               // ввели не число — возвращаемся в меню
            }
            if (g.remove_vertex(v)) {
                cout << "Вершина " << v << " удалена вместе со всеми её рёбрами.\n";
            } else {
                cout << "Вершины " << v << " в графе нет.\n";
            }
        }
         else if (choice == 8) {
            int a, b;
            cout << "Номер первой вершины: ";
            if (!read_int(a)) {
                continue;
            }
            cout << "Номер второй вершины: ";
            if (!read_int(b)) {
                continue;
            }
            if (g.remove_edge(a, b)) {
                cout << "Ребро " << a << " - " << b << " удалено.\n";
            } else {
                cout << "Ребра " << a << " - " << b << " в графе нет.\n";
            }
        }


         else if (choice == 9) {
            std::vector<std::vector<int> > comps = g.connected_components();

            if (comps.empty()) {
                cout << "В графе нет вершин.\n";
            } else if (comps.size() == 1) {
                cout << "Граф связен (одна компонента).\n";
            } else {
                cout << "Граф НЕ связен. Компонент: " << comps.size() << "\n";
            }

            for (size_t i = 0; i < comps.size(); i++) {
                cout << "  Компонента " << (i + 1) << ":";
                for (size_t j = 0; j < comps[i].size(); j++) {
                    cout << " " << comps[i][j];
                }
                cout << "\n";
            }
        }

                else if (choice == 10) {
            int v;
            cout << "Номер новой вершины: ";
            if (!read_int(v)) { continue; }
            if (g.add_vertex(v)) {
                cout << "Вершина " << v << " добавлена.\n";
            } else {
                cout << "Вершина " << v << " уже есть или введен неверный номер.\n";
            }
        }

        else if (choice == 11) {
            int a, b;
            long long w;
            cout << "Номер первой вершины: ";
            if (!read_int(a)) { continue; }
            cout << "Номер второй вершины: ";
            if (!read_int(b)) { continue; }
            cout << "Вес ребра: ";
            if (!(cin >> w)) {
                cin.clear(); cin.ignore(10000, '\n');
                cout << "Это не число.\n";
                continue;
            }
            if (g.add_edge(a, b, w)) {
                cout << "Ребро " << a << " - " << b << " (вес " << w << ") добавлено.\n";
            } else {
                cout << "Не удалось добавить ребро (вершины не существуют или это петля).\n";
            }
        }

        else if (choice == 12) {
            int a, b;
            cout << "От вершины: ";
            if (!read_int(a)) { continue; }
            cout << "До вершины: ";
            if (!read_int(b)) { continue; }

            std::vector<long long> dist;
            std::vector<int> prev;
            if (!g.bellman_ford(a, dist, prev)) {
                cout << "В графе есть цикл отрицательного веса, кратчайшие пути не определены.\n";
                continue;
            }
            if (dist[b] >= 1000000000000000000LL / 2) { // бесконечность
                cout << "Пути из " << a << " в " << b << " нет.\n";
            } else {
                cout << "Длина пути: " << dist[b] << "\n";
                std::vector<int> path = g.restore_path(a, b, prev);
                cout << "Путь: ";
                for (size_t i = 0; i < path.size(); i++) {
                    if (i > 0) cout << " -> ";
                    cout << path[i];
                }
                cout << "\n";
            }
        }

        else if (choice == 13) {
            int a;
            cout << "От вершины: ";
            if (!read_int(a)) { continue; }

            std::vector<long long> dist;
            std::vector<int> prev;
            if (!g.bellman_ford(a, dist, prev)) {
                cout << "В графе есть цикл отрицательного веса, расстояния не определены.\n";
                continue;
            }
            std::vector<int> verts = g.vertices();
            for (size_t i = 0; i < verts.size(); i++) {
                int v = verts[i];
                cout << "  " << a << " -> " << v << " : ";
                if (dist[v] >= 1000000000000000000LL / 2) {
                    cout << "недостижима\n";
                } else {
                    cout << dist[v] << "\n";
                }
            }
        }

        else if (choice == 14) {
            std::vector<Edge> mst = g.prim();
            if (mst.empty()) {
                cout << "Остовное дерево пусто (в графе нет вершин или нет рёбер).\n";
                continue;
            }
            long long total = 0;
            cout << "Минимальное остовное дерево:\n";
            for (size_t i = 0; i < mst.size(); i++) {
                cout << "  " << mst[i].from << " - " << mst[i].to
                     << " (вес " << mst[i].weight << ")\n";
                total += mst[i].weight;
            }
            cout << "Суммарный вес: " << total << "\n";
        }

        else {
            cout << "Выберите корректный пнукт.\n";
        }
    }

    return 0;
}
