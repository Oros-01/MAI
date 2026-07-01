#include <iostream>
using namespace std;
/*
int main() {
    int N;
    cin >> N;
    if (N == 0) {
        cout << 0;
        return 0;
    }
    
    while (N % 10 == 0) {
        N /= 10;
    }
    
    int reversed = 0;
    while (N > 0) {
        reversed = reversed * 10 + (N % 10);
        N /= 10;
    }
    
    cout << reversed;
    
    return 0;
}
    */

/*
int main(){
    int size;
    cin >> size;
    char arr[size];
    for(int i = 0; i < size; i++){
        cin >> arr[i];
    }

    for(int i = size; i >= 0; i--){
        cout << arr[i];
    }
}
    */


int gcd(int a, int b){
    if(b == 0){
        return a;
    }
    return gcd(b, a%b);
}

int main(){
    int a, b;
    cin >> a >> b;
    cout << gcd(a,b);

}