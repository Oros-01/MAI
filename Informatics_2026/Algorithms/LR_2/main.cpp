#include <windows.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <random>

using namespace std;

// Безопасный ввод целых чисел (защита от букв)
int safeInputInt(const string& prompt) {
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);
        try {
            size_t pos;
            int val = stoi(input, &pos);
            if (pos == input.length()) return val; // Успешно преобразовали
        } catch (...) {}
        cout << "Ошибка! Введите целое число: ";
    }
}

// ================= ГЛОБАЛЬНЫЕ ПЕРЕМЕННЫЕ =================
long long g_operations = 0; // Счетчик операций (сравнений/пересылок) для анализа

// ---------- Сортировка Слиянием (MergeSort) ----------
void merge(vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Временные массивы
    vector<int> L(n1), R(n2);

    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        g_operations++;
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    // Копируем оставшиеся элементы
    while (i < n1) { arr[k] = L[i]; i++; k++; }
    while (j < n2) { arr[k] = R[j]; j++; k++; }
}

void mergeSort(vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

// ---------- Быстрая Сортировка (QuickSort) ----------
// pivotType: 1 - с начала, 2 - с конца, 3 - из середины
int partition(vector<int>& arr, int low, int high, int pivotType) {
    int pivotIndex = low;
    if (pivotType == 2) pivotIndex = high;
    else if (pivotType == 3) pivotIndex = low + (high - low) / 2;

    // Ставим опорный элемент в конец для удобства алгоритма
    swap(arr[pivotIndex], arr[high]);
    int pivot = arr[high];

    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        g_operations++;
        if (arr[j] < pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return (i + 1);
}

void quickSort(vector<int>& arr, int low, int high, int pivotType) {
    if (low < high) {
        // Защита: если элементов слишком много и выбран плохой опорный элемент,
        // переключаемся на сортировку слиянием (чтобы не упасть)
        if (high - low > 50000 && pivotType != 3) {
            mergeSort(arr, low, high); 
            return;
        }
        
        int pi = partition(arr, low, high, pivotType);
        quickSort(arr, low, pi - 1, pivotType);
        quickSort(arr, pi + 1, high, pivotType);
    }
}

// ---------- Вспомогательные функции ----------
void printArr(const vector<int>& a) {
    cout << "[";
    for (size_t i = 0; i < a.size(); ++i)
        cout << a[i] << (i + 1 < a.size() ? ", " : "");
    cout << "]\n";
}

bool isSorted(const vector<int>& a) {
    for (int i = 1; i < (int)a.size(); ++i)
        if (a[i - 1] > a[i]) return false;
    return true;
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    vector<int> arr;
    string cmd;
    int pivotType = 3; // По умолчанию - из середины

    while (true) {
        cout << "\n=== Лаба №2: Сортировки ===\n"
             << "1 - ввод (консоль)\n"
             << "2 - ввод (файл)\n"
             << "3 - сгенерировать случайный массив\n"
             << "4 - показать массив\n"
             << "5 - Сортировка Слиянием (MergeSort)\n"
             << "6 - Быстрая Сортировка (QuickSort)\n"
             << "7 - Выбрать опорный элемент для QuickSort (текущий: ";
        if (pivotType == 1) cout << "с начала";
        else if (pivotType == 2) cout << "с конца";
        else cout << "из середины";
        cout << ")\n"
             << "exit - выход\n> ";

        if (!getline(cin, cmd)) break;
        if (cmd == "exit") break;

        if (cmd == "1") {
            int n = safeInputInt("N = ");
            if (n <= 0 || n > 1000000) { 
                cout << "Ошибка: размер от 1 до 1 000 000.\n"; 
                continue; 
            }
            arr.resize(n);
            for (int i = 0; i < n; ++i) {
                arr[i] = safeInputInt("Элемент [" + to_string(i) + "]: ");
            }
        }
        else if (cmd == "2") {
            string path; cout << "Файл: "; getline(cin, path);
            ifstream f(path);
            int n;
            if (!(f >> n)) { cout << "Ошибка файла\n"; continue; }
            vector<int> tmp(n);
            bool ok = true;
            for (int i = 0; i < n; ++i) if (!(f >> tmp[i])) { ok = false; break; }
            if (ok) { arr = move(tmp); cout << "Загружено " << n << " элементов\n"; }
            else cout << "Ошибка: мало элементов в файле\n";
        }
        else if (cmd == "3") {
            int n = safeInputInt("N (размер): ");
            if (n <= 0 || n > 1000000) { cout << "Ошибка размера (1-1000000)\n"; continue; }
            
            int low = safeInputInt("Диапазон от: ");
            int high = safeInputInt("Диапазон до: ");
            if (low > high) swap(low, high);
            
            random_device rd; //сид
            mt19937 generator(rd()); //генератор случайного числа на основе сида rd
            uniform_int_distribution<int> dist(low, high); //берём случайные числа из generator и обрезаем чтобы они были от low до high
            arr.resize(n);
            for (int i = 0; i < arr.size(); i++) {
                arr[i] = dist(generator);
            }
            cout << "Сгенерировано " << n << " элементов\n";
        }
        else if (cmd == "4") {
            if (arr.empty()) cout << "(пусто)\n";
            else printArr(arr);
        }
        else if (cmd == "5") {
            if (arr.empty()) { cout << "Массив пуст\n"; continue; }
            g_operations = 0;
            auto t0 = chrono::high_resolution_clock::now();
            mergeSort(arr, 0, arr.size() - 1);
            auto t1 = chrono::high_resolution_clock::now();
            long long ns = chrono::duration_cast<chrono::nanoseconds>(t1 - t0).count();
            cout << "MergeSort готов за " << ns << " нс. Сравнений: " << g_operations << "\n";
            cout << "Отсортирован: " << (isSorted(arr) ? "Да" : "Нет") << "\n";
        }
        else if (cmd == "6") {
            if (arr.empty()) { cout << "Массив пуст\n"; continue; }
            g_operations = 0;
            auto t0 = chrono::high_resolution_clock::now();
            quickSort(arr, 0, arr.size() - 1, pivotType);
            auto t1 = chrono::high_resolution_clock::now();
            long long ns = chrono::duration_cast<chrono::nanoseconds>(t1 - t0).count();
            cout << "QuickSort готов за " << ns << " нс. Сравнений: " << g_operations << "\n";
            cout << "Отсортирован: " << (isSorted(arr) ? "Да" : "Нет") << "\n";
        }
        else if (cmd == "7") {
            cout << "Выберите опорный элемент:\n1 - с начала\n2 - с конца\n3 - из середины\n> ";
            string s; getline(cin, s);
            if (s == "1" || s == "2" || s == "3") {
                pivotType = stoi(s);
                cout << "Выбрано: ";
                if (pivotType == 1) cout << "с начала\n";
                else if (pivotType == 2) cout << "с конца\n";
                else cout << "из середины\n";
            } else cout << "Неверный ввод\n";
        }
    }
    return 0;
}