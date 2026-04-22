#include "Windows_Console_Interaction.h"

//
// Функции для удобного взаимодействия с консолью Windows
//

using namespace std_usr::console;

namespace std_usr {
// Текущий установленный размер буфера консоли
COORD get_console_size() {
	HANDLE
		handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO
		console_info;

	GetConsoleScreenBufferInfo(handle, &console_info);

	return console_info.dwSize;
}

// Получение текущих координат каретки
COORD get_cursor_position() {
	HANDLE
		handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO
		console_info;

	GetConsoleScreenBufferInfo(handle, &console_info);

	return console_info.dwCursorPosition;
}

// Очистка текущей строки, начиная с start_x на count символов (-1 значит, что будет очищена вся строка)
void clear_raw(integer start_x, integer count) {
	HANDLE
		handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO
		console_info;

	GetConsoleScreenBufferInfo(handle, &console_info);

	set_cursor_position(start_x);

	// Предел очистки по умолчанию - размер консоли по X
	if (count <= 0)
		count = console_info.dwSize.X;

	cout << string(count, ' ');

	set_cursor_position(start_x);
}

// Установка каретки в заданную позицию в консоли
void set_cursor_position(natural x, natural y) {
	HANDLE
		handle = GetStdHandle(STD_OUTPUT_HANDLE);

	COORD
		new_position;

	new_position.X = x;
	// По умолчанию y = not_exist, что значит, что он меняться не будет
	if (y != not_exist)
		new_position.Y = y;
	else
		new_position.Y = get_cursor_position().Y;

	SetConsoleCursorPosition(handle, new_position);
	
}

// Передвижение каретки от текущего положения на заданное количество строчек и стобцов (удобнее, чем через set)
void move_cursor(integer dx, integer dy) {
	if (dx != 0 || dy != 0)
		set_cursor_position(dx + get_cursor_position().X,
							dy + get_cursor_position().Y);
}

// Скрытие/вывод курсора
void set_cursor_visibility(bool is_visible) {
	// ANSI escape-последовательности режима видимости курсора
	if (is_visible)
		cout << "\x1b[?25h";
	else
		cout << "\x1b[?25l";
}

// Создание пометки для выделения области цветом при печати
string cmark(const string str, natural local_color) {
	string
		output = console::color_left_mark;

	if (local_color != not_exist) {
		output += console::color_correct_left;
		output += to_string(local_color);
		output += console::color_correct_right;
	}
	output += str;
	output += console::color_right_mark;
	return output;
}

// подсчёт числа непечатных символов в строке
natural count_garbadge(const string str) {
	natural
		color_tag_quantity = 0,
		garbadge = 0;

	if (str.find(color_left_mark) != not_found) {
		// Подсчёт числа выделенных учатсков
		for (natural i = 0; i < str.size();) {
			color_tag_quantity++;
			i = str.find(color_left_mark, i);
			if (i != not_found)
				i = str.find(color_left_mark, i + strlen(color_left_mark));
		}

		garbadge = strlen(color_left_mark) * color_tag_quantity;
		garbadge += strlen(color_right_mark) * color_tag_quantity;
		// подсчёт числа выделенных участков с уточнённым цветом
		if (str.find(color_correct_left) != not_found) {
			for (natural i = 0; i < str.size();) {
				i = str.find(color_correct_left, i);
				if (i != not_found) {
					garbadge += str.substr(i, str.find(color_correct_right, i) - i).length();
					garbadge += strlen(color_correct_right);
					i = str.find(color_correct_right, i);
				}
			}
		}
	}

	return garbadge;
}

// Цветной вывод помеченных подстрок
void highlight(const string& str, natural mark_color, natural base_color) {
	natural
		local_color = mark_color,
		left_index = str.find(color_left_mark),
		right_index = 0;
	string
		not_marked,
		// Помеченная для покраски подстрока
		marked;

	// Для случая подсветки всей строки
	while (left_index != not_found) {
		not_marked = str.substr(right_index, left_index - right_index);

		color_print(not_marked, base_color);

		right_index = str.find(color_right_mark, left_index) + strlen(color_left_mark);

		marked = str.substr(left_index, right_index - left_index);
		// Удаление пометок
		marked.erase(0, marked.find(color_left_mark) + strlen(color_left_mark));
		marked.erase(marked.rfind(color_right_mark));

		// В случае если проставлено усточнение цвета
		if (marked.find(color_correct_left) != not_found) {
			marked.erase(0, marked.find(color_correct_left) + strlen(color_left_mark));
			local_color = atoi(marked.substr(0, marked.find(color_correct_right)).c_str());
			marked.erase(0, marked.find(color_correct_right) + strlen(color_correct_right));
		}
		else
			local_color = mark_color;

		color_print(marked, local_color);

		left_index = str.find(color_left_mark, right_index);
	};
	// Допечатывание оставшейся незакрашенной строки
	color_print(str.substr(right_index), base_color);
}

// Цветной вывод в консоль
void color_print(const string& str, integer print_color) {
	HANDLE
		handle = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleTextAttribute(handle, print_color);
	cout << str;
	SetConsoleTextAttribute(handle, white);
}
} // end namespace