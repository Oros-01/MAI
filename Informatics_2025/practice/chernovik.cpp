/* Уроки и дни недели  
- Объект: урок (идентификатор, предмет, аудитория, день недели).  
- Категории: рабочие дни (например: пн–пт).  
- Реализовать: количество уроков по дням; фильтр по дню и диапазону аудиторий; поиск по предмету (подстрока); сортировка по дню, затем по аудитории."
*/

//std:: array <int, 5>//тип данных и размер массива
// switch (s) case заменяет многоразовое написание if else(разобраться)
//
/* struct *name* {
    тип данных название;
    тип данных название;
    тип данных название;
} */
// const это фиксатор значения(константа)(если нарушить const компилятор выдаст ошибку)


//static_cast<type>(value)
//В угловых скобках указывается тип,
//к которому необходимо преобразовать значение value, которое стоит в круглых скобках




//
#include <bits/stdc++.h>
#include <windows.h>
using namespace std;


enum class Days {Monday, Tuesday, Wednesday, Thursday, Friday, Saturday,COUNT}; //пользовательский тип данных
//---Структура
struct Lesson { //шаблон-набор данных (только данные)
    int id;
    string subject;
    int room;
    Days day; //тип данных Days(наш новый тип данных, который мы сами создали) и переменная day
    // Переменная day может быть только одним из Days(т.е. Monday, Tuesday, ...)
};//конец структуры

class LessonStorage { // Это шаблон объекта (данные + логика,поведение)
    // Всё что до public: автоматом входит в private:
    vector<Lesson> lessons_box; //вектор который хранит переменные типа Lesson(то есть наши объекты)
    vector<string> subjects = {"Math", "Physics", "Informatics", "History"};
    vector<int> rooms = {101, 102, 103, 201, 202, 203};
   

    // функции внутри объекта это методы(то есть что объект умеет делать)
public: //интерфейс класса LessonStorage(всё что ниже — можно вызвать снаружи)

    void addLesson(const Lesson& l_name) {// название переменной (const не изменять + работать с оригиналом(&) шаблон объекта, название объекта, допустим "l1")
        lessons_box.push_back(l_name); //в вектор lessons добавляется конкретный объект l_name
    }
    //объявление метода(тип метода(вектор), название метода, параметры(что нужно передать при вызове метода и далее тело метода {..))
    vector<Lesson> FilterByDayByRoom(Days d, int from, int to) const {
        vector<Lesson> result; //пустой вектор

        for (const Lesson& one_les: lessons_box) { //one_les название переменной и он будет браться из lessons_box
            bool dayMatch = (one_les.day == d); //флаг что выбранный день совпадает с днём взятого объекта, то есть one_les
            bool roomInRange = (one_les.room >= from && one_les.room <=to); //флаг что комната объекта начинается с числа from и заканчивается числом to

            if (dayMatch && roomInRange) { //если оба флага верны мы добавляем в пустой result наш one_les
                result.push_back(one_les);
            }
        }
        return result; // возвращаем все one_les, в котором содержится нужный день и диапазон аудиторий(то есть те дни, которые нужны пользователю по дню и аудитории)

    }
    
    vector<Lesson> FindBySubject(const string& user_subj) const {
        vector<Lesson> result; //создаем пустой вектор для возвращаемых результатов
        string lower_user_subj; //создаем пустую строку
        //к нижнему регистру строку, которую вводит пользователь
        for (char ch: user_subj) {
            lower_user_subj += tolower(ch);//добавляем преобразованный к нижнему регистру символ
        }
        //главный цикл, перебор каждого one_les(конкретный объект l1 например)
        for (const Lesson& one_les: lessons_box){ // для каждого урока преобразуем его название
            string lowerSubj; // пустая строка для названия предмета
            // подцикл, перебор побуквенно по названию объекта (l1.subject например)
            for (char ch : one_les.subject){ // побуквенно проходимся 
                lowerSubj += tolower(ch); //приводим каждый символ из subject к нижней строке(заполняем пустую строку созданную выше)
            } //lower_user_subj(подстрока) ищется в строке lowerSubj(название предмета)
            if (lowerSubj.find(lower_user_subj) != string:: npos) { //если не нашёл, он вернёт string::npos
                result.push_back(one_les);
            }
        }
        return result;
    }

