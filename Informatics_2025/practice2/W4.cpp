#include <iostream>
#include <vector>
#include <set>
#include <execution>

using namespace std;


const float p1 = 3.14;
#define MACROS(x)

void print_vector_10th(vector<int> v){
    if(v.size() < 10){
        throw out_of_range("vector doesn't contain enough elements"); // throw выбросить ошибку
    }
    cout << v[9] << endl;
}



int main() {
    cerr << "Error" << endl; //cerr поток вывода(cout), но для ошибок!
    int var;
    vector<int> v = (1,2);
    cout << v.at(5) << endl; // метод at(№ элемента) в vector позволяет обращаться к конкретному элементу массива вектора
}