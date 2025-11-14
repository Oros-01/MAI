//1. На вход поступает 2 перменные(ширина и высота)
//2. Задача: найти периметр прямоугольника по двум переменным
//3. Создать функцию, которая будет суммировать эти 2 перменные и умножать на 2 = периметр прямоугольника


// В программу поступают данные в численном виде, их сумма умножается на 2 = периметр
// Функция per: в условия поступают 2 int/double/float данные, в теле функции возвращается сумма 1-го и 2-го значения умноженная на 2
// Пример функции per(длина, высота){
//    тело функции: (длина+высота)*2; и возврат результата }


#include <iostream>
#include <cassert>
int per(int length, int height){
    return (length+height)*2;
}
float per(float length, float height){
    return (length+height)*2.0;
}

double per(double length, double height){
    return (length+height)*2.0;
}




int main(){
    assert(per(2,3)==10);
    std:: cout << "test 1 complete" << std::endl;
    assert(per(2,2)==8);
    std:: cout << "test 2 complete" << std::endl;
    assert(per(3.5 , 2.8)==12.6);
    std:: cout << "test 3 complete" << std::endl;
    return 0;
}