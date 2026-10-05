#pragma once
#include <stdexcept>

template <typename T> //шаблонный тип данных

class MyVector{
    public:
        //Можно создавать один и тот же конструктор,
        //комплиятор сам выбирает какой применить при создании объекта!!
        MyVector(): data_(nullptr), size_(0), capacity_(0){} //конструктор с пустыми параметрами
        explicit MyVector(size_t n, const T& value = T{}): data_(new T[n]), size_(n), capacity_(n) {
            for (size_t i = 0; i < n; ++i) {
                data_[i] = value;
            }
        } //конструктор с заданными параметрами
        // new оператор — резервирует ячейки в памяти и возвращает адрес первой ячейки.
        MyVector(const MyVector& other): data_(new T[other.size_]), size_(other.size_), capacity_(other.size_){
            // здесь myvecotr& other выступает в роли ссылки на оригинальный вектор
            for(int i = 0; i < size_; i++){
                data_[i] = other.data_[i];
            }
        }
        ~MyVector(){
            delete[] data_;
        }

        void reserve(int new_cap){
            if (new_cap <= capacity_) return; //return Здесь значит досрочный выход из reserve, так как new_cap меньше и выделять память не надо.

            T* new_data = new T[new_cap]; //выделение нового массива, если new_cap > capacity_
            for (int i = 0; i < size_; ++i){
                new_data[i] = data_[i];     // копирование старых элементов в new_data
            }
            delete[] data_; //освободили старый массив
            data_ = new_data; //теперь data_ это наша new_data, которую мы заполнили не до конца, ведь new_cap наверняка больше чем size_
            capacity_ = new_cap; //обновляем вместимость
        } //БАМ нахуй

        void push_back(const T& value){
            T copy = value;
            if (size_ == capacity_){ // push_back увеличит ёмкость в 2 раза, если size_ == capacity_
                                    //или выделит ёмкость под 1 элемент, если было 0, тк 0*2 = 0(оно сломает программу)
                //тернарный оператор: условие ? A : B, то есть если условие == true, то выполняем действие A, иначе B
                // file://./media/ternary_operator.png
                reserve(capacity_ == 0 ? 1 : capacity_ * 2); 
            }
            data_[size_] = copy; //в ячейке под номером size_ появляется элемент со значением value(шаблонный тип данных T позволяет засовывать char, string, int и т.д.)
            ++size_; //++x увеличивает x и отдаёт новое значение, а не старое как при x++(т.е. это префикс)
        }

         void pop_back() {
            if (empty()) throw std::out_of_range("pop_back: empty");
            --size_;
        }

        void clear() { size_ = 0; } // делаем вектор пустым, не освобождая(почему-то) память

        void erase(int pos) {
            if (pos < 0 || pos >= size_) throw std::out_of_range("erase: bad position");
            for (int i = pos; i < size_ - 1; ++i) {
                data_[i] = data_[i+1];     // сдвигаем элемент справа на место текущего
            }
            --size_;
        }
        
        void resize(int n, const T& value = T{}) {
            if (n < size_) {
                size_ = n;                    // обрезаем: просто уменьшаем размер
            } else if (n > size_) {
                T copy = value;
                reserve(n);                   // гарантируем, что места хватит
                for (int i = size_; i < n; ++i) {
                    data_[i] = copy;         // заполняем новые клетки
                }
                size_ = n;                    // теперь элементов ровно n
            }
            // если n == size_ — ничего не делаем
        }

        void insert(int pos, const T& value) {
            if (pos > size_) throw std::out_of_range("insert: bad position");
            T copy = value;

            if (size_ == capacity_) {
                reserve(capacity_ == 0 ? 1 : capacity_ * 2);  // нужно место под новый элемент
            }

            for (int i = size_; i > pos; --i) {
                data_[i] = data_[i - 1];     // сдвигаем элементы вправо
            }
            data_[pos] = copy;              // кладём value на освободившееся место
            ++size_;
        }


        T& operator[](int i) { //если не написать эту магическую строку, то не сможем выводить i-й элемент массива
            return data_[i];
        }

        MyVector& operator=(const MyVector& other) {
            if (this == &other) return *this;    // самоприсваивание: ничего не делаем

            T* new_data = new T[other.size_];            // сначала выделяем новый массив
            for (int i = 0; i < other.size_; ++i) {
                new_data[i] = other.data_[i];               // копируем элементы оригинала
            }
            delete[] data_;                        // освобождаем СТАРЫЙ массив
            data_ = new_data;                         // переключаемся на новый
            size_ = other.size_;
            capacity_ = other.size_;
            return *this;
        }
        T& at(int i) { //Да, это то же, что и operator, но без неопределенног поведения в случае выхода за границы массива
            if (i < 0 || i >= size_) throw std::out_of_range("at: index out of range"); //throw — резкий выход из функции
            return data_[i];
        }
        const T& operator[](int i) const { return data_[i]; } //ещё одна версия оператора(магии), но вызывается только для const векторов

        const T& at(int i) const {
            if (i < 0 || i >= size_) throw std::out_of_range("at: index out of range");
            return data_[i];
        }

        bool empty() const { return size_ == 0; }  //просто чекаем пустой массив или нет

        T& front() { //вернёт значение первого элемента
            if (empty()) throw std::out_of_range("front: empty");
            return data_[0];
        }
        const T& front() const {
            if (empty()) throw std::out_of_range("front: empty");
            return data_[0];
        }

        T& back() { // вернёт значение последнего элемента
            if (empty()) throw std::out_of_range("back: empty");
            return data_[size_-1];
        }
        const T& back() const { // вернёт значение последнего элемента
            if (empty()) throw std::out_of_range("back: empty");
            return data_[size_-1];
        }

        T* begin() { return data_; }       // указатель на первый элемент
        T* end()   { return data_ + size_; }       // указатель на «один за последним»
        // data_ + size_ сдвигает указатель на size_ элементов вперёд.
        // т.е. data_ — указатель на первый элемент а data_ + 1 указатель на второй элемент массива
        //data_ эквивалентен &data_[0]
        const T* begin() const { return data_; }
        const T* end()   const { return data_ + size_; }


        size_t size() const { //метод для проверки размера объекта(вектора)
            return size_;
        }
        size_t capacity() const { //метод для проверки вместимости объекта(вектора)
            return capacity_; //То есть возвращаем поле capacity_
        }

    private:
        T* data_;
        size_t size_;
        size_t capacity_;



};