    void sortByDayAndRoom() {
        // lessons_box.begin() - указатель на начало вектора
        // lessons_box.end() - указатель на конец вектора
        // Третий параметр - лямбда-функция, которая определяет порядок сортировки
        
        sort(lessons_box.begin(), lessons_box.end(), 
            // Лямбда-функция: [](параметры) { тело }
            // Принимает два урока (a и b) для сравнения
            // Возвращает true, если a должен идти РАНЬШЕ b
            [](const Lesson& lesson_a, const Lesson& lesson_b) {
                
                // Правило 1: Сначала сравниваем дни
                if (lesson_a.day != lesson_b.day) {
                    // Если дни разные, то урок с "меньшим" днем идет раньше
                    // Monday (0) < Tuesday (1) < Wednesday (2) и т.д.
                    return lesson_a.day < lesson_b.day;
                }
                
                // Правило 2: Если дни одинаковые, сравниваем аудитории
                // Урок с меньшей аудиторией идет раньше
                return lesson_a.room < lesson_b.room;
            }
        );
    }

    map<Days, int> countByDays() const { // объявили его и открыли его тело

        // ШАГ 1 задаем каждому дню(ключу) числовое значение int 0
        // т.е. таблицу соответствий где каждый key может хранить в себе одно value
        map<Days, int> Les_for_day;

        Les_for_day[Days::Monday]    = 0;
        Les_for_day[Days::Tuesday]   = 0;
        Les_for_day[Days::Wednesday] = 0;
        Les_for_day[Days::Thursday]  = 0;
        Les_for_day[Days::Friday]    = 0;
        Les_for_day[Days::Saturday]  = 0;

        // Подсчёт уроков(объектов) в день
        for (const Lesson& one_les: lessons_box) { //(const *struct name*& *переменная name* : *хранилище name*)
            // берём каждый урок из lessons и называем его one_les

            Les_for_day[one_les.day]++; //То есть если у нас существует объект с днём Monday, то у Monday будет +1
            // берём день урока
            // и увеличиваем счётчик на 1

        }
        return Les_for_day; //Возвращаем измененную таблицу ассоциативности после циклов
    }
        
};// конец LessonStorage(шаблона)


