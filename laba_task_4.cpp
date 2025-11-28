#include <iostream>
#include <array>
#include <string>
#include <vector>
using namespace std;
/*Уроки и дни недели  
- Объект: урок (идентификатор, предмет, аудитория, день недели).  
- Категории: рабочие дни (например: пн–пт).  
- Реализовать: количество уроков по дням; фильтр по дню и диапазону аудиторий; поиск по предмету (подстрока); сортировка по дню, затем по аудитории."
*/
/*Что такое size_t?
size_t - беззнаковый целый тип для хранения размеров массивов и индексов
Гарантированно вмещает размер любого объекта в памяти
Используется для индексации массивов
*/
/*
1. Создаем классы(польз. типы данных)
2. Создаем структуру(шаблон)
3. Заполняем несколько шаблонов
*/
enum class days {Monday, Tuesday, Wednesday, Thursday, Friday, Saturday, COUNT}

struct lesson {
int id;
string subject;
int room;
days day;
}
int lesson_count(vector<lesson> list, days day){ // вектор массив с типом данным lesson, с названием list, состоящий из day
    day_count = 6;
    for (int i = 0; i <= day_count; i= i+1){
        cout << day << endl;
        return 0;
    }
}




