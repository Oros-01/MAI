#include <iostream>
using namespace std;

int main(){
    int k;
    cin >> k;

    if (k == 1 || k == 2 || k == 4 || k == 7){
        cout << "NO";
    }
    else cout << "YES";
}

//убираем слишком маленькие числа, оставляя остальные, которые можно получить комбинируя 3 и 5