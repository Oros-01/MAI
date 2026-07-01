#include <iostream>
using namespace std;

int main(){
    int n;
    bool flag = false;
    int num;
    cin >> n;
    int arr[n];
    for(int i = 0; i < n ; i++){// вначале массив полностью заполняется(до конца цикла)
                                // а только потом начинается второй цикл с границей n-1, где мы уже фильтруем
        cin >> arr[i];
    }
    for(int i = 0; i < n-1 ; i++){ // чтобы не выйти за границы n-1
        if((arr[i] >= 0 && arr[i+1] >= 0) || (arr[i] < 0 && arr[i+1] < 0)){
            flag = true;
            break;
        }
    }
    if(flag){
        cout << "YES";
    }
    else cout << "NO";
}