#include <iostream>
using namespace std;

int main(){
    int n = 0;
    cin >> n;
    int numcnt = 0;
    int numprt = 1;
    for(int i = 0; i < n; i++){
        cout << numprt << " ";
        numcnt++;
        if(numcnt==numprt){
            numprt++;
            numcnt = 0;
        
        }
        

    }
    

    return 0;
}