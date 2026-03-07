# Lessons

## Класс

```cpp
class LessonStorage
```
* подсчет уроков по дням недели
* фильтрация по дню и диапазону аудиторий
* поиск по предмету (подстрока)
* сортировка по дню, затем по аудитории
* добавление урока
* вывод списка уроков

---

# Структура Lesson

```cpp
struct Lesson {
    int id;        // ID урока
    string subject; // предмет
    int room;      // номер аудитории
    Days day;      // день недели
};
```

## Дни недели

```cpp
enum class Days {Monday, Tuesday, Wednesday, Thursday, Friday, Saturday, COUNT};
```
* **Monday** - понедельник
* **Tuesday** - вторник
* **Wednesday** - среда
* **Thursday** - четверг
* **Friday** - пятница
* **Saturday** - суббота
* **COUNT** - специальное значение, указывающее на количество элементов в enum

---

# LessonStorage

---

## Добавление урока

```cpp
void addLesson(const Lesson& l)
```

Добавляет новую комнату в вектор `lessons_box`.

---

## Количество уроков по дням недели

```cpp
map<Days, int> countByDays() const
```

Возвращает:

map<Days, int> - словарь, где ключ - день недели, значение - количество уроков в этот день

Алгоритм:
1. Инициализирует счетчики для всех дней недели нулями
2. Проходит по всем урокам в хранилище
3. Для каждого урока увеличивает счетчик соответствующего дня на 1

## Фильтр по дню и диапазону аудиторий

```cpp
vector<Lesson> FilterByDayByRoom(Days d, int from, int to) const
```

Возвращает все уроки, которые проводятся в указанный день и в аудиториях заданного диапазона.

Параметры:
- d - день недели для фильтрации
- from - минимальный номер аудитории (включительно)
- to - максимальный номер аудитории (включительно)

Возвращает:
- vector<Lesson> - вектор уроков, удовлетворяющих условиям

Условия отбора:
```cpp
lesson.day == d
lesson.room >= from && lesson.room <= to
```

---

## Поиск по предмету (подстрока)

```cpp
vector<Lesson> FindBySubject(const string& user_subj) const
```

Ищет уроки по названию предмета (регистронезависимый поиск подстроки)

Параметры:

* `user_subj` - подстрока для поиска в названии предмета

Возвращает:
- vector<Lesson> - вектор уроков, в названии предмета которых содержится заданная подстрока

Особенности:
- Поиск регистронезависимый ("math" найдет "Mathematics", "MATH", "Math")
- Если user_subj - пустая строка, возвращаются все уроки
- Используется string::find() для поиска подстроки

---

## Сортировка по дню и аудитории

```cpp
void sortByDayAndRoom()
```

Сортирует уроки в хранилище

1. По дню недели (в порядке перечисления в enum: Monday → Tuesday → ... → Saturday)
2. При одинаковом дне - по номеру аудитории (по возрастанию)

Использует:
- Стандартную функцию sort() с пользовательской лямбда-функцией сравнения
- Порядок сортировки: сначала сравниваются дни, затем аудитории

---

# Вспомогательные функции

## Преобразование дня в строку
```cpp
string dayToString(Days day)
```
Преобразует значение enum Days в читаемую строку

Возвращает:
- Строковое представление дня недели ("Monday", "Tuesday", и т.д.)

# Тестирование

---

## testCountByDays()

### Тест 1
**Входные данные: пустое хранилище уроков**

**Результат: для всех дней недели количество уроков = 0**

### Тест 1
Входные данные:
```cpp
Lesson{1, "Math", 101, Days::Monday}
Lesson{2, "Physics", 201, Days::Tuesday}
Lesson{3, "Math", 102, Days::Monday}
Lesson{4, "History", 202, Days::Thursday}
Lesson{5, "Informatics", 103, Days::Monday}
Lesson{7, "Biology", 302, Days::Wednesday}
```

**Результат:** 
- Monday: 3 урока
- Tuesday: 1 урок
- Wednesday: 1 урок
- Thursday: 1 урок
- Friday: 0 уроков
- Saturday: 0 уроков


# testFilterByDayByRoom()

### Тест 1
**Входные данные:** пустое хранилище уроков

**Фильтр:** `Monday`, аудитории `0-1000`

**результат:** пустой вектор

---

### Тест 2

**Входные данные:**
```cpp
Lesson{1, "Math", 101, Days::Monday}
Lesson{2, "Physics", 201, Days::Tuesday}
Lesson{3, "Math", 102, Days::Monday}
Lesson{4, "History", 202, Days::Tuesday}
Lesson{5, "Informatics", 103, Days::Monday}
Lesson{6, "Biology", 150, Days::Wednesday}
Lesson{7, "Chemistry", 250, Days::Thursday}
```
**Фильтр 1:** `Monday`, аудитории `0-1000`

**Результат 1:** 3 урока (ID: 1, 3, 5)

**Фильтр 2:** `Tuesday`, аудитории `200-250`

**Результат 2:** 2 урока (ID: 2, 4)

**Фильтр 3:** `Monday`, аудитории `200-300`

**Результат 3:** пустой вектор

**Фильтр 4:** `Monday`, аудитории `250-250`

**результат 4:** 1 урок (ID: 7)
## testFindBySubject()
### Тест 1
**Входные данные: пустое хранилище уроков**


**Фильтр:** `Monday`, аудитории `0-1000`

**Поиск:**  "math"

**результат:** пустой вектор
### Тест 2
**Входные данные:**

```cpp
Lesson{1, "Mathematics", 101, Days::Monday}
Lesson{2, "PHYSICS", 201, Days::Tuesday}
Lesson{3, "Math", 102, Days::Monday}
Lesson{4, "Math", 202, Days::Tuesday}
Lesson{5, "Biology", 103, Days::Wednesday}
Lesson{6, "Physical Education", 203, Days::Thursday}
```
**Поиск 1:** "mAth" (регистронезависимый)

**Результат 1:** 3 урока (ID: 1, 3, 4)

**Поиск 2:** "physics"

**Результат 2:** 1 урок (ID: 2)

**Поиск 3:** "chemistry" 

**Результат 3:** пустой вектор

**Поиск 4:** "" (пустая строка)

**Результат 4:** все 6 уроков

### Тест 3
**Входные данные:**


**Фильтр:** `Available`, минимальная вместимость `0`

**результат:** id: 1, 2

---

## testSortByDayAndRoom()
### Тест 1
**Входные данные:** пустое хранилище уроков

**Действие:** сортировка

**результат:** хранилище остаётся пустым

### Тест 2
**Входные данные:** 
```cpp
Lesson{5, "Physics", 300, Days::Tuesday}
Lesson{1, "Math", 200, Days::Monday}
Lesson{4, "History", 150, Days::Tuesday}
Lesson{2, "Physics", 100, Days::Monday}
Lesson{3, "Math", 250, Days::Monday}
Lesson{6, "Chemistry", 50, Days::Wednesday}
Lesson{7, "Biology", 350, Days::Thursday}
```
**Действие:** сортировка

**результат:**
- Monday: аудитории 100, 200, 250
- Tuesday: аудитории 150, 300
- Wednesday: аудитория 50
- Thursday: аудитория 350


