#include <iostream>
#include <vector>
using namespace std;


int main(){

    int size;
    cin >> size;
    int total = 0;
    vector<int> arr(size); //создали векторный массив arr размера size

    for(int i = 0; i < size; i++){
        cin >> arr[i];
    }
    bool changed = true; //флаг удаления шариков из цепочки

    while(changed){
        changed = false;
        int i = 0;
        while(i < arr.size()){ // arr.size — метод вектора, возвращает текущую длину
                                
            int j = i;
            while (j < arr.size() && arr[j] == arr[i]){
                j++;
            }
            int len = j-i; // длина найденной цепочки одинаковых шариков
            if(len >= 3){
                total += len;
                arr.erase(arr.begin() + i, arr.begin()+ j); // arr.erase(от, до) удаляет элементы
                                                            // в нужном диапазоне(остальные сдвигаются, пустоты не возникает)
                changed = true; // удаление случилось, нужен ещё один проход(while цикл)

            }
            else i = j;
        }
    }

    cout << total;
    return 0;
}



/*int main(){
    int size;
    int cnt = 1;
    int cnt_max = 0;
    cin >> size;
    int arr[size];
    for(int i = 0; i < size; i++){ //тк инкремент прибавляется после выполнения тела цикла, то настаёт момент,
    // когда i == size и именно когда происходит это условие, мы не выполняем тело цикла ещё раз
    // и соотвественно не делаем i++, значит не выходим за границы
    cin >> arr[i];
    }
    for(int i = 1; i < size; i++){
        if(arr[i-1] == arr[i]){
            cnt++;
        }
        else {
            if(cnt > cnt_max){
                cnt_max = cnt;
            }
            cnt = 1;
        }
    }
    if(cnt > cnt_max){
    cnt_max = cnt; 
    }
    // cout << cnt << endl;
    cout << cnt_max;



    return 0;
}
    */