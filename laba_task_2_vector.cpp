//Задание: Счетчик уникальных слов(ASCII, регистр не важен)
//Условия: Все слова на английском языке, разный регистр, который нужно игнорировать)
//1. Поступает список слов
//2. Приводим к одному регистру
//3. Проверка слова на наличие в уникальном списке
//   - Слова нет?  => добавляем в уникальный список
//   - Слово есть? => игнорируем
//---------------------------------------
// && - and
// || - or (это слеши)
//for (инициализация; условие; изменение) {тело цикла}
// Разница между большими и маленькими буквами в ASCII = 32 ('A' = 65    'a' = 97)
// "*" указатель звёздочка нужна, если хотим работать(изменять) с оригиналом, а не копией слова(изменять строку прямо в памяти)
//strlen(*name*) возвращает длину строки(без '\0'), не работает без <cstring>
// переделать на векторы
// узнать как работать с кучей

#include <bits/stdc++.h>
using namespace std;

void make_lower(string& word) {
    for (int i = 0; i < word.length(); i = i + 1) {
        if (word[i] >= 'A' && word[i] <= 'Z') {
            word[i] = word[i] + 32;
        }
    }
}

int proverka(vector<string>& words){
    vector<string> uniq_words; //теперь тут сразу строки(string), а не двумерные char массивы
    int uniq_count = 0;
    for (int i = 0; i <words.size(); i = i+1) {
        make_lower(words[i]);
    }
    for (int i = 0; i < words.size(); i = i+1) { 
        bool found = false;
        
        for (int j=0; j < uniq_words.size(); j = j+1) { // открываем цикл, по длине списка уник. слов
            bool copy_word = true; // ставим флаг 1, предполагая что слово копия
            // проходим по каждой букве двух слов и сравниваем их

            for (int k = 0; k < words[i].length(); k = k + 1) { 
                // если найдены разные буквы, слова разные
                if (words[i][k] != uniq_words[j][k]) { 
                    copy_word = false; //флаг сброшен, слова разные 
                    break; //дальше смысла проверять нет, выходим (words != uniq_words)
                }
            }
            if (copy_word) { // if (copy_word == true), сокращенная запись
                found = true; // нашли одинаковое слово (на 34 задан)
                break; //т.е. если списки одинаковы, то ничего не добавляем, а просто выходим из цикла проверки 
            }
        }// выходим из цикла уникального списка(кол-во уник. слов)
        if (!found) { //если не нашли слово(found = false), то:
            uniq_words.push_back(words[i]);
        }
    }
    return uniq_words.size();
}
int main(){
    vector<string> test_words1 = {"meow", "meOw"};
    assert(proverka(test_words1) == 1); // Тест счетчика, если посчитал кол-во уникальных слов, то complete
    vector<string> test_words2 = {"Hello", "WORLD", "hello", "world", "HELLO"};
    assert(proverka(test_words2) == 2);
    vector<string> test_words3 = {"a", "b", "c", "A", "B", "C"};
    assert(proverka(test_words3) == 3);
    cout << "All tests complete!" << endl;
    return 0;

}
