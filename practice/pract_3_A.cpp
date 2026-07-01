#include <iostream> 
using namespace std;
//механизм вовзведния в степень
int main(){
    int N;
    int k =0;
    int osnovanie = 2;
    cin >> N;
    int result = 1;
    while(result<N){
        k++;
        result = result*osnovanie;
    }
    cout << k << endl;

    return 0;
}