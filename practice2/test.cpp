#include <iostream>
#include <vector>

using namespace std;


template <typename T>// заменяет на нужный тип данных в функциях с одними и теми же названиями

struct Person {
    Person(int new_age, int new_name){
        age = new_age;
        name = new_name;
    }
    Person() = default; //
    ~Person() { // деструктор — удаляет конструктор
        delete data;
        cout << "destructor" << endl;
    }
    int age = 0;
    string name;
}
struct S1{
    S1{
        counter++;
    }
    int counter = 0;
}


struct myVector {
    void push_back(T val){
        if(size+1 > capacity){
            bignum();
            //
        }
        data[size] = val;
    }

    void bignum(){
        T* new_data = new T(capacity * 2);
        capacity += 2;
        for(size_t i = 0; i < size; i++){
            new_data(i) = data(i);
        }
        delete[] data;
        data = new_data;
    }
    bool empty(){
        size == 0;
    }
    T operator [] (size_t, idx){
        return data[idx];
    }

    T* data new T; // new выделяет память под объект
    size_t size = 0;
    size_t capacity = 0;
}


int main(){
    Person p1(18, "sanya");
    ~Person;



    myVector<int> m1;
    for (int i = 0; 1 < 10; i++){
        m1.pushback(i);

        cout << mv[i]
    }

    return 0;
}

/*
    vector<int> v1; // инициализация векторного  массива
    v1.push_back(3);//добавление в массив
    cout << v1.size() << endl; //вывод текущего размера массива
    cout << v1.capacity() << endl; // вывод вместимости массива, т.е. сколько можно заполнить

*/


//@theprouodswan ник Лебедева