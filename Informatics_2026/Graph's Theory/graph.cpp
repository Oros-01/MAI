#include "graph.h" //объявили класс, поля и тд.

#include <fstream> //для работы с файлами(filestream)
#include <iostream>
#include <sstream>

// ---------------------------------------------------------------
// Конструкторы и служебные методы

Graph::Graph(bool directed) {
    directed_ = directed;
}

Graph::Graph(const std::string& fileName, bool directed) {//принимает путь к файлу
    directed_ = directed; //ориентированный/неориентированный
    loadFromFile(fileName);
}

void Graph::clear() { //в graph.h подтягивается vector, а в самом vector уже есть метод clear, вот такая хитрость)
    is_edge_.clear();
    weight_.clear();
    exists_.clear();
}

bool Graph::isDirected() const {
    return directed_; //поле класса graph(ориентированный ли true/false)
}

// если вершины с номером v ещё нет в матрице, увеличиваем матрицы.
void Graph::ensure_size(int v) {
    if (v < 0) {
        return;
    }
    int old_size = (int)exists_.size();
    if (v < old_size) {
        return;
    }

    int new_size = v + 1;

    is_edge_.resize(new_size);
    weight_.resize(new_size);
    exists_.resize(new_size, false);   // новые вершины пока не существуют

    for (int i = 0; i < new_size; i++) {
        is_edge_[i].resize(new_size, false);
        weight_[i].resize(new_size, 0);
    }
}

bool Graph::has_vertex(int v) const {
    if (v < 0 || v >= (int)exists_.size()) {
        return false;
    }
    return exists_[v];
}

// ---------------------------------------------------------------
// методы, из задания

int Graph::size() const {
    int count = 0;
    for (int i = 0; i < (int)exists_.size(); i++) {
        if (exists_[i]) {
            count++;
        }
    }
    return count;
}

long long Graph::weight(int a, int b) const {
    if (!is_edge(a, b)) {
        return 0;
    }
    return weight_[a][b];
}

bool Graph::is_edge(int a, int b) const {
    if (a < 0 || b < 0) {
        return false;
    }
    if (a >= (int)is_edge_.size() || b >= (int)is_edge_.size()) {
        return false;
    }
    return is_edge_[a][b];
}

bool Graph::add_vertex(int v) {
    if (v < 0) {
        return false;
    }
    ensure_size(v);
    if (exists_[v]) {
        return false;   // такая вершина уже есть
    }
    exists_[v] = true;
    return true;
}

bool Graph::add_edge(int a, int b, long long w) {
    if (a == b) {
        return false;   // петли не поддерживаем
    }
    if (!has_vertex(a) || !has_vertex(b)) {
        return false;
    }

    is_edge_[a][b] = true;
    weight_[a][b] = w;

    // в неориентированном графе ребро идёт в обе стороны,
    // поэтому матрица смежности получается симметричной.
    if (!directed_) {
        is_edge_[b][a] = true;
        weight_[b][a] = w;
    }
    return true;
}

bool Graph::remove_vertex(int v) {
    if (!has_vertex(v)) {
        return false;
    }
    exists_[v] = false;

    // стираем все рёбра, которые были связаны с этой вершиной:
    // её строку и её столбец.
    for (int i = 0; i < (int)exists_.size(); i++) {
        is_edge_[v][i] = false;
        weight_[v][i] = 0;
        is_edge_[i][v] = false;
        weight_[i][v] = 0;
    }
    return true;
}

bool Graph::remove_edge(int a, int b) {
    if (!is_edge(a, b)) {
        return false;
    }

    is_edge_[a][b] = false;
    weight_[a][b] = 0;

    if (!directed_) {
        is_edge_[b][a] = false;
        weight_[b][a] = 0;
    }
    return true;
}

std::vector<int> Graph::vertices() const {
    std::vector<int> result;
    for (int i = 0; i < (int)exists_.size(); i++) {
        if (exists_[i]) {
            result.push_back(i);
        }
    }
    return result;
}

std::vector<Edge> Graph::list_of_edges() const {
    std::vector<Edge> result;

    for (int i = 0; i < (int)exists_.size(); i++) {
        if (!exists_[i]) {
            continue;
        }
        for (int j = 0; j < (int)exists_.size(); j++) {
            if (!exists_[j]) {
                continue;
            }
            if (!is_edge_[i][j]) {
                continue;
            }
            // в неориентированном графе матрица симметрична,
            // поэтому ребро (i, j) и ребро (j, i) - это одно и то же ребро.
            // чтобы не вывести его дважды, берём только j > i.
            if (!directed_ && j < i) {
                continue;
            }

            Edge e;
            e.from = i;
            e.to = j;
            e.weight = weight_[i][j];
            result.push_back(e);
        }
    }
    return result;
}

std::vector<Edge> Graph::list_of_edges(int v) const {
    std::vector<Edge> result;

    if (!has_vertex(v)) {
        return result;
    }

    for (int j = 0; j < (int)exists_.size(); j++) {
        if (!exists_[j]) {
            continue;
        }
        if (!is_edge_[v][j]) {
            continue;
        }

        Edge e;
        e.from = v;
        e.to = j;
        e.weight = weight_[v][j];
        result.push_back(e);
    }
    return result;
}


// ---------------------------------------------------------------
// чтение графа из файла со списками смежности


// Формат файла txt
//   строки с '#' и пустые строки игнорируются, так что комменты можно писать и там
bool Graph::loadFromFile(const std::string& fileName) {
    std::ifstream fin(fileName.c_str());
    if (!fin.is_open()) {
        return false;
    }

    clear();

    std::string line;
    int n = 0;
    bool n_is_read = false;

    // Ищем первую "настоящую" строку - в ней записано число вершин.
    while (std::getline(fin, line)) {
        std::istringstream iss(line);
        if (iss >> n) {
            n_is_read = true;
            break;
        }
    }
    if (!n_is_read || n <= 0) {
        return false;
    }

    // Создаём вершины с номерами 1..n.
    for (int v = 1; v <= n; v++) {
        add_vertex(v);
    }

    // Читаем списки смежности.
    while (std::getline(fin, line)) {
        std::istringstream iss(line);
        int v;
        if (!(iss >> v)) {
            continue;   // пустая строка или комментарий
        }

        int u;
        long long w;
        while (iss >> u >> w) {
            if (!add_edge(v, u, w)) {
                std::cout << "Предупреждение: ребро " << v << "-" << u
                          << " пропущено (неверные номера вершин)\n";
            }
        }
    }
    return true;
}
