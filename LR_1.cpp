#include <iostream>
#include <fstream>
//ofstream для вывода в файл
//ifstream для вывода в файл
using namespace std;

void classicBouble(int arr[], int size){
    for(int i = 1; i < size; i++){
        for(int j = 0; j < size; j++){
            if(arr[i]<arr[j]){
                swap(arr[i], arr[j]);
            }
        }
    }
}

void flagSorted(int arr[], int size){ //неправильно.
    bool changed = true;
    for(int i = 1; i < size; i++){
        while(changed){
            for(int j = 0; j < size; j++){
                if(arr[i]<arr[j]){
                    swap(arr[i], arr[j]);
                    changed = true;
                }
                else changed = false;
            }
        }
    }
}

int main(){
    int size;
    cin >> size;
    int arr[size];

    for(int i = 0; i < size; i++){
        cin >> arr[i];
    }
    for(int i = 0; i < size; i++){
        cout << arr[i] << " ";
    }
    cout << endl;

    classicBouble(arr, size);

    cout << endl;

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