#include <iostream> 
#include <fstream>     // ifstream — чтение файла с результатами dir
#include <string>
#include <algorithm>   // toLower(строку к нижнему регистру)
#include <cstdlib>     // system() — запуск команд CMD

using namespace std;


string toLower(string str) {
    transform(str.begin(), str.end(), str.begin(), [](unsigned char c) { return tolower(c); });
    return str;
}



// Извлечь расширение(extension) из имени файла
string getExt(const string& path) { 

    size_t dotPos = path.rfind('.');     // rfind ищет последнее вхождение символа с конца строки
    if (dotPos == string::npos) return "";          // точки нет — нет расширения
    return toLower(path.substr(dotPos));
}



// Принимает байты, возвращает строку вида "2.34 KB"
string formatSize(long long bytes) {
    const char* suffixes[] = { "B", "KB", "MB", "GB" };
    int index = 0;
    double size = static_cast<double>(bytes);

    while (size >= 1024.0 && index < 3) {
        size /= 1024.0;
        index++;
    }

    // Форматируем вручную через целую и дробную часть
    long long whole = static_cast<long long>(size);
    long long frac  = static_cast<long long>((size - whole) * 100);
    if (frac < 0) frac = -frac;

    string result = to_string(whole) + "." +
                    (frac < 10 ? "0" : "") + to_string(frac) +
                    " " + suffixes[index];
    return result;
}







int main() {
    string inputPath;  // путь к папке для поиска
    string inputExt;   // расширение файла


    cout << "Enter PATH for browse: ";
    getline(cin, inputPath);



    cout << "Enter extenstion (for exapmle, cpp): ";
    getline(cin, inputExt);

    // --- Нормализация расширения ---
    // Если пользователь ввёл ".cpp" — убираем точку
    if (!inputExt.empty() && inputExt[0] == '.') {
        inputExt = inputExt.substr(1);
    }
    string targetExt = "." + toLower(inputExt);  // итог: ".cpp", ".txt" и тд


    // Открываем папку через dir — если вернёт не 0, путь неверный
    // Временно перенаправляем вывод в NUL чтобы не засорять консоль
    string checkCmd = "dir \"" + inputPath + "\" > nul 2>&1";
    if (system(checkCmd.c_str()) != 0) {
        cerr << "Error: PATH not exist or dont have permission: " << inputPath << endl;
        return 1;
    }

    // /S — рекурсивно обходит все подпапки
    // /B — bare format: только полные пути, без лишней информации
    // > result.txt — перенаправляем вывод в файл
    string dirCmd = "dir \"" + inputPath + "\" /S /B > result.txt 2>&1";

    cout << "File searching with extension " << targetExt << " ..." << endl;

    // system() передаёт строку командному процессору cmd.exe (слайд 3)
    system(dirCmd.c_str());

    // --- Читаем result.txt и фильтруем по расширению ---
    // ifstream — поток для чтения файла
    ifstream f("result.txt");
    if (!f.is_open()) {
        cerr << "Error: couldn't open temporary file result.txt" << endl;
        return 1;
    }

    int count = 0;        // счётчик найденных файлов
    string line;          // текущая строка из файла

    cout << "Found files:" << endl;

    // getline читает файл построчно — каждая строка это полный путь к файлу
    while (getline(f, line)) {
        // Пропускаем пустые строки и сообщения об ошибках доступа
        if (line.empty() || line.find("permission denied") != string::npos ||
            line.find("Access") != string::npos) {
            continue;
        }

        // Проверякм расширение текущего файла
        if (getExt(line) == targetExt) {
            // Получаем размер файла через отдельный вызов ifstream
            long long fileSize = 0;
            ifstream testFile(line, ios::binary | ios::ate); // открываем в бинарном режиме и встаём в конец
            if (testFile.is_open()) {
                fileSize = testFile.tellg();  // tellg в позиции ate = размер файла
                testFile.close();
            }

            cout << "  " << line << " (" << formatSize(fileSize) << ")" << endl;
            count++;
        }
    }

    f.close();

    // Удаляем временный файл — он больше не нужен
    system("del result.txt > nul 2>&1");

    // --- Итог ---
    if (count == 0) {
        cout << "Files with extension " << targetExt << " not found." << endl;
    } else {
        cout << "Total found" << count << " files." << endl;
    }

    return 0;
}