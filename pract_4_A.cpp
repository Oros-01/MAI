#include <iostream>
using namespace std;

int main(){
    int n;
    int num = 0;
    int summ = 0;
    cin >> n;
    for(int i = 0; i < n; i++){
        cin >> num;
        summ = summ + num;
        //Если поменяем местами num и summ, то последнее число так и не прибавится, программа запросит его
        //ввести, а потом закроет цикл, так как следующая i, уже будет за нашей границей(n)
    }
    cout << summ;
}