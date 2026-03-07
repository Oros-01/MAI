/*
"Задание 12: Проверка на подстроку ""abc""
Автомат определяет, заканчивается ли строка на ""abc"".

**Пример:**
- `""xyzabc""` → заканчивается
- `""abcdef""` → не заканчивается
- `""abc""` → заканчивается"
*/

#include <bits/stdc++.h>
using namespace std;

enum class State {Start, Found_C, Found_BC, Found_ABC, Invalid};

struct Automaton{
    State status;
};

bool isLetterC(const char symbol){
    return tolower(symbol) == 'c'; //возвращает true, если символ равен 'c'(т.е. вся конструкция (sybol == 'c') может иметь либо true либо false )
}

bool isLetterB(const char symbol){
    return tolower(symbol) == 'b'; //возвращает true, если символ равен 'c'
}

bool isLetterA(const char symbol){
    return tolower(symbol) == 'a'; //возвращает true, если символ равен 'c'
}


void transition (Automaton& aut, const char input) {
    switch (aut.status){ // смотрим на текущее состояние автомата
        case State::Start: // Действие если наше состояние Start
            if (isLetterC(input)) {
                aut.status = State::Found_C;
            }
            else {
                aut.status = State::Invalid;
            }
            break;
        
        case State::Found_C:
            if (isLetterB(input)) {
                aut.status = State::Found_BC;
            }
            else {
                aut.status = State::Invalid;
            }
            break;

        case State::Found_BC:
            if (isLetterA(input)) {
                aut.status = State::Found_ABC;
            }
            else {
                aut.status = State::Invalid;
            }
            break;
        case State::Found_ABC:
            break;
        case State::Invalid:
            break;
    }
}

bool obrabotka(const string &input){
    if (input.length() < 3) {
        return false;
    }
    
    // создаем автомат
    Automaton aut1;
    aut1.status = State::Start;
    for (int i = input.length() - 1; i >= 0; i--) { //-1 тк индекс начинается с 0
        char current_char = input[i];

        transition(aut1, current_char);
         if (aut1.status == State::Found_ABC || aut1.status == State::Invalid) {
            break;  // Выходим из цикла
        }
    }

    if (aut1.status == State::Found_ABC) { // единственный упешный случай
        return true;
    } else {
        return false;
    }
}

void tests(){
    assert(obrabotka("xyzabc") == true);
    assert(obrabotka("abcdef") == false);
    assert(obrabotka("abc") == true);
    assert(obrabotka("ab") == false);
    assert(obrabotka("testABC") == true);
    cout << "All tests complete!" << endl;

}

int main(){
    tests();
    return 0;
}