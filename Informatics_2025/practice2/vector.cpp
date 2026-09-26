#include <iostream>
#include <vector>
#include <string>
#include <chrono>

using namespace std;


auto nachalo = chrono::high_resolution_clock:: now();

void PrintVector(vector<string> words){
    for(auto s: words){
        cout << s;
    }
}


int main(){
    vector<string> v;
    string slovo;
    while(cin >> slovo){
        v.push_back(slovo);
    }
    
    PrintVector(v);
    
    auto end = chrono::high_resolution_clock:: now();
    cout << chrono::duration_cast<chrono::milliseconds>(end - nachalo).count() << " ms" << endl;
    return 0;

}


/*
int main(){
    int size;
    cout << "enter vector size:" << endl;
    cin >> size;
    vector<string> v(size); 
    cout << "vector before filling" << endl;
    for (string s : v) {
        cout << s << endl;
        }
    cout << "input vector elements:" << endl;
    for(string& s: v){ //без & будет создавать копии, с & обращается к оригиналу
        cin >> s;
    }
    cout << "vector after filling" << endl;
    for (string s : v) {
        cout << s << endl;
        }

    return 0;

}
*/