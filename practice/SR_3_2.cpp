#include <iostream>
using namespace std;

int main(){
    int size = 12;
    int arr[size];
    
    for(int i = 0; i < size; i++){
        cin >> arr[i];
    }
    for(int i = 0; i < size - 1; i++){
        if(i % 2 == 0 && arr[i] < 0){
            for(int j = i + 1; j < size; j++){
                if(arr[j] >= 0){
                    swap(arr[i], arr[j]);
                    break;
                }
            }
        }
        else if(i % 2 == 1 && arr[i] >= 0){
            for(int j = i + 1; j < size; j++){
                if(arr[j] < 0){
                    swap(arr[i], arr[j]);
                    break;
                }
            }
        }
    }
    
    for(int i = 0; i < size; i++){
        cout << arr[i] << " ";
    }
    
    return 0;
}