void testCountByDays() {
    LessonStorage empty_storage;
    //TEST 1
    map<Days, int> result1 = empty_storage.countByDays();
    assert(result1[Days::Monday] == 0);
    assert(result1[Days::Tuesday] == 0);
    assert(result1[Days::Wednesday] == 0);
    assert(result1[Days::Thursday] == 0);
    assert(result1[Days::Friday] == 0);
    assert(result1[Days::Saturday] == 0);
    cout << "TEST1 complete" << endl;

    //TEST 2
    LessonStorage test_storage;
    test_storage.addLesson({1, "Math", 101, Days::Monday});
    test_storage.addLesson({2, "Physics", 201, Days::Tuesday});
    test_storage.addLesson({3, "Math", 102, Days::Monday});
    test_storage.addLesson({4, "History", 202, Days::Thursday});
    test_storage.addLesson({5, "Informatics", 103, Days::Monday});
    test_storage.addLesson({7, "Biology", 302, Days::Wednesday});

    map<Days, int> result2 = test_storage.countByDays();
    assert(result2[Days::Monday] == 3);
    assert(result2[Days::Tuesday] == 1);
    assert(result2[Days::Wednesday] == 1);
    assert(result2[Days::Thursday] == 1);
    assert(result2[Days::Friday] == 0);
    assert(result2[Days::Saturday] == 0);
    cout << "TEST2 complete" << endl;
    
    cout << "All tests countByDays complete!" << endl;
}
// доделать тесты
void testFilterByDayByRoom() {// 
    vector<Lesson> empty;
    LessonStorage emptyStorage;
    vector<Lesson> res1 = emptyStorage.FilterByDayByRoom(Days::Monday, 0, 1000);
    assert(res1.empty());

    LessonStorage storage;
    storage.addLesson({1, "Math", 101, Days::Monday});
    storage.addLesson({2, "Physics", 201, Days::Tuesday});
    storage.addLesson({3, "Math", 102, Days::Monday});
    storage.addLesson({4, "History", 202, Days::Tuesday});
    storage.addLesson({5, "Informatics", 103, Days::Monday});
    storage.addLesson({6, "Biology", 150, Days::Wednesday});
    storage.addLesson({7, "Chemistry", 250, Days::Thursday});

    vector<Lesson> res2 = storage.FilterByDayByRoom(Days::Monday, 100, 150);
    assert(res2.size() == 3);

    vector<Lesson> res3 = storage.FilterByDayByRoom(Days::Tuesday, 200, 250);
    assert(res3.size() == 2);

    vector<Lesson> res4 = storage.FilterByDayByRoom(Days::Wednesday, 200, 300);
    assert(res4.empty());

    vector<Lesson> res5 = storage.FilterByDayByRoom(Days::Thursday, 250, 250);
    assert(res5.size() == 1);

    vector<Lesson> res6 = storage.FilterByDayByRoom(Days::Friday, 0, 1000);
    assert(res6.empty());
}

void testFindBySubject() {
    vector<Lesson> empty;
    LessonStorage emptyStorage;
    vector<Lesson> res1 = emptyStorage.FindBySubject("math");
    assert(res1.empty());

    LessonStorage storage;
    storage.addLesson({1, "Mathematics", 101, Days::Monday});
    storage.addLesson({2, "PHYSICS", 201, Days::Tuesday});
    storage.addLesson({3, "Math", 102, Days::Monday});
    storage.addLesson({4, "Math", 202, Days::Tuesday});
    storage.addLesson({5, "Biology", 103, Days::Wednesday});
    storage.addLesson({6, "Physical Education", 203, Days::Thursday});

    vector<Lesson> res2 = storage.FindBySubject("mAth"); // изменил
    assert(res2.size() == 3);

    vector<Lesson> res3 = storage.FindBySubject("physics");
    assert(res3.size() == 1);

    vector<Lesson> res4 = storage.FindBySubject("chemistry");
    assert(res4.empty());

    vector<Lesson> res5 = storage.FindBySubject("");
    assert(res5.size() == 6);
}

void testSortByDayAndRoom() {
    vector<Lesson> empty;
    LessonStorage emptyStorage;
    emptyStorage.sortByDayAndRoom();
    
    // Проверяем, что сортировка пустого хранилища не ломает ничего
    map<Days, int> counts = emptyStorage.countByDays();
    assert(counts[Days::Monday] == 0);

    LessonStorage storage;
    storage.addLesson({5, "Physics", 300, Days::Tuesday});
    storage.addLesson({1, "Math", 200, Days::Monday});
    storage.addLesson({4, "History", 150, Days::Tuesday});
    storage.addLesson({2, "Physics", 100, Days::Monday});
    storage.addLesson({3, "Math", 250, Days::Monday});
    storage.addLesson({6, "Chemistry", 50, Days::Wednesday});
    storage.addLesson({7, "Biology", 350, Days::Thursday});

    storage.sortByDayAndRoom();

    // Проверяем через фильтры
    vector<Lesson> mondays = storage.FilterByDayByRoom(Days::Monday, 0, 1000);
    assert(mondays.size() == 3);
    assert(mondays[0].room == 100);
    assert(mondays[1].room == 200);
    assert(mondays[2].room == 250);

    vector<Lesson> tuesdays = storage.FilterByDayByRoom(Days::Tuesday, 0, 1000);
    assert(tuesdays.size() == 2);
    assert(tuesdays[0].room == 150);
    assert(tuesdays[1].room == 300);

    vector<Lesson> wednesdays = storage.FilterByDayByRoom(Days::Wednesday, 0, 1000);
    assert(wednesdays.size() == 1);
    assert(wednesdays[0].room == 50);
}


