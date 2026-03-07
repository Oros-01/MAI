#include <iostream>
using namespace std;

void printSquare(int number){
    int i = 1;
    while((i*i) <= number){
        cout << i*i << ", ";
        if((i*i) >= number){
            cout << ".";
            
        }
            
        i++;
    }
}


int main(){
    int N = 0;
    cout << "Введите число: " << endl;
    if (!(cin >> N) || N < 0 || N > 10000){
        cout << "НЕВЕРНЫЙ ВВОД!" << endl;
        return 1;
    }
    else {
        printSquare(N);
        cout << endl;
    }

    return 0;
}