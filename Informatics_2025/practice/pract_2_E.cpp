#include <iostream>
#include <cmath>

using namespace std;

int main(){
    int k;
    cin >> k;
    if(k == 1){
        cout << "YES";
        return 0;
    }
    else if(k%4 == 0 && k >= 4){
        cout << "YES";
    }
    else cout << "NO";



    return 0;
}
//доска имеет 4 стороны, но по сути дублирование фишек, которые принадлежат
//обоим сторонам сразу нас не волнует, так как в сумме дублированные фишки дадут 4
//а в условии проверяется кратность 4.