string dayToString(Days day) {
    switch(day) {
        case Days::Monday: return "Monday";
        case Days::Tuesday: return "Tuesday";
        case Days::Wednesday: return "Wednesday";
        case Days::Thursday: return "Thursday";
        case Days::Friday: return "Friday";
        case Days::Saturday: return "Saturday";
        default: return "Unknown";
    }
}

Days intToDay(int day) {
    switch(day) {
        case 0: return Days::Monday;
        case 1: return Days::Tuesday;
        case 2: return Days::Wednesday;
        case 3: return Days::Thursday;
        case 4: return Days::Friday;
        case 5: return Days::Saturday;
        default: return Days::Monday;
    }
}
int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    LessonStorage storage;
    // Запускаем все тесты
    testCountByDays();
    testFilterByDayByRoom();
    testFindBySubject();
    testSortByDayAndRoom();
    
    cout << "All tests passed!" << endl;

    cout << "\n=== Управление уроками ===" << endl;
    
    // Ввод данных (как в примере с заказами)
    int n;
    while (true) {
        cout << "Введите количество уроков: ";
        if (!(cin >> n) || n < 0 || n > 1000) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Неверный ввод" << endl;
        } else {
            break;
        }
    }
    
    cout << n << endl; 
    
    for (int i = 0; i < n; i++) {
        while (true) {
            int id;
            string subject;
            int room;
            int dayInt;
            
            cout << "День = {0-Понедельник, 1-Вторник, 2-Среда, 3-Четверг, 4-Пятница, 5-Суббота}" << endl;
            cout << i + 1 << " - Введите <id subject room day>: ";
            
            if (!(cin >> id >> subject >> room >> dayInt)) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Неверный ввод" << endl;
            } else {
                if (room < 0 || room > 10000) {
                    cout << "Неверный ввод аудитории" << endl;
                } else if (dayInt < 0 || dayInt > 5) {
                    cout << "Неверный ввод дня" << endl;
                } else {
                    // Создаем урок и добавляем в хранилище
                    Lesson new_lesson = {
                        id: id,
                        subject: subject,
                        room: room,
                        day: intToDay(dayInt)
                    };
                    storage.addLesson(new_lesson);
                    break;
                }
            }
        }
    }

    // Основное меню (как в примере с заказами)
    while (true) {
        cout << "\n=== Меню ===" << endl;
        cout << "1. Количество уроков по дням" << endl;
        cout << "2. Фильтр по дню и диапазону аудиторий" << endl;
        cout << "3. Поиск по предмету (подстрока)" << endl;
        cout << "4. Сортировка по дню, затем по аудитории" << endl;
        cout << "5. Выход" << endl;
        
        int op;
        while (true) {
            cout << "Выбор: ";
            if (!(cin >> op) || op < 1 || op > 5) {
                cout << "Неверный ввод" << endl;
                cin.clear();
                cin.ignore(1000, '\n');
            } else {
                break;
            }
        }
        
        if (op == 1) {
            // 1. Количество уроков по дням
            cout << "\n=== Количество уроков по дням ===" << endl;
            map<Days, int> counts = storage.countByDays();
            
            // Выводим для всех дней
            for (int i = 0; i < 6; i++) {
                Days d = intToDay(i);
                cout << dayToString(d) << ": " << counts[d] << " уроков" << endl;
            }
        }
        else if (op == 2) {
            // 2. Фильтр по дню и диапазону аудиторий
            cout << "\n=== Фильтр по дню и аудитории ===" << endl;
            
            int dayInt;
            while (true) {
                cout << "Введите день (0-5): ";
                if (!(cin >> dayInt) || dayInt < 0 || dayInt > 5) {
                    cout << "Неверный ввод" << endl;
                    cin.clear();
                    cin.ignore(1000, '\n');
                } else {
                    break;
                }
            }
            
            int from, to;
            while (true) {
                cout << "Введите диапазон аудиторий (от и до): ";
                if (!(cin >> from >> to)) {
                    cout << "Неверный ввод" << endl;
                    cin.clear();
                    cin.ignore(1000, '\n');
                } else {
                    break;
                }
            }
            
            vector<Lesson> result = storage.FilterByDayByRoom(intToDay(dayInt), from, to);
            cout << "Найдено " << result.size() << " уроков:" << endl;
            
            if (!result.empty()) {
                cout << "ID\tПРЕДМЕТ\tАУДИТОРИЯ\tДЕНЬ" << endl;
                for (const Lesson& lesson : result) {
                    cout << lesson.id << "\t" 
                         << lesson.subject << "\t" 
                         << lesson.room << "\t\t" 
                         << dayToString(lesson.day) << endl;
                }
            }
        }
        else if (op == 3) {
            // 3. Поиск по предмету (подстрока)
            cout << "\n=== Поиск по предмету ===" << endl;
            string search_subject;
            cout << "Введите название предмета (или часть): ";
            cin >> search_subject;
            
            vector<Lesson> result = storage.FindBySubject(search_subject);
            cout << "Найдено " << result.size() << " уроков:" << endl;
            
            if (!result.empty()) {
                cout << "ID\tПРЕДМЕТ\tАУДИТОРИЯ\tДЕНЬ" << endl;
                for (const Lesson& lesson : result) {
                    cout << lesson.id << "\t" 
                         << lesson.subject << "\t" 
                         << lesson.room << "\t\t" 
                         << dayToString(lesson.day) << endl;
                }
            }
        }
        else if (op == 4) {
            // 4. Сортировка по дню, затем по аудитории
            cout << "\n=== Сортировка уроков ===" << endl;
            storage.sortByDayAndRoom();
            cout << "Уроки отсортированы!" << endl;
            
            // Покажем результат сортировки
            // Для этого создадим копию всех уроков (в реальности нужно добавить метод)
            // Вместо этого просто выведем сообщение
            cout << "Теперь уроки отсортированы по дню недели," << endl;
            cout << "а внутри каждого дня - по номеру аудитории." << endl;
        }
        else if (op == 5) {
            // 5. Выход
            cout << "Выход из программы." << endl;
            break;
        }
    }
    
    return 0;
}
    









    /* LessonStorage storage; // создали объект с поведением и методами(LessonStorage)
    Lesson l1 {1, "Math", 405, Days::Monday}; // создали конкретный объект l1
    Lesson l2 {2, "Phisics", 312, Days::Tuesday};
    Lesson l3 {3, "Math", 412, Days::Tuesday};

    storage.addLesson(l1); //с помощью функции добавили его в lessons(вектор)
    storage.addLesson(l2);
    storage.addLesson(l3);
    
    map<Days, int> Les_for_day = storage.countByDays();

    cout<<"Monday: " << Les_for_day[Days::Monday] <<endl;
    cout<<"Tuesday: " << Les_for_day[Days::Tuesday] <<endl;
    

    vector<Lesson> filter_result = storage.FilterByDayByRoom(Days::Tuesday, 0, 500);
    cout <<"number lessons finded: " << filter_result.size() << endl;
    for (int i = 0; i < filter_result.size(); i++) {
        cout << i+1 <<". " << filter_result[i].subject << " (room " <<filter_result[i].room 
        << ")" << endl;
    }
    return 0; */ 