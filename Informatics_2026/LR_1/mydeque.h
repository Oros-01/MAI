#pragma once
#include <stdexcept>
#include "myvector.h"   // дека строится на двух MyVector

template <typename T> //шаблонный тип данных, как в MyVector

class MyDeque{
    public:
        MyDeque() = default; //создаём пустую деку

        MyDeque(const MyDeque& other)://создаём новую как копию другой
            front_(other.front_),
            back_(other.back_) {}

        MyDeque& operator=(const MyDeque& other) { //заменяем существующую другой(оператор=)
            if (this != &other) {                  //т.е. присваивание
                front_ = other.front_;
                back_  = other.back_;
            }
            return *this;//ничего не присваиваем, если одинаковые.
        }

        ~MyDeque() = default;


        size_t size() const { //общее количество элементов в деке
            return front_.size() + back_.size(); //просто считаем кол-во элементов обоих массивов в сумме
        }

        bool empty() const { //дека пуста, если оба вектора пусты
            return size() == 0;
        }

        void push_front(const T& value){ //value здесь ссылка на элемент, который добавляем
            T copy = value;
            front_.push_back(copy); //в front_ обратный порядок,
        }

        void push_back(const T& value){ //добавить копию value в конец
            T copy = value;
            back_.push_back(copy); //в back_ прямой порядок,
                                   //поэтому просто push_back
        }

        void pop_front() {
            if (empty()) throw std::out_of_range("pop_front: empty");

            // если в деке ровно один элемент, проверим оба вектора
            if (size() == 1) {
                if (!front_.empty()) {
                    front_.pop_back();
                } else {
                    back_.pop_back();
                }
                return;//досрочно выйдет, break выдаст ошибку компилятора
            }

            if (front_.empty()) {
                size_t half = back_.size() / 2;
                for (size_t i = half; i > 0; --i) {
                    front_.push_back(back_[i - 1]);
                }
                for (size_t i = 0; i + half < back_.size(); ++i) {
                    back_[i] = back_[i + half];
                }
                back_.resize(back_.size() - half);
            }

            front_.pop_back();
        }

        void pop_back() {
            if (empty()) throw std::out_of_range("pop_back: empty");

            // также как в pop_front чекаем и в back_ и в front_
            if (size() == 1) {
                if (!back_.empty()) {
                    back_.pop_back();
                } else {
                    front_.pop_back();
                }
                return;
            }

            if (back_.empty()) {
                size_t half = front_.size() / 2;
                for (size_t i = half; i > 0; --i) {
                    back_.push_back(front_[i - 1]);
                }
                for (size_t i = 0; i + half < front_.size(); ++i) {
                    front_[i] = front_[i + half];
                }
                front_.resize(front_.size() - half);
            }

            back_.pop_back();
        }


        T& front() { //ссылка на первый логический элемент
            if (empty()) throw std::out_of_range("front: empty");
            // если front_ непуст — первый логический элемент лежит в front_.back()
            if (!front_.empty()) return front_.back(); //ВНИМАНИЕ! Конкретно последний элемент вектора front_
                                                        //т.е. метод из myvector.h
            return back_.front();
        }

        const T& front() const { //то же для const-деки
            if (empty()) throw std::out_of_range("front: empty");
            if (!front_.empty()) return front_.back();
            return back_.front();
        }

        T& back() { //ссылка на последний логический элемент
            if (empty()) throw std::out_of_range("back: empty");
            // Если back_ непуст — последний логический элемент лежит в back_.back()
            if (!back_.empty()) return back_.back();
            // Иначе последний логический элемент — это front_.front()
            return front_.front();
        }

        const T& back() const {
            if (empty()) throw std::out_of_range("back: empty");
            if (!back_.empty()) return back_.back();
            return front_.front();
        }


        
        T& operator[](size_t i) { //доступ без проверки границ
            return element_at(i);
        }

        const T& operator[](size_t i) const {
            return element_at(i);
        }

        T& at(size_t i) { //доступ с проверкой границ
            if (i >= size()) throw std::out_of_range("at: index out of range");
            return element_at(i);
        }

        const T& at(size_t i) const {
            if (i >= size()) throw std::out_of_range("at: index out of range");
            return element_at(i);
        }

        void clear() { //сделать деку пустой, сохранив вместимость векторов
            front_.clear(); //capacity_ сохранится
            back_.clear();
        }

    private:
        MyVector<T> front_; //начало деки в обратном порядке
        MyVector<T> back_;  //конец деки в прямом порядке

        /*
        front_ = {1, 2, 3, 4}
        back_  = {6, 7, 8, 9}
        тогда:
        front_ читается задом наперёд: то есть в деке front_ =  {4, 3, 2, 1};
        back_ читается как есть: то есть в деке back_ = {6, 7, 8, 9}.
        значит вся дека
        mydeka = {4, 3, 2, 1, 6, 7, 8, 9}
        */

        //не для пользователей библиотеки.
        T& element_at(size_t i) {
            if (i < front_.size()) { //если условие не выполнилось, то элемент лежит в back_
                // элемент лежит во front_, но в обратном порядке:
                // логический i-й = front_[front_.size() - 1 - i]
                return front_[front_.size() - 1 - i]; //
            } else {
                // Элемент лежит в back_ в прямом порядке:
                // логический i-й = back_[i - front_.size()]
                return back_[i - front_.size()];
            }
        }

        const T& element_at(size_t i) const {
            if (i < front_.size()) {
                return front_[front_.size() - 1 - i];
            } else {
                return back_[i - front_.size()];
            }
        }
};