#include <iostream>
using namespace std;

int main(){
    int size;
    cin >> size;
    int arr[size];
    for(int i =0; i< size; i++){
        cin >> arr[i];
    }
    int lastnum = arr[size-1];
    
    for(int i = size; i > 0; i--){ //вначале проводим итерацию, потом i-1
        arr[i] = arr[i-1]; //последний элемент массива приравнивается к предпоследнему

    }
    arr[0] = lastnum;
    
    for(int i =0; i < size; i++){
        cout << arr[i] << " ";
    }




    return 0;
}