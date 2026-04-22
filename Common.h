#pragma once
#include <string>

//
// Константы, типы данных, применяемые в модулях
//

using namespace std;

// Единое пространство имён для подпрограмм
namespace std_usr {

// Тип данных для работы с неотрицательными целыми числами
// Нужен для совместимости со многими стандартными функциями и методами
typedef size_t natural;
// Целочисленный тип данных, указанный для универализации типов по программе
typedef int integer;

const string
	// Строка для конечного вывода для наглядного уазания на отсутствие значения в поле
	absent_data = "[ Н/Д ]",
	// Разделители лексем
	separator_line = "\n",
	separator_word = ";",
	// Ниже словари (методы string не работают с кириллицей)
	letters_ru_cp1251_small = "абвгдеёжзийклмнопрстуфхцчшщъыьэюя",
	letters_ru_cp1251_big = "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ",
	letters_en_small = "abcdefghijklmnopqrstuvwxyz",
	letters_en_big = "ABCDEFGHIJKLMNOPQRSTUVWXYZ",
	symbols_digits = "0123456789",
	symbols_all = "`~!@#№$%&_=+-*^/()[{ }]\\|;:'\"<,.>?";

const natural
	// Значение, информирующее об отсуствии содержательной информации в переменной
	not_exist = -1,
	// Аналогичное значение, показывающее, что поиск не имеет положительных результатов
	not_found = std::string::npos;
} // end namespace