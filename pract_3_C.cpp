#include <iostream>
using namespace std;

int main(){
    int vklad;
    int koef;
    int result;
    int period = 0;
    cin >> vklad >> koef >> result;

    while(vklad < result){
        vklad = vklad + (vklad * koef)/100; // по сути перестановка знаков не играет значения(коэф делить на 100 доо или после)
        period++;
    }
    cout << period;



    return 0;
}