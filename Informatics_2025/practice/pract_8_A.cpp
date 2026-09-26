#include <iostream>
using namespace std;



void reversePart(int arr[], int left, int right) {
    for (int i = left, j = right; i < j; i++, j--) {
        swap(arr[i], arr[j]);
    }
}

int main() {
    int N, A, B, C, D;
    cin >> N >> A >> B >> C >> D;
    
    int arr[N];

    for (int i = 0; i < N; i++) {
        arr[i] = i + 1;
    }
    
    reversePart(arr, A - 1, B - 1);
    
    reversePart(arr, C - 1, D - 1);
    
    for (int i = 0; i < N; i++) {
        cout << arr[i] << " ";
    }
    
    return 0;
}