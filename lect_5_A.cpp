#include <iostream>
using namespace std;

int main(){
    int N;
    cin >> N;


    int arr[N];

    for(int i = 0; i < N; i++){
        cin >> arr[i];
        if(i%2 == 0){
            cout << arr[i] << " " ;
        }
    }



    return 0;
}