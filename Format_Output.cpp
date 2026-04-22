#define _CRT_SECURE_NO_WARNINGS
#include "Format_Output.h"

//
// Формирование строк для табличного вывода информации
//

using namespace std_usr::format;
using namespace std_usr::console;

namespace std_usr {
// Формирование строки со значениями, отгорожденными друг от друга и выровненными посередине
string form_table_row(const string one_row, natural cell_quantity, bool to_mark, natural cell_width) {
	string
		current_field,
		output;

	for (int i = 0; i < cell_quantity; i++) {
		current_field = element_at(one_row, i);
		if (current_field.empty())
			if (to_mark)
				current_field = cmark(absent_data, console::dim);
			else
				current_field = absent_data;

		output += boundary_vertical;
		if (current_field.size() - count_garbadge(current_field) < cell_width - space_width) {
			output += string(space_width / 2, ' ');
			output += current_field;
			output += string(cell_width + count_garbadge(current_field) - current_field.length() - space_width / 2, ' ');
		}
		else
			output += current_field;
	}
	output += boundary_vertical;

	return output;
}

// Формирование горизонтальной разделительной строки с перекркестиями
string form_delim_row(natural cell_quantity, natural cell_width) {
	string
		output = string();
	for (cell_quantity; cell_quantity > 0; cell_quantity--) {
		output += boundary_intersection;
		output += string(cell_width, '-');
	}
	output += boundary_intersection;
	return output;
}

// Преобразования текущего времени в секундах в дату string
string time_str(natural options, natural time_factor) {
	time_t
		current_time = time(NULL);
	struct tm
		* now = localtime(&current_time);
	string
		output = string();

	// Текущая дата в формате ДД.ММ.ГГ
	if (options & day) {
		if (now->tm_mday < 10)
			output += "0";
		output += to_string(now->tm_mday);
	}
	if (options & month) {
		if (output.size())
			output += ".";
		if (now->tm_mon + 1 < 10)
			output += "0";
		output += to_string(now->tm_mon + 1);
	}
	if (options & year) {
		if (output.size())
			output += ".";
		if (now->tm_year % 100 < 10)
			output += "0";
		output += to_string(now->tm_year % 100);
	}

	// Добавка времени в формате _ЧЧ-ММ-СС
	if (options & hour) {
		if (output.size())
			output += "_";
		if (now->tm_hour < 10)
			output += "0";
		output += to_string(now->tm_hour);
	}
	if (options & format::min) {
		if (output.size())
			output += "-";
		if (now->tm_min < 10)
			output += "0";
		output += to_string(now->tm_min);
	}
	if (options & sec) {
		if (output.size())
			output += "-";
		if (now->tm_sec < 10)
			output += "0";
		output += to_string(now->tm_sec);
	}
	return output;
}
} // end namespace