#include <iostream>
using namespace std;

int main(){
    int N;
    int bigger_num = 0;
    cin >> N;
    int arr[N];
    for(int i =0; i < N; i++){
        cin >> arr[i]; 
    }
    for(int i = 1; i <N ; i++){
        if(arr[i-1]<arr[i]){
            bigger_num++;
        }        
    }

    
    cout << bigger_num;
    return 0;
}