#pragma once

#include <Windows.h>
#include <string>
#include <vector>
#include <fstream>

//
// Функции для удобной и безопасной работы с файловой системой
//

using namespace std;

namespace std_usr {
// Вывод всех названий файлов, содержащих параметр в названии
vector<string> filenames_take(const string criteria);

// Считывание одного файла в строку
string file_read(const string filename);

// Дозапись в файл
void file_write(const string filename, const string data);
} // end namespace