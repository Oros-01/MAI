#include <iostream>
using namespace std;

int main(){
    int size = 12;
    double summ;
    double arr[size];
    int n;

    for(int i = 0; i<size; i++){
        cin >> arr[i];
        summ = summ + arr[i];
    }
    double avarage = summ/size;
    for(int i = 0; i<size; i++){
        if(arr[i] > avarage){
            n++;
        }
    }
    cout << n;

    return 0;
}