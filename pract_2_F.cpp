#include <iostream>
using namespace std;

int main(){
    int a_sale;
    int b_sale;
    int a_pay;
    int b_pay;
    cin >> a_sale;
    cin >> b_sale;
    cin >> a_pay;
    cin >> b_pay;

    int sdacha_rub = a_pay - a_sale;
    int sdacha_kop = b_pay - b_sale;

    if (sdacha_kop >= 100){
        sdacha_rub = sdacha_rub+1;
    }
    if(sdacha_kop<0){
        sdacha_rub = sdacha_rub-1;
        sdacha_kop = sdacha_kop+100;

    }

    cout << sdacha_rub << " " << sdacha_kop;

    return 0;
}

//Если платят 100,00 за товар который стоит 99,99, то рублевая сдача должна умножаться на 100 и переходить в копеечную
