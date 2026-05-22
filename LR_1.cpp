#include <iostream>
#include <fstream>
#include <cstring>
//ofstream для вывода в файл
//ifstream для вывода в файл
using namespace std;

/*void classicBubble(int arr[], int size){
    for(int i = 1; i < size; i++){
        for(int j = 0; j < size; j++){
            if(arr[i]<arr[j]){
                swap(arr[i], arr[j]);
            }
        }
    }
}*/

void classicBubble(int arr[], int size, long long &comps, long long &swaps) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - 1; j++) {
            comps++;
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



int main(){
    int size;
    cin >> size;
    int arr[size];
    int bufer[size];

    for(int i = 0; i < size; i++){
        cin >> arr[i];
    }

    long long comps, swaps;
    ofstream out("results.csv");
    out << "Algorithm;Comparisons;Swaps\n";


    /*for(int i = 0; i < size; i++){
        cout << arr[i] << " ";
    }
    cout << endl; */

    //классическая сортировка
    memcpy(bufer, arr, size * sizeof(int));
    classicBubble(bufer, size, comps, swaps);
    out << "classic;" << comps << ";" << swaps << "\n";

    cout << endl;

    comps = 0; swaps = 0;
    for(int i = 0; i < size; i++){
        cout << arr[i] << " ";
    }

    ofstream out("hello.csv"); //создаёт csv файл
    if (out.is_open()){ // ведёт запись в csv файл
        for(int i = 0; i < size; i++){
            out << arr[i] << ";";
        }
    }
    out.close();


    return 0;
}