#include <iostream>
using namespace std;

double inputMeasurements(int k) { 
    double *temperatures = new double[k]; // тип данных массива *название переменной-указателя, (new)выделение памяти, тип элементов(кол-во элементов)
    //заполняем массив измерениями температуры
    for (int i = 0; i < k; i++) {
        cin >> temperatures[i];
    }
    
    double sum = 0.0;
    for (int i = 0; i < k; i++) {
        sum += temperatures[i];
    }
    double average = sum / k; // возвращает среднюю температуру
    
    delete[] temperatures; //освобождаем массив от того, чем заполнили.
    
    return average;
}

double* computeDailyAverages(int days) {
    double* daily_averages = new double[days];
    
    // для каждого дня смотрим кол-во измерений
    for (int i = 0; i < days; i++) {
        int k;
        cout << i + 1 << ":" << endl;
        cin >> k;
        daily_averages[i] = inputMeasurements(k);
    }
    
    return daily_averages;
}


int main(){

    return 0;
}



/* Авторское решение
#include <iostream>
#include <iomanip>
using namespace std;

// Функция ввода измерений одного дня и возврата среднего
double inputMeasurements() {
    int K;

    cin >> K;
    
    double* measurements = new double[K];
    for (int i = 0; i < K; ++i) {
        cin >> measurements[i];
    }
    
    double sum = 0.0;
    for (int i = 0; i < K; ++i) {
        sum += measurements[i];
    }
    double avg = sum / K;
    
    delete[] measurements;
    return avg;
}

// Основной цикл по дням: заполнение массива средних
double* computeDailyAverages(int days) {
    double* dailyAverages = new double[days];
    for (int i = 0; i < days; ++i) {
        dailyAverages[i] = inputMeasurements();
    }
    return dailyAverages;
}

// Вывод массива с заголовком (два знака после запятой)
void printArray(const double* arr, int size, const char* title) {
    cout << title << ": ";
    cout << fixed << setprecision(2) << arr[0];
    for (int i = 1; i < size; ++i) {
        cout << " " << fixed << setprecision(2) << arr[i];
    }
    cout << endl;
}

// Вычисление массива изменений (разностей между соседними средними)
double* computeDeltas(const double* arr, int size, int& deltaSize) {
    if (size < 2) {
        deltaSize = 0;
        return nullptr;
    }
    deltaSize = size - 1;
    double* deltas = new double[deltaSize];
    for (int i = 0; i < deltaSize; ++i) {
        deltas[i] = arr[i+1] - arr[i];
    }
    return deltas;
}

int main() {
    int D;
    cin >> D;
        
    double* dailyAverages = computeDailyAverages(D);
    
    printArray(dailyAverages, D, "Средние температуры по дням");
    
    int deltaSize;
    double* deltas = computeDeltas(dailyAverages, D, deltaSize);
    if (deltaSize > 0) {
        printArray(deltas, deltaSize, "Изменения между днями (deltas)");
    }
    
    delete[] dailyAverages;
    delete[] deltas;
    
    return 0;
}*/