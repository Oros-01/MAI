#include <iostream>
using namespace std;

int main(){
    int size;
    cin >> size;
    int arr[size];
    
    for(int i =0; i < size; i++){
        cin >> arr[i];
    }
    int maxIndex = 0;
    int minIndex = 0;

    for(int i =0; i < size; i++){
        if(arr[i] > arr[maxIndex]){
            maxIndex = i; 
        }
    }
    int min = arr[0];
    for(int i = 0; i < size; i++){
        if(arr[i] < arr[minIndex]){
            minIndex = i;
        }
    }
    swap(arr[minIndex], arr[maxIndex]);

    for(int i = 0; i < size; i++){
        cout << arr[i] << " ";
    }
}