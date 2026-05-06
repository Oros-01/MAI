#include <iostream>
using namespace std;

int main(){
    int size;
    cin >> size;
    int arr[size];
    for(int i = 0; i < size; i++){ //тк инкремент прибавляется после выполнения тела цикла, то настаёт момент,
    // когда i == size и именно когда происходит это условие, мы не выполняем тело цикла ещё раз
    // и соотвественно не делаем i++, значит не выходим за границы
    cin >> arr[i];

    }
    return 0;
}