#include <windows.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>

using namespace std;

// ---------- Линейный поиск: O(n) ----------
int linearSearch(const vector<int>& a, int key) {
    for (int i = 0; i < (int)a.size(); ++i)
        if (a[i] == key) return i;
    return -1;
}

// ---------- Бинарный поиск: O(log n), требует сортировки ----------
int binarySearch(const vector<int>& a, int key) {
    int l = 0, r = (int)a.size() - 1;
    while (l <= r) {
        int m = l + (r - l) / 2;
        if (a[m] == key) return m;
        if (a[m] < key) l = m + 1;
        else            r = m - 1;
    }
    return -1;
}

// ---------- Сортировка вставками: O(n^2) ----------
void insertionSort(vector<int>& a) {
    for (int i = 1; i < (int)a.size(); ++i) {
        int key = a[i], j = i - 1;
        while (j >= 0 && a[j] > key) { a[j + 1] = a[j]; --j; }
        a[j + 1] = key;
    }
}

// ---------- Сортировка выбором: O(n^2) ----------
void selectionSort(vector<int>& a) {
    int n = (int)a.size();
    for (int i = 0; i < n - 1; ++i) {
        int mn = i;
        for (int j = i + 1; j < n; ++j)
            if (a[j] < a[mn]) mn = j;
        if (mn != i) swap(a[i], a[mn]);
    }
}

// ---------- Проверка отсортированности ----------
bool isSorted(const vector<int>& a) {
    for (int i = 1; i < (int)a.size(); ++i)
        if (a[i - 1] > a[i]) return false;
    return true;
}

void printArr(const vector<int>& a) {
    cout << "[";
    for (size_t i = 0; i < a.size(); ++i)
        cout << a[i] << (i + 1 < a.size() ? ", " : "");
    cout << "]\n";
}

int main() {
    SetConsoleOutputCP(CP_UTF8); 
    vector<int> arr;
    string algo = "linear";
    string cmd;

    while (true) {
        cout << "\n=== Лаба №1: Поиск ===\n"
             << "1 - ввод (консоль)\n"
             << "2 - ввод (файл)\n"
             << "3 - алгоритм поиска (linear/binary)\n"
             << "4 - сортировка (insertion/selection)\n"
             << "5 - поиск ключа\n"
             << "6 - показать массив\n"
             << "exit - выход\n> ";
        if (!getline(cin, cmd)) break;
        if (cmd == "exit") break;

        if (cmd == "1") {
            int n; cout << "N = "; cin >> n;
            arr.resize(n);
            cout << "Элементы: ";
            for (int& x : arr) cin >> x;
            cin.ignore();
        }
        else if (cmd == "2") {
            string path; cout << "Файл: "; getline(cin, path);
            ifstream f(path);
            int n;
            if (!(f >> n)) { cout << "Ошибка файла\n"; continue; }
            arr.resize(n);
            for (int& x : arr) f >> x;
            cout << "Загружено " << n << " элементов\n";
        }
        else if (cmd == "3") {
            cout << "linear / binary: "; getline(cin, algo);
            if (algo != "linear" && algo != "binary") algo = "linear";
        }
        else if (cmd == "4") {
            string s; cout << "insertion / selection: "; getline(cin, s);
            auto t0 = chrono::high_resolution_clock::now();
            if (s == "insertion") insertionSort(arr);
            else if (s == "selection") selectionSort(arr);
            else { cout << "нет такой\n"; continue; }
            auto t1 = chrono::high_resolution_clock::now();
            cout << "Готово за "
                 << chrono::duration_cast<chrono::microseconds>(t1 - t0).count()
                 << " мкс\n";
        }
        else if (cmd == "5") {
            if (arr.empty()) { cout << "Массив пуст\n"; continue; }
            int key; cout << "Ключ: "; cin >> key; cin.ignore();

            if (algo == "binary" && !isSorted(arr)) {
                cout << "ОШИБКА: массив не отсортирован. "
                     << "Отсортировать? (insertion/selection/no): ";
                string s; getline(cin, s);
                if (s == "insertion") insertionSort(arr);
                else if (s == "selection") selectionSort(arr);
                else { cout << "Поиск отменён\n"; continue; }
            }

            auto t0 = chrono::high_resolution_clock::now();
            int idx = (algo == "binary") ? binarySearch(arr, key)
                                         : linearSearch(arr, key);
            auto t1 = chrono::high_resolution_clock::now();

            cout << (idx == -1 ? "Не найдено" : "Найдено, индекс = " + to_string(idx))
                 << " (" << chrono::duration_cast<chrono::nanoseconds>(t1 - t0).count()
                 << " нс)\n";
        }
        else if (cmd == "6") printArr(arr);
    }
    cout << "Выход.\n";
}