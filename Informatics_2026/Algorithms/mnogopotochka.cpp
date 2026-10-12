#include <iostream>
#include <chrono>
#include <thread>
#include <log_duration.h>
#include <atomic>


using namespace std;

int counter = 0;

void big_num(){
    

}


void save_report(){
    this_thread::sleep_for(chrono::milliseconds(500));

}

void do_other_work(){
    this_thread::sleep_for(chrono::milliseconds(500));

}


int main(){
    LOG_DURATION("Multi thread");
    thread worker(big_num); //функцией worker создаем доп потоки
                                //если умирает основной поток, то умирают и дочерние
    big_num(); //главный поток
    worker.join();//присоединяем поток к процессу
    cout << counter << endl;


    return 0;
}