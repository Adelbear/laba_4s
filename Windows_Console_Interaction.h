#pragma once

#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include "Common.h"

//
// Функции для удобного взаимодействия с консолью Windows
//

using namespace std;

namespace std_usr {
// Внутреннее пространство имён с используемыми цветами
namespace console {
// Теги для обозначения области, которую нужно выделить цветом
// constexpr использован т.к. значения нужны уже на этапе компиляции
constexpr const char*
	color_left_mark = "<\a";
constexpr const char*
	color_right_mark = "\a>";
constexpr const char*
	color_correct_left = "[\a";
constexpr const char*
	color_correct_right = "\a]";


// 4-х битные коды цветов для вывода в консоль
const enum Colors {
	red		= FOREGROUND_RED,
	blue	= FOREGROUND_BLUE,
	green	= FOREGROUND_GREEN,
	dim		= FOREGROUND_INTENSITY,
	white	= red	| blue	| green,
	bright	= white	| dim,
	cyan	= blue	| green,
	magenta = red	| blue,
	yellow	= green	| red,
	black	= 0
};
} // end internal namespace

// Текущий установленный размер буфера консоли
COORD get_console_size();

// Получение текущих координат каретки
COORD get_cursor_position();

// Установка каретки в заданную позицию в консоли (not_exist значит, что по умолчанию y остаётся текущим)
void set_cursor_position(natural x, natural y = not_exist);

// Скрытие/вывод курсора
void set_cursor_visibility(bool is_visible);

// Передвижение каретки от текущего положения на заданное количество строчек и стобцов
void move_cursor(integer dx, integer dy = 0);

// Очистка текущей строки, начиная с start_x на count символов (-1 значит, что будет очищена вся строка)
void clear_raw(integer start_x = 0, integer count = -1);

// === Работа с цветом при выводе === 
// Цветной вывод в консоль
void color_print(const string& str, integer print_color = console::white);

// Создание пометки для выделения области цветом при печати
string cmark(const string str, natural local_color = not_exist);

// подсчёт числа непечатных помечающих символов в строке
natural count_garbadge(const string str);

// Цветной вывод помеченных подстрок
void highlight(const string& str, natural mark_color = console::white, natural base_color = console::white);

} // end namespace
