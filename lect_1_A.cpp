#include <iostream>
#include <cmath>
using namespace std;



int calc(int a, int b){
    int result = ((a*a) + (b*b));
    return result;
    }

int main(){
    int a = 0;
    int b = 0;
        cout << "Введите длину 1-го катета:" << endl;
        
        if (!(cin >> a) || a < 0 || a > 1000){
            cout << "НЕВЕРНЫЙ ВВОД" << endl;
            return 1;
        }
        cout << "Введите длину 2-го катета:" << endl;
        
        if (!(cin >> b) || b < 0 || b > 1000){
            cout << "НЕВЕРНЫЙ ВВОД" << endl;
            return 1;
        }
    cout << "Длина гипотенузы равна: " << sqrt(calc(a,b)) << endl;

    return 0; 
}
