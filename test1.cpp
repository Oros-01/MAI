//1. На вход поступает 2 перменные(ширина и высота)
//2. Засуммировать эти 2 перменные и умножаем на 2 = периметр прямоугольника

//Как задать разные комбинации этих 2 переменных?
//В ручную вводит пользователь
#include <iostream>
int main(){
    int length;
    int height;
    std:: cout << "length: " << std:: endl;
    std:: cin >> length;
    std:: cout << "height: " << std:: endl;
    std:: cin >> height;
    std:: cout << "perimeter:" << (length+height)*2 << std:: endl;
    return 0;
}