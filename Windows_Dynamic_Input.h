#pragma once

#include <conio.h>
#include <string>
#include <Windows.h>
#include <vector>
#include <map>
#include "Common.h"
#include "User_String_Addons.h"
#include "Windows_Console_Interaction.h"
#include "ASCII_Key_Indexes.h"

//
// Функция для посимвольного ввода с клавиатуры для анализа считываемых команд
//

using namespace std;

namespace std_usr {
// Внутреннее пространство имён
namespace input {
const enum Allowed_Dictionary {
	all_signs		= 0x1,
	only_command	= 0x2
};

// Переключатели для допустимых для ввода символов
const map<Allowed_Dictionary, string> dictionary{
	{only_command, string()},
	{all_signs, symbols_all + symbols_digits
				+ letters_en_big + letters_en_small
				+ letters_ru_cp1251_big + letters_ru_cp1251_small}
};

const natural
	// Максимальное число печатных символов в строке в фукнции getch_input
	default_vis_limit = 30,
	// Максимальное число символов, вводимых в строку
	default_str_limit = sizeof(char)*255;

const string
	str_width_lack = "НЕДОСТАТОЧНО ШИРИНЫ КОНСОЛИ";

// Список обрабатываемых команд, часть из которых обрабатывается вне функции
const enum Getch_Return_Value {
	GRV_nothing		= -1,
	GRV_enter		= KEY_ENTER,
	GRV_escape		= KEY_ESCAPE,
	GRV_tabulation	= KEY_TABULATION,
	GRV_F1			= KEY_F1,
	GRV_F2			= KEY_F2,
	GRV_F3			= KEY_F3,
	GRV_left		= KEY_LEFT,
	GRV_right		= KEY_RIGHT,
	GRV_up			= KEY_UP,
	GRV_down		= KEY_DOWN,
	GRV_delete		= KEY_DELETE,
	GRV_backspace	= KEY_BACKSPACE
};

// === Вспомогательные функции для функции getch_input ===
// Действия со строкой в соответвтие с коммандой
void getch_process(string& left_str, string& right_str, key_index symbol);

// Печать вводимой строки, корректное отображение строки
void getch_print(const string& left_str = string(), const string& right_str = string(), natural vis_limit = 0, natural start_x = 0);

} // end internal namespace

// Функция для работы с вводом строки
integer getch_input(string& output, const string prefix = string(),
				const input::Allowed_Dictionary allowed = input::all_signs,
				natural str_limit = input::default_str_limit,
				natural vis_limit = input::default_vis_limit);

} // end namespace