#pragma once

#include <conio.h>
#include <vector>
#include <string>
#include <list>
#include "Common.h"
#include "User_String_Addons.h"
#include "ASCII_Key_Indexes.h"
#include "Windows_Console_Interaction.h"

//
// Функция для вывода на экран многоуровнего меню с возможностью переключения нескольких пунктов меню зараз
//

using namespace std;

namespace std_usr {
// Внутреннее пространство имён
namespace menu {
const string
	sub_option_tab{"  "}, // {'\t'},
	one_space{ ' ' },
	sub_option_mark = "¤", //"->",
	menu_pointer = " >> ",
	option_on = "[+]",
	option_off = "[-]",
	default_stop_word = "[ Завершить (Esc) ]";

enum Option_States {
	off = 0,
	on = 1
};

// Хранение всех необходимых данных об одном пункте многомерного меню
struct Menu_Item {
	natural
		// Состояние пункта меню (в текущей реализации только вкл/выкл)
		is_turned,
		// Номер родительского пункта меню
		parent,
		// Уровень подменю
		level;
	string
		// Значащее содержание пункта меню
		name;

	Menu_Item();
	Menu_Item(string name, natural is_turned = on, natural head = not_found, natural level = 0);
	~Menu_Item();
};
} // end internal namespace


class Menu_Store {
private:
	vector<menu::Menu_Item>
		options_store;

	// True, если other_option является дочерним пунктом head_option
	bool is_wired(natural head_option, natural other_option);

	// Вывод родительского пункта данного подменю (при наличии)
	menu::Menu_Item parent(natural option_number);
public:
	// Переключение пункта (вкл/выкл)
	void change_state(natural option_number);

	// Добавление пункта
	void add(const string name, const string head_name = string(), natural is_turned = menu::on);

	// Вывод структуры пункта меню
	menu::Menu_Item at(natural option_number);

	// Вывод числа пунктов меню
	natural size();

	Menu_Store();
	// Ручная инициализация через перечисление пунктов
	Menu_Store(initializer_list<menu::Menu_Item> list);
};

// Отрисовка пунктов меню, их значений и состояний
void print_leveled_menu(Menu_Store& options, natural current, const string exit_option);

// Динамическое меню с возможностью изменять состояние пунктов, если опция выхода - пустая строка - считается, что можно нажать только 1 раз
string multilevel_menu(const Menu_Store* options, const string exit_option = string());
string multilevel_menu(Menu_Store& options, const string exit_option = menu::default_stop_word);
} // end namespace