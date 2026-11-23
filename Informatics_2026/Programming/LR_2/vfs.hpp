// vfs - Virtual File System
#pragma once

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

/*
• Объект — конкретный экземпляр класса в памяти с реальными данными
  (v1 и v2 — это два объекта разных типов).

• Полиморфизм — способность одной и той же функции или команды работать
  с объектами разных типов, выполняя действия под каждый тип.

• В C++ для разных типов вроде vector<string> и vector<int> это
  чаще всего делается через шаблоны (статический полиморфизм).

  //у нас динамический полиморфизм, но для справки полезно.
*/


namespace vfs { //пространство имён для отсуствия конфликтов vfs::File(если есть класс File в 
                //нашей библиотеке и в ещё одной)

class Entry {
public:
    virtual ~Entry() = default; //virtual значит виртуальный, т.е. здесь виртуальный деструктор для entry

    const std::string& name() const noexcept; //объявили метод(реализуем в cpp), который возвращает имя объекта класса Entry
    virtual std::size_t size() const noexcept = 0; //тоже самое, но метод возвращает размер
    virtual std::unique_ptr<Entry> clone() const = 0; //возвращаем unique_ptr на новую копию объекта, того же настоящего типа

protected:
    explicit Entry(std::string name); //создаёт запись с заданным именем; доступен только наследникам
    Entry(const Entry&) = default; //создаём дефолтный Entry
    Entry& operator=(const Entry&) = delete; //Запрещаем этот метод(нельзя присвоить объекту класса Entry значение другого объекта класса Entry)

private:
    std::string name_;
};

class File final : public Entry {// файл: запись с содержимым; final = наследовать от File нельзя
public:                          // то есть мы создали наследника, который сам не можем передавать наследуемые методы
    explicit File(std::string name,
                  std::string content = {});
    File(const File&) = default;

    const std::string& content() const noexcept; //const после скобок () значит, что метод не будет менять объект
    void write(std::string content); //ещё один прикол, метод безопасно вызывать где угодно, даже в деструкторе, тк он точно не прервёт его, выкинув ошибку

    std::size_t size() const noexcept override;
    std::unique_ptr<Entry> clone() const override;

private:
    std::string content_;  
};

class Directory final : public Entry { // каталог: запись, которая владеет другими записями
public:
    explicit Directory(std::string name);// создаёт пустой каталог
    Directory(const Directory& other);// глубокое копирование (дефолтное тут не подойдёт)

    void add(std::unique_ptr<Entry> entry);// принимает владение элементом; бросает при null/дубликате
    Entry* find(const std::string& name) noexcept;// непосредственный ребёнок или nullptr
    const Entry* find(const std::string& name) const noexcept; // то же для const-каталога
    bool remove(const std::string& name) noexcept;// удаляет ребёнка; true, если нашёл
    std::size_t count() const noexcept;// число непосредственных детей

    std::size_t size() const noexcept override;// сумма размеров всех файлов на любой глубине
    std::unique_ptr<Entry> clone() const override;// копия, которая тоже Directory

private:
    std::vector<std::unique_ptr<Entry>> children_; // дети; каталог единолично ими владеет
};

} // namespace vfs