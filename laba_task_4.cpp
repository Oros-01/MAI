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
enum class class_name {Linal, Mathanalisys, History, Rus_lang, Eng_lang, Phisics}
enum class days {Monday, Tuesday, Wednesday, Thursday, Friday, Saturday}
std:: array <char[], days> days+1
enum class rooms {100, 101, 102, 103, 104, 105}

struct classes_days {
int id;
char class_name;
char day;
int room_number;
}
for (int i = 0; i < 6; i = i+1){
classes_day.room_number[i]
}
