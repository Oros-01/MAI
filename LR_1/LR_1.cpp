#include <iostream>
#include <fstream>
//ofstream для вывода в файл
//ifstream для вывода в файл
#include <cstring>
#include <chrono>
#include <cstdlib> 
using namespace std;
//auto start = chrono::high_resolution_clock::now(); //auto - автоматический тип данных
            //подпространство имён(std пронстранство, chrono вложенное пространство)
                    //high_resolution_clock - класс(пользовательский тип данных)
                            //now - функция берущая время в данный момент
/*void classicBubble(int arr[], int size){
    for(int i = 1; i < size; i++){
        for(int j = 0; j < size; j++){
            if(arr[i]<arr[j]){
                swap(arr[i], arr[j]);
            }
        }
    }
}*/

enum DataType {
    RANDOM   = 0,
    SORTED   = 1,
    REVERSED = 2
};

void generateArray(int arr[], int size, DataType type) {
    if (type == RANDOM) {
        for (int i = 0; i < size; i++) {
            arr[i] = rand() % 64001 - 32000;
        }
    }
    else if (type == SORTED) {
        for (int i = 0; i < size; i++) {
            arr[i] = i;
        }
    }
    else if (type == REVERSED) {
        for (int i = 0; i < size; i++) {
            arr[i] = size - 1 - i;
        }
    }
}

void classicBubble(int arr[], int size, long long &comps, long long &swaps) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - 1; j++) {
            comps++; //на каждый j++ + одно сравнение
            if (arr[j] > arr[j+1]) {
                swap(arr[j], arr[j+1]);
                swaps++;
            }
        }
    }
}

void flagBubble(int arr[], int size, long long &comps, long long &swaps){
    for(int i = 0; i < size - 1; i++){
        bool swapped = false;
        for(int j = 0; j < size - 1 - i; j++){
            comps++;
            if(arr[j] > arr[j+1]){
                swap(arr[j], arr[j+1]);
                swaps++;
                swapped = true;
            }
        }
        if(!swapped) break;
    }
}

void boundaryBubble(int arr[], int size, long long &comps, long long &swaps){
    int boundary = size - 1;  // граница — до куда идём во внутреннем цикле
    while(boundary > 0){
        int lastSwap = 0;     // индекс последнего обмена в этом проходе
        for(int j = 0; j < boundary; j++){
            comps++;
            if(arr[j] > arr[j+1]){
                swap(arr[j], arr[j+1]);
                swaps++;
                lastSwap = j; // запоминаем, где был последний обмен
            }
        }
        boundary = lastSwap;  // следующий проход — только до этого места
        // если обменов не было, lastSwap = 0, boundary = 0, цикл завершится
    }
}

// возвращает строку-имя для типа данных
const char* typeName(DataType type) {
    if (type == RANDOM)   return "random";
    if (type == SORTED)   return "sorted";
    if (type == REVERSED) return "reversed";
    return "unknown";
}


int main() {
    auto seed = chrono::high_resolution_clock::now().time_since_epoch().count();
    srand(seed);

    int sizes[] = {10, 100, 500, 1000, 2000, 5000, 10000};
    int numSizes = 7;

    int arr[10000];
    int buf[10000];

    ofstream out("results.csv");
    out << "Size;DataType;Algorithm;Comparisons;Swaps;Time_us\n";

    for (int s = 0; s < numSizes; s++) {
        int n = sizes[s];

        for (int t = 0; t < 3; t++) {
            DataType type = (DataType)t;
            generateArray(arr, n, type);

            // --- алгоритм 1: классический пузырёк ---
            long long comps = 0, swaps = 0;
            memcpy(buf, arr, n * sizeof(int));

            auto start = chrono::high_resolution_clock::now();
            classicBubble(buf, n, comps, swaps);
            auto stop = chrono::high_resolution_clock::now();
            long long time_us = chrono::duration_cast<chrono::microseconds>(stop - start).count();

            out << n << ";" << typeName(type) << ";classic;"
                << comps << ";" << swaps << ";" << time_us << "\n";

            // --- алгоритм 2: пузырёк с флагом ---
            comps = 0; swaps = 0;
            memcpy(buf, arr, n * sizeof(int));

            start = chrono::high_resolution_clock::now();
            flagBubble(buf, n, comps, swaps);
            stop = chrono::high_resolution_clock::now();
            time_us = chrono::duration_cast<chrono::microseconds>(stop - start).count();

            out << n << ";" << typeName(type) << ";flag;"
                << comps << ";" << swaps << ";" << time_us << "\n";

            // --- алгоритм 3: пузырёк с границей ---
            comps = 0; swaps = 0;
            memcpy(buf, arr, n * sizeof(int));

            start = chrono::high_resolution_clock::now();
            boundaryBubble(buf, n, comps, swaps);
            stop = chrono::high_resolution_clock::now();
            time_us = chrono::duration_cast<chrono::microseconds>(stop - start).count();

            out << n << ";" << typeName(type) << ";boundary;"
                << comps << ";" << swaps << ";" << time_us << "\n";
        }
    }

    out.close();
    cout << "Done. Results are uploaded in results.csv" << endl;
    return 0;
}

/*
int main(){
    int size;
    cin >> size;
    int arr[size];
    int bufer[size];

    for(int i = 0; i < size; i++){
        cin >> arr[i];
    }

    long long comps, swaps;
    ofstream outRes("results.csv");
    outRes << "Algorithm;Comparisons;Swaps\n";


    for(int i = 0; i < size; i++){
        cout << arr[i] << " ";
    }
    cout << endl; 

    //классическая сортировка
    memcpy(bufer, arr, size * sizeof(int));
    classicBubble(bufer, size, comps, swaps);
    outRes << "classic;" << comps << ";" << swaps << "\n";

    cout << endl;

    comps = 0; swaps = 0;
    for(int i = 0; i < size; i++){
        cout << arr[i] << " ";
    }

    ofstream outHello("hello.csv"); //создаёт csv файл
    if (outHello.is_open()){ // ведёт запись в csv файл
        for(int i = 0; i < size; i++){
            outHello << arr[i] << ";";
        }
    }
    //outHello.close();

    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    long long time_spend = duration.count();
    cout << time_spend << endl;
    return 0;
}
*/