//1. На вход поступает 2 перменные(ширина и высота)
//2. Задача: найти периметр прямоугольника по двум переменным
//3. Создать функцию, которая будет суммировать эти 2 перменные и умножать на 2 = периметр прямоугольника
// 
//////////////////
//функция per: поступают величины(ширина и длина) прямоугольника в численном виде(int, double, float)
//в теле функции первая и вторая величина суммируются и умножаются на 2, в итоге на выходе получаем периметр прямоугольника




//Как задать разные комбинации этих 2 переменных?
//Сами задаем через тесты
#include <iostream>
#include <cassert>
int per(int length, int height){
    return (length+height)*2;
}
double per(double length, double height){
    return (length+height)*2;
}
float per(float length, float height){
    return (length+height)*2;
}
int main(){
    assert(per(2,3)==10);
    std:: cout << "test 1 complete" << std::endl;
    assert(per(2,2)==8);
    std:: cout << "test 2 complete" << std::endl;
    assert(per(6.5, 5.55)==24.1);
    std:: cout << "test 3 complete" << std::endl;
    return 0;
}