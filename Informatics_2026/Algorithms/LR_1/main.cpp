#include <windows.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <random>
#include <iomanip>

using namespace std;

// ================= ГЛОБАЛЬНЫЕ СЧЁТЧИКИ СРАВНЕНИЙ =================
long long g_comparisons = 0;

// ---------- Линейный поиск: O(n) ----------
//пробегаем весь массив сравнивая его с ключом
int linearSearch(const vector<int>& arr, int key) {
    g_comparisons = 0;
    for (int i = 0; i < (int)arr.size(); ++i) {
        ++g_comparisons;
        if (arr[i] == key) return i;
    }
    return -1;//если ключ не найден возвращаем -1
}
//O(1) - ключ первый
//O(n) - клю

// ---------- Бинарный поиск: O(log n), требует сортировки ----------
int binarySearch(const vector<int>& arr, int key) {
    g_comparisons = 0;
    int left = 0, right = (int)arr.size() - 1;
    while (left <= right) {
        int m = left + (right - left) / 2;
        ++g_comparisons;
        if (arr[m] == key) return m;
        if (arr[m] < key) left = m + 1; //сдвигаем левую границу массива до середины, можно сказать разбивая массив на 2 части и смотря только правую часть, тк key в правой части
        else              right = m - 1; //наоборот рассматриваем только левую(если key меньше arr[m])
    }
    return -1;
}

// ---------- Сортировка вставками: O(n^2) ----------
void insertionSort(vector<int>& arr) {
    for (int i = 1; i < (int)arr.size(); ++i) {
        int key = arr[i], j = i - 1;
        while (j >= 0 && arr[j] > key) { arr[j + 1] = arr[j]; --j; }
        arr[j + 1] = key;
    }
}

// ---------- Сортировка выбором: O(n^2) ----------
void selectionSort(vector<int>& arr) {
    int n = (int)arr.size();
    for (int i = 0; i < n - 1; ++i) {
        int mn = i;
        for (int j = i + 1; j < n; ++j)
            if (arr[j] < arr[mn]) mn = j;
        if (mn != i) swap(arr[i], arr[mn]);
    }
}

// ---------- Проверка отсортированности ----------
bool isSorted(const vector<int>& arr) {
    for (int i = 1; i < (int)arr.size(); ++i)
        if (arr[i - 1] > arr[i]) return false;
    return true;
}

void printArr(const vector<int>& arr) {
    cout << "[";
    for (size_t i = 0; i < arr.size(); ++i)
        cout << arr[i] << (i + 1 < arr.size() ? ", " : "");
    cout << "]\n";
}

