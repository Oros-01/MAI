#include <iostream>
using namespace std;

void sort(int arr[], int size){
    for(int i = 0; i < size-1; i+=2){
        swap(arr[i], arr[i+1]);


    }
}


int main(){
    int size;
    cin >> size;
    int arr[size];

    for(int i = 0; i < size; i++){
        cin >> arr[i];
    }
    sort(arr, size);
    for(int i = 0; i < size; i++){
        cout << arr[i] << " ";
    }

    return 0;

}