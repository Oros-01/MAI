#include <iostream>
#include <vector>


using namespace std;



int lin_search(const vector<int>& arr, int key){

    int maxborder = arr.size();
    for(int i = 0; i < maxborder; i++){
        if(arr[i] == key){
            return i;//возвращаем именно индекс
        }
    }
    return -1;//не нашли
}