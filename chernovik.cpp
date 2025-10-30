#include <iostream>

int main(){
    int word_count = 2;
    int uniq_count = 2;
    char words[10][50] = {"meow", "banana"};
    char uniq_words[10][50] = {"meow", "banana"};
    for (int i = 0; i < word_count; i=i+1){
        bool word_found = false;
        for (int j = 0; j <uniq_count; j=j+1){
            for (int k = 0; words[i][k] != '\0'; k = k+1)
            if (words[i][k] == uniq_words[j][k]){
                word_found = true;
                std:: cout << "complete" << std:: endl;
            }
        }   
    }
return 0;
}