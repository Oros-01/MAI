#pragma once
#include <stdexcept>
#include "myvector.h"   // дека строится на двух MyVector

template <typename T> //шаблонный тип данных, как в MyVector

class MyDeque{
    // Внутреннее устройство:
    //   front_ — хранит начало деки В ОБРАТНОМ ПОРЯДКЕ.
    //            front_.back() — первый логический элемент деки.
    //   back_  — хранит конец деки В ПРЯМОМ ПОРЯДКЕ.
    //            back_.front() — элемент сразу после front_.back().
    //
    // Пример: дека [A, B, C, D, E]
    //   front_ = [B, A]   (в обратном порядке)
    //   back_  = [C, D, E] (в прямом порядке)
    //
    // Логический индекс i:
    //   если i < front_.size() → front_[front_.size() - 1 - i]
    //   иначе                  → back_[i - front_.size()]

    public:

        size_t size() const { //общее количество элементов в деке
            return front_.size() + back_.size();
        }

        bool empty() const { //дека пуста, если оба вектора пусты
            return size() == 0;
        }

        void push_front(const T& value){ //добавить копию value в начало
            // Если value — ссылка на элемент этой же деки,
            // push_back во front_ может перевыделить память и сломать ссылку.
            // Копируем заранее — как в MyVector::push_back.
            T copy = value;
            front_.push_back(copy); //в front_ обратный порядок,
                                    //поэтому «в начало деки» = «в конец front_»
        }

        void push_back(const T& value){ //добавить копию value в конец
            T copy = value;
            back_.push_back(copy); //в back_ прямой порядок,
                                   //поэтому просто push_back
        }

        void pop_front() { //удалить первый логический элемент
            if (empty()) throw std::out_of_range("pop_front: empty");

            if (front_.empty()) {
                // front_ пуст, а back_ нет — перекладываем ПОЛОВИНУ back_ во front_.
                // Берём первую половину back_ (по логике деки)
                // и кладём её в front_ так, чтобы порядок сохранился.
                size_t half = back_.size() / 2; //сколько перекладываем
                // Например, back_ = [C, D, E], half = 1.
                // Перекладываем C. Останется back_ = [D, E].
                // front_ станет [C] (в обратном порядке — но там один элемент, не важно).
                // Логически дека была [C, D, E], останется [C, D, E] — порядок сохранён.

                // Кладём первые half элементов back_ во front_ в обратном порядке.
                // back_[0] — это логически первый из back_, он должен стать
                // последним в front_ (т.к. front_ в обратном порядке).
                // Значит, идём с конца: сначала кладём back_[half-1], потом back_[half-2]...
                // Тогда front_.back() окажется back_[0] — верно.
                for (size_t i = half; i > 0; --i) {
                    front_.push_back(back_[i - 1]);
                }
                // Теперь front_ = [back_[half-1], back_[half-2], ..., back_[0]]
                // (в порядке добавления), и front_.back() = back_[0] — верно.

                // Удаляем первые half элементов из back_.
                // У MyVector нет erase диапазона, поэтому сдвигаем вручную:
                for (size_t i = 0; i + half < back_.size(); ++i) {
                    back_[i] = back_[i + half];
                }
                // Уменьшаем логический размер back_ на half.
                // resize в меньшую сторону у MyVector просто уменьшает size_.
                back_.resize(back_.size() - half);
            }

            front_.pop_back(); //front_.back() — первый логический элемент деки
        }

        void pop_back() { //удалить последний логический элемент
            if (empty()) throw std::out_of_range("pop_back: empty");

            if (back_.empty()) {
                // back_ пуст, а front_ нет — перекладываем ПОЛОВИНУ front_ в back_.
                // front_ хранит начало в обратном порядке:
                //   front_ = [B, A]  (логически [A, B])
                // Нам нужно взять ПОСЛЕДНЮЮ половину логической деки
                // и положить её в back_ в прямом порядке.
                //
                // Логически front_ = [front_.back(), ..., front_.front()]
                // Последние half логических элементов — это front_[0..half-1]
                // (в обратном порядке: front_[half-1] — самый «поздний» из них).
                size_t half = front_.size() / 2;
                // Например, front_ = [B, A] (логически [A, B]), half = 1.
                // Последний логический элемент — A = front_[1]. Хм, не front_[0].
                // Давай аккуратно: front_ = [B, A], логически [A, B].
                // front_[0] = B (логически второй), front_[1] = A (логически первый).
                // Последний логический элемент = front_[0] = B. Значит, для half=1
                // берём front_[0..0] = [B]. Перекладываем в back_ в прямом порядке:
                // back_ = [B]. Логически дека была [A, B], осталась [A, B]? Нет:
                // мы взяли B из front_, значит front_ должен стать [A] (один элемент).
                // Но половина — это 1 элемент из 2, а логически «последняя половина»
                // из [A, B] — это [B]. Тогда front_ = [A], back_ = [B].
                //
                // Общее правило: последние half логических элементов front_ —
                // это front_[0], front_[1], ..., front_[half-1] (в обратном логическом порядке).
                // Чтобы положить их в back_ в прямом порядке, идём от front_[half-1] к front_[0]:
                for (size_t i = half; i > 0; --i) {
                    back_.push_back(front_[i - 1]);
                }
                // Удаляем последние half логических элементов front_,
                // то есть первые half физических элементов front_ (индексы 0..half-1).
                // Сдвигаем оставшиеся влево:
                for (size_t i = 0; i + half < front_.size(); ++i) {
                    front_[i] = front_[i + half];
                }
                front_.resize(front_.size() - half);
                // Проверим на примере: front_ = [B, A], half = 1.
                // back_.push_back(front_[0]) = back_.push_back(B). back_ = [B].
                // Сдвиг: front_[0] = front_[1] = A. front_.resize(1) → front_ = [A].
                // Логически: front_ = [A] (один элемент, логически [A]),
                //           back_ = [B] (логически [B]).
                // Дека = [A, B] — верно, порядок сохранён. ✅
            }

            back_.pop_back(); //back_.back() — последний логический элемент деки
        }

        T& front() { //ссылка на первый логический элемент
            if (empty()) throw std::out_of_range("front: empty");
            // Если front_ непуст — первый логический элемент лежит в front_.back()
            if (!front_.empty()) return front_.back();
            // Иначе первый логический элемент — это back_.front()
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

        // Вспомогательный метод: по логическому индексу i вернуть ссылку на элемент.
        // Приватный, чтобы пользователь не вызывал его напрямую.
        // Используется в operator[] и at.
    private:
        T& element_at(size_t i) {
            if (i < front_.size()) {
                // Элемент лежит во front_, но в обратном порядке:
                // логический i-й = front_[front_.size() - 1 - i]
                return front_[front_.size() - 1 - i];
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

    public:
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
            front_.clear();
            back_.clear();
        }

    private:
        MyVector<T> front_; //начало деки в обратном порядке
        MyVector<T> back_;  //конец деки в прямом порядке
};