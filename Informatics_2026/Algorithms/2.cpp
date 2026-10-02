#include <iostream>
#include <array>
#include <stdexcept>

using namespace std; 

int fibonacci(int n)
{
    
    if (n < 0 || n > 30)
    {


        throw out_of_range("Fibonacci number out of range");
    }
    

    if (n == 0)
    {
        return 0;
    }
    

    if (n == 1)
    {
        return 1;
    }
    

    return fibonacci(n - 1) + fibonacci(n - 2);
}



int main() 
{
    int n;

    cout << "Enter number n (from 0 to 30): ";
    cin >> n;


    try 
    {
        int result = fibonacci(n);
        cout << "Fib number" << n << " = " << result << endl; // Выводим результат
    } 
    catch (const out_of_range& e)
    {

        
        cout << "Error: " << e.what() << endl; 
    }

    return 0;
}