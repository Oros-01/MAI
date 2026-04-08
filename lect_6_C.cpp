#include <iostream>
using namespace std;

int phi(int n){
    int arr[n];
    //частично заполняем массив
    for(int i = 0; i < 2; i++){
        arr[i] = 1;
    }
    // заполняем массив последовательностью
    for(int i = 2; i <= n; i++){
        arr[i] = arr[i-1] + arr[i-2];


    }
    return arr[n];
}

main(){
    int n;
    cin >> n;
    int result = phi(n);
    cout << result;


    return 0;
}