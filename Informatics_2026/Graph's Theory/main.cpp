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

// Читает целое число. Если введено не число, возвращает false.
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

// Спрашивает у пользователя, ориентированный граф или нет.
bool ask_directed() {
    int answer;
    cout << "Граф ориентированный? (1 - да, 0 - нет): ";
    while (!read_int(answer)) {
        cout << "Граф ориентированный? (1 - да, 0 - нет): ";
    }
    return answer == 1;
}

// ---------------------------------------------------------------
// Главная функция
// ---------------------------------------------------------------

int main() {
#ifdef _WIN32
    // Чтобы русские буквы правильно выводились в консоли Windows.
    SetConsoleOutputCP(65001);
#endif

    Graph g(false);   // пока пустой неориентированный граф

    cout << "Программа для работы с графом.\n";

    while (true) {
        cout << "\n================= МЕНЮ =================\n";
        cout << " 1 - создать пустой граф\n";
        cout << " 2 - считать граф из файла\n";
        cout << " 3 - вывести список всех вершин\n";
        cout << " 4 - вывести список всех рёбер\n";
        cout << " 5 - вывести рёбра, связанные с вершиной\n";
        cout << " 6 - проверить наличие ребра и узнать его вес\n";
        cout << " 0 - выход\n";
        cout << "Ваш выбор: ";

        int choice;
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
        } else if (choice == 2) {
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
        } else if (choice == 3) {
            print_vertices(g);
        } else if (choice == 4) {
            print_edges(g);
        } else if (choice == 5) {
            int v;
            cout << "Номер вершины: ";
            if (read_int(v)) {
                print_edges_of_vertex(g, v);
            }
        } else if (choice == 6) {
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
        } else {
            cout << "Такого пункта меню нет.\n";
        }
    }

    return 0;
}
