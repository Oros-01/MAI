#include <iostream>
using namespace std;


void reversePart(int arr[], int left, int right) {
    for (int i = left, j = right; i < j; i++, j--) {
        swap(arr[i], arr[j]);
    }
}
  
int main(){
    int size;
    cin >> size;
    int arr[size];

    for(int i = 0; i < size; i++){
        cin >> arr[i];
    }

    return 0;
}