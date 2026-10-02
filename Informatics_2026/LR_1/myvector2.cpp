#include <iostream>


template <typename T> //шаблонный тип данных

class myvector{
    public:
        //Можно создавать один и тот же конструктор,
        //комплиятор сам выбирает какой применить при создании объекта!!
        myvector(): data_(nullptr), size_(0), capacity_(0){} //конструктор с пустыми параметрами
        myvector(int size, int capacity): data_(new T[capacity]), size_(size), capacity_(capacity){} //конструктор с заданными параметрами
        // new оператор — резервирует ячейки в памяти и возвращает адрес первой ячейки.
        ~myvector(){
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
            if (size_ == capacity_){
                //тернарный оператор: условие ? A : B, то есть если условие == true, то выполняем действие A, иначе B
                // file://./media/ternary_operator.png
                reserve(capacity_ == 0 ? 1 : capacity_ * 2); 
            }
            data_[size_] = value;
            ++size_; //++x увеличивает x и отдаёт новое значение, а не старое как при x++(т.е. это префикс)

        }
        T& operator[](int i) { //если не написать эту магическую строку, то не сможем выводить i-й элемент массива
            return data_[i];
        }




        int size() const { //метод для проверки размера объекта(вектора)
            return size_;
        }
        int capacity() const { //метод для проверки вместимости объекта(вектора)
            return capacity_; //То есть возвращаем поле capacity_
        }

    private:
        T* data_;
        int size_;
        int capacity_;



};


int main() {

    myvector<int> v(2,3);
    v[0] = 14;
    v[1] = 67;
    std::cout << v[0];

    /* myvector<int> v;
    for (int i = 0; i < 6; ++i) {
        v.push_back(i * 10);
        std::cout << "size=" << v.size() << " capacity=" << v.capacity() << "\n";
    }
    std::cout << v[0] << " " << v[5] << "\n";
    */
}