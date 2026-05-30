#include <iostream> 
#include <fstream>     // ifstream — чтение файла с результатами dir
#include <string>      // string
#include <algorithm>   // transform — для toLower
#include <cstdlib>     // system() — запуск команд CMD

using namespace std;


string toLower(string str) {
    transform(str.begin(), str.end(), str.begin(),
              [](unsigned char c) { return tolower(c); });
    return str;
}

// -------------------------------------------------------
// Функция: извлечь расширение из имени файла
// Например: "main.CPP" -> ".cpp"
// -------------------------------------------------------
string getExt(const string& path) {
    // rfind ищет последнее вхождение символа с конца строки
    size_t dotPos = path.rfind('.');
    if (dotPos == string::npos) return "";          // точки нет — нет расширения
    return toLower(path.substr(dotPos));            // берём от точки до конца
}

// -------------------------------------------------------
// Функция: форматировать размер файла
// Принимает байты, возвращает строку вида "2.34 KB"
// -------------------------------------------------------
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

// -------------------------------------------------------
// Основная функция программы
// -------------------------------------------------------
int main() {
    string inputPath;  // путь к папке для поиска
    string inputExt;   // расширение файла

    // --- Запрос пути ---
    cout << "Enter PATH for browse: ";
    getline(cin, inputPath);

    // --- Запрос расширения ---
    cout << "Enter extenstion (for exapmle, cpp): ";
    getline(cin, inputExt);

    // --- Нормализация расширения ---
    // Если пользователь ввёл ".cpp" — убираем точку
    if (!inputExt.empty() && inputExt[0] == '.') {
        inputExt = inputExt.substr(1);
    }
    string targetExt = "." + toLower(inputExt);  // итог: ".cpp", ".txt" и т.д.

    // --- Проверка пути ---
    // Пробуем открыть папку через dir — если вернёт не 0, путь неверный
    // Временно перенаправляем вывод в NUL чтобы не засорять консоль
    string checkCmd = "dir \"" + inputPath + "\" > nul 2>&1";
    if (system(checkCmd.c_str()) != 0) {
        cerr << "Error: PATH not exist or dont have permission: " << inputPath << endl;
        return 1;
    }

    // --- Запуск dir /S /B — рекурсивный список всех файлов ---
    // /S — рекурсивно обходит все подпапки (слайд 5)
    // /B — bare format: только полные пути, без лишней информации (слайд 5)
    // > result.txt — перенаправляем вывод в файл (слайд 11)
    string dirCmd = "dir \"" + inputPath + "\" /S /B > result.txt 2>&1";

    cout << "\nПоиск файлов с расширением " << targetExt << " ...\n\n";

    // system() передаёт строку командному процессору cmd.exe (слайд 3)
    system(dirCmd.c_str());

    // --- Читаем result.txt и фильтруем по расширению ---
    // ifstream — поток для чтения файла (слайд 12)
    ifstream f("result.txt");
    if (!f.is_open()) {
        cerr << "Error: couldn't open temporary file result.txt\n";
        return 1;
    }

    int count = 0;        // счётчик найденных файлов
    string line;          // текущая строка из файла

    cout << "Found files:\n";

    // getline читает файл построчно — каждая строка это полный путь к файлу (слайд 12)
    while (getline(f, line)) {
        // Пропускаем пустые строки и сообщения об ошибках доступа
        if (line.empty() || line.find("Отказано") != string::npos ||
            line.find("Access") != string::npos) {
            continue;
        }

        // Проверяем расширение текущего файла
        if (getExt(line) == targetExt) {
            // Получаем размер файла через отдельный вызов ifstream
            long long fileSize = 0;
            ifstream testFile(line, ios::binary | ios::ate);
            if (testFile.is_open()) {
                fileSize = testFile.tellg();  // tellg в позиции ate = размер файла
                testFile.close();
            }

            cout << "  " << line << " (" << formatSize(fileSize) << ")\n";
            count++;
        }
    }

    f.close();

    // Удаляем временный файл — он больше не нужен
    system("del result.txt > nul 2>&1");

    // --- Итог ---
    if (count == 0) {
        cout << "Files with extension " << targetExt << " not found.\n";
    } else {
        cout << "\nTotal found" << count << " файл(а/ов).\n";
    }

    return 0;
}