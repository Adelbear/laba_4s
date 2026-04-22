#include "Windows_File_Work.h"

//
// Функции для удобной и безопасной работы с файловой системой
//

namespace std_usr {

// Вывод всех названий файлов, содержащих параметр в названии
vector<string> filenames_take(const string criteria) {
	WIN32_FIND_DATAA
		file_data;
	HANDLE
		//Указатель на текущий файл из каталога
		file_path;
	string
		buf;
	vector<string>
		filenames = vector<string>();

	const char* lit = "sad";

	file_path = FindFirstFileA("*", &file_data);

	//Выбираем из всех названий файлов только подходящие под опр. критерий
	do {
		buf = file_data.cFileName;
		if (buf.find(criteria) != string::npos)
			filenames.push_back(buf);
	} while (FindNextFileA(file_path, &file_data));

	FindClose(file_path);
	return filenames;
}

// Считывание одного файла в строку
string file_read(const string filename) {
	ifstream
		file(filename);
	string
		output = string();

	if (file.is_open()) {
		getline(file, output,  '\0');
		file.close();
	}
	return output;
}

// Дозапись в файл
void file_write(const string filename, const string data) {
	ofstream
		file;

	file.open(filename, ios::app);

	if (file.is_open())	{
		file << data << endl;
		file.close();
	}
}
} // end namespace