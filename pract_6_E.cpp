#include <iostream>
using namespace std;

int main(){
    int size;
    cin >> size;
    int arr[size];
    
    for(int i =0; i < size; i++){
        cin >> arr[i];
    }
    int max = arr[0];
    for(int i =0; i < size; i++){
        if(arr[i] > max){
            max = arr[i];
        }
    }
    int min = arr[0];
    for(int i = 0; i < size; i++){
        if(arr[i] < min){
            min = arr[i];
        }
    }
    for(int i = 0; i < size; i++){
        if(max == arr[i]){
            if(min == arr[j]){
                swap(arr[j], arr[i])
            }
        }
    }
    cout << max;
    cout << min;
}