/*
Проверка на анаграмму(На вход даётся 2 строки, нужно проверить являются ли они анаграммамой друг друга или нет)
1. На вход даётся 2 разных строки
2. Нужно сравнить эти 2 двумерных[символ][слово] массива 
3. Не забудь make_lower при "сложных" вариантах для краша проги
4. Методы решений:
 ! (не подходит) Перевести слова в целочисленное и сравнивать эти целочисленные суммы
 ! Посчитать количество каждого различного символа и сравнить их с счетчиками этих символов второй строки
 ! Отсортировать по алфавитам и сравнить эти строки посимвольно

 ! Проходиться по каждому следующему символу и сверять с предыдущим, менять их местами в случае
 если следующий больше ([i] > [i+1])
*/

#include <iostream>
#include <cassert>
/*
int main(){
    char first[10][50] = {"god", "act", "master"};
    int first_count = 3
    char second[10][50] = {"dog", "cat", "stream"};
    int second_count = 3
    for (i = 0; i < first_count; i=i+1){

    }
}
*/


    // char ch = 'A';
    // std:: cout << int(ch) << std:: endl;
/* 
int ascii_value(char* word){
    int general_value = 0;
    for (int i = 0; i != '\0'; i = i+1){
        general_value = int(word[i])+general_value;
    
    }
    return general_value;
}
int main(){
    char slowo[50] = "dog";
    int val = ascii_value(slowo);
    std:: cout << val << std:: endl;
    return 0;
}
*/
int sortt(char* word){
    for (int i = 0; i != '\0'; i = i+1){
        word[i] = int(word[i]);
    }
    
}
int main(){
    char slowo[50] = "banana";
    char empty[50]
    for (int i = 0; slowo[i] != '\0'; i=i+1){ // если сравнивать просто i != '\0', то у нас цикл оборвётся моментально, тк '\0' это нулевой символ и с него начинается слово(и заканчивается тоже)
        empty[i] = int(slowo[i]);
        std:: cout << int(slowo[i]) << std:: endl;
        std:: cout << empty << std:: endl;
    }
    return 0;
}
