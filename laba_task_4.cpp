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
//COUNT пишем в конце для получения размера массива
enum class class_name {Linal, MathAnalysis, History, Rus_lang, Eng_lang, Physics, COUNT};
enum class days {Monday, Tuesday, Wednesday, Thursday, Friday, Saturday,COUNT};
enum class rooms {R100, R101, R102, R103, R104, R105, COUNT};

struct classes_days {
int id;
class_name subject;
days day;
rooms room;
};
string name_class(class_name subject){
    switch (subject)
    {
    case class_name::Linal: return "Linear Algebra";
    case class_name::MathAnalysis: return "Math Analysis";
    case class_name::History: return "History";
    case class_name::Rus_lang: return "Russian Language";
    case class_name::Eng_lang: return "English Language";
    case class_name::Physics: return "Physics";
    default: return "Unknown";

    }
}
string name_day(days day) {
    switch(day) {
        case days::Monday: return "Monday";
        case days::Tuesday: return "Tuesday";
        case days::Wednesday: return "Wednesday";
        case days::Thursday: return "Thursday";
        case days::Friday: return "Friday";
        case days::Saturday: return "Saturday";
        default: return "Unknown";
    }
}
string number_room(rooms room) {
    switch(room) {
        case rooms::R100: return "100";
        case rooms::R101: return "101";
        case rooms::R102: return "102";
        case rooms::R103: return "103";
        case rooms::R104: return "104";
        case rooms::R105: return "105";
        default: return "Unknown";
    }
}
// Функция подсчета уроков по дням
void lessons_by_day(const vector<classes_days>& schedule) {
    // Создаем массив для подсчета уроков (0-пн, 1-вт, ... 5-сб)
    int count[6] = {0};  // {0,0,0,0,0,0} - обнуляем массив
    for (const auto& lesson : schedule){
        count[static_cast<int>(lesson.day)]++;
    }
    cout << "Count lessons by day:\n";
    for(int i = 0; i < 6; i++) {
        // static_cast<days>(i) - преобразуем число обратно в enum days
        cout << name_day(static_cast<days>(i)) << ": " << count[i] << " lessons"<<endl;
    }
}
int main() {
    // Создаем тестовое расписание
    vector<classes_days> schedule = {
        {1, class_name::Linal, days::Monday, rooms::R101},
        {2, class_name::MathAnalysis, days::Monday, rooms::R102},
        {3, class_name::Physics, days::Tuesday, rooms::R103}
    };

    // Вызываем нашу функцию
    lessons_by_day(schedule);

    return 0;
}