// ---------- Сохранение результата в файл ----------
void saveResultToFile(const string& path, const vector<int>& arr,
                      const string& algo, int key, int idx,
                      long long comparisons, long long timeNs) {
    ofstream f(path);
    if (!f) { cout << "Не удалось открыть файл для записи\n"; return; }
    f << "=== Результат лабораторной работы №1 ===\n";
    f << "Размер массива: " << arr.size() << "\n";
    f << "Алгоритм поиска: " << algo << "\n";
    f << "Ключ: " << key << "\n";
    f << (idx == -1 ? "Результат: НЕ найдено\n"
                    : "Результат: найдено, индекс = " + to_string(idx) + "\n");
    f << "Количество сравнений: " << comparisons << "\n";
    f << "Время выполнения: " << timeNs << " нс\n";
    f << "Массив отсортирован: " << (isSorted(arr) ? "да" : "нет") << "\n";
    f.close();
    cout << "Результат сохранён в " << path << "\n";
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    vector<int> arr;
    string algo = "linear";
    string cmd;

    // для сохранения последнего результата поиска
    int lastKey = 0, lastIdx = -2;   // -2 = поиска ещё не было
    long long lastComparisons = 0, lastTimeNs = 0;

    while (true) {
        cout << "\n=== Лаба №1: Поиск ===\n"
             << "1 - ввод (консоль)\n"
             << "2 - ввод (файл)\n"
             << "3 - алгоритм поиска (linear/binary)\n"
             << "4 - сортировка (insertion/selection)\n"
             << "5 - поиск ключа\n"
             << "6 - показать массив\n"
             << "7 - сгенерировать случайный массив\n"
             << "8 - сохранить результат в файл\n"
             << "exit - выход\n> ";
        if (!getline(cin, cmd)) break;
        if (cmd == "exit") break;

        if (cmd == "1") {
            int n;
            cout << "N = ";
            if (!(cin >> n) || n < 0) {
                cin.clear(); cin.ignore(10000, '\n');
                cout << "Ошибка ввода\n"; continue;
            }
            arr.resize(n);
            cout << "Элементы: ";
            for (int& x : arr) {
                if (!(cin >> x)) {
                    cin.clear(); cin.ignore(10000, '\n');
                    cout << "Ошибка ввода элемента\n"; break;
                }
            }
            cin.ignore();
        }
        else if (cmd == "2") {
            string path;
            cout << "Файл: ";
            getline(cin, path);
            ifstream f(path);
            int n;
            if (!(f >> n) || n < 0) {
                cout << "Ошибка файла (не удалось прочитать N)\n"; continue;
            }
            vector<int> tmp(n);
            bool ok = true;
            for (int i = 0; i < n; ++i) {
                if (!(f >> tmp[i])) { ok = false; break; }
            }
            if (!ok) {
                cout << "Ошибка: в файле меньше " << n << " элементов\n";
                continue;
            }
            arr = move(tmp);
            cout << "Загружено " << n << " элементов\n";
        }
        else if (cmd == "3") {
            cout << "linear / binary: ";
            getline(cin, algo);
            if (algo != "linear" && algo != "binary") algo = "linear";
            cout << "Выбран: " << algo << "\n";
        }
        else if (cmd == "4") {
            if (arr.empty()) { cout << "Массив пуст\n"; continue; }
            string s;
            cout << "insertion / selection: ";
            getline(cin, s);
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
            int key;
            cout << "Ключ: ";
            if (!(cin >> key)) {
                cin.clear(); cin.ignore(10000, '\n');
                cout << "Ошибка ввода ключа\n"; continue;
            }
            cin.ignore();

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
            long long ns = chrono::duration_cast<chrono::nanoseconds>(t1 - t0).count();

            // запоминаем для сохранения
            lastKey = key;
            lastIdx = idx;
            lastComparisons = g_comparisons;
            lastTimeNs = ns;

            cout << (idx == -1 ? "Не найдено" : "Найдено, индекс = " + to_string(idx))
                 << " (" << ns << " нс)\n";
            cout << "Сравнений: " << g_comparisons << "\n";
        }
        else if (cmd == "6") {
            if (arr.empty()) cout << "(пусто)\n";
            else printArr(arr);
        }
        else if (cmd == "7") {
            int n;
            cout << "N (размер): ";
            if (!(cin >> n) || n < 0) {
                cin.clear(); cin.ignore(10000, '\n');
                cout << "Ошибка\n"; continue;
            }
            cin.ignore();
            int lo, hi;
            cout << "Диапазон [lo hi]: ";
            if (!(cin >> lo >> hi)) {
                cin.clear(); cin.ignore(10000, '\n');
                cout << "Ошибка\n"; continue;
            }
            cin.ignore();
            if (lo > hi) swap(lo, hi);
            random_device rd;
            mt19937 gen(rd());
            uniform_int_distribution<int> dist(lo, hi);
            arr.resize(n);
            for (int& x : arr) x = dist(gen);
            cout << "Сгенерировано " << n << " элементов в диапазоне ["
                 << lo << ".." << hi << "]\n";
        }
        else if (cmd == "8") {
            if (lastIdx == -2) {
                cout << "Сначала выполните поиск (пункт 5)\n"; continue;
            }
            string path;
            cout << "Файл для записи: ";
            getline(cin, path);
            saveResultToFile(path, arr, algo, lastKey, lastIdx,
                             lastComparisons, lastTimeNs);
        }
        else {
            cout << "Неизвестная команда\n";
        }
    }
    cout << "Выход.\n";
    return 0;
}

