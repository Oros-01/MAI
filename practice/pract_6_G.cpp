#include <iostream>
using namespace std;

int main(){
    int size;
    cin >> size;
    int arr[size];
    int height;
    int pos = 1;
    for(int i = 0; i < size; i++){
        cin >> arr[i];
    }

    cin >> height;
    for(int i = 0; i < size; i++){
        if(arr[i] >= height){
            pos++;
        }
    }
    cout << pos;

    return 0;

}