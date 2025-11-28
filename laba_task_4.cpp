#include <iostream>
#include <array>
#include <string>
#include <vector>
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
enum class class_name {Math, History, Rus_lang, Eng_lang, Phisics,}
enum class days {Monday, Tuesday, Wednesday, Thursday, Friday, Saturday}
vector<char> rooms {R100, R101, R102, R103, R104, R105}

struct classes_days {
int id;
string subject;
days days;
string rooms;
}



