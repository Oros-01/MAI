#include <iostream>
using namespace std;

int main(){
    int rub = 0;
    float kop = 0;
    int n = 0;
    cin >> rub;
    cin >> kop;
    cin >> n;
    float summaRub = ((n*rub) + (n*(kop/100)));
    cout << "Сумма в рублях: " << summaRub << endl;
    cout << "Сумма в копейках: " << summaRub*100 << endl;

    return 0;
}