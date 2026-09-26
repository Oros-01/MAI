#include <iostream>
using namespace std;

int min(int a, int b, int c, int d){
    int result;
    int arr[4] = {a,b,c,d}; 
    for(int i =0; i<4; i++){
        for(int j = 1;j<4 ;j++){
            if(arr[j]<arr[i]){
                swap(arr[j], arr[i]);
            }
        }

    }
    return arr[0];

}


int main(){
    int a, b, c, d;
    cin >> a >> b >> c >> d;
    int result = min(a,b,c,d);

    cout << result;


    return 0;
}