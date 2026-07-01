#include <iostream>
using namespace std;

int main() {
    int size;
    cin >> size;
    
    int arr[size];
    for(int i = 0; i < size; i++) {
        cin >> arr[i];
    }
    
    int swapCount = 0;
    
    for(int i = 0; i < size - 1; i++) {
        for(int j = 0; j < size - i - 1; j++) {
            if(arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
                swapCount++;
            }
        }
    }
    
    cout << swapCount << endl;
    
    return 0;
}