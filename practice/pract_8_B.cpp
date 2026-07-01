#include <iostream>
using namespace std;

void reversePart(int arr[], int left, int right) {
    for (int i = left, j = right; i < j; i++, j--) {
        swap(arr[i], arr[j]);
    }
}



int main() {
    int N, K;
    cin >> N;
    
    int arr[N];
    for (int i = 0; i < N; i++) {
        cin >> arr[i];
    }
    cin >> K;
    
    K = K % N;
    
    if (K < 0) {
        K = N + K;
    }
    
    if (K != 0 && N > 1) {

        reversePart(arr, 0, N - 1);
        reversePart(arr, 0, K - 1);
        reversePart(arr, K, N - 1);
    }
    
    for (int i = 0; i < N; i++) {
        cout << arr[i] << " ";
    }
    
    return 0;
}