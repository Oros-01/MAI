#include <iostream>
using namespace std;

bool election(bool x, bool y, bool z){
    int tru = 0;
    int fals = 0;
    bool result;

    if(x) tru++; else fals++;
    if(y) tru++; else fals++;
    if(z) tru++; else fals++;
    if(tru>fals){
        result = true;
    }
    else result = false;
    return result;
}


int main(){
    bool x, y, z;
    cin >> x >> y >> z;
    bool result = election(x, y, z);
    cout << result;


    return 0;
}