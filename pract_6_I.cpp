#include <iostream>
using namespace std;

int main(){
    int size;
    int max1 = -30001;
    int max2 = -30001;
    int min1 = 30001;
    int min2 = 30001;
    
    cin >> size;
    
    for(int i = 0; i < size; i++){
        int x;
        cin >> x;
        
        if(x > max1){
            max2 = max1;
            max1 = x;
        } else if(x > max2){
            max2 = x;
        }
        
        if(x < min1){
            min2 = min1;
            min1 = x;
        } else if(x < min2){
            min2 = x;
        }
    }
    
    int product_max1 = max1 * max2;
    int product_max2 = min1 * min2;
    
    if(product_max1 > product_max2){
        if(max1 < max2){
            cout << max1 << " " << max2;
        } else {
            cout << max2 << " " << max1;
        }
    } else {
        if(min1 < min2){
            cout << min1 << " " << min2;
        } else {
            cout << min2 << " " << min1;
        }
    }
    
    return 0;
}