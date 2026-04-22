#include "Multilevel_Dynamic_Menu.h"

//
// Функция для вывода на экран многоуровнего меню с возможностью переключения нескольких пунктов меню зараз
//

using namespace std_usr;
using namespace menu;

namespace std_usr {
Menu_Item::Menu_Item() {
	name = string();
	parent = not_exist;
	is_turned = on;
	level = 0;
}

Menu_Item::~Menu_Item() { }

Menu_Item::Menu_Item(string name, natural is_turned, natural parent, natural level) {
	this->name = name;
	this->parent = parent;
	this->is_turned = is_turned;
	if (parent != not_exist)
		this->level = level;
	else
		this->level = 0;
}

// True, если other_option является дочерним пунктом head_option
bool Menu_Store::is_wired(natural head_option, natural other_option) {
	integer
		level_difference = (integer)at(other_option).level - at(head_option).level;
	bool
		result = false;
	Menu_Item
		option_higher = at(other_option);

	if (head_option < size() and other_option < size()) {
		if (level_difference > 0) {
			for (int i = 0; i < level_difference; i++)
				option_higher = at(option_higher.parent);

			// Проверка, что пункт, найденный при подъёме является искомым родительским
			if (option_higher.name == at(head_option).name
				and option_higher.parent == at(head_option).parent)
				result = true;
		}
	}
	return result;
}

// Вывод числа пунктов меню
natural Menu_Store::size() {
	return options_store.size();
}

// Вывод родительского пункта данного подменю (при наличии)
Menu_Item Menu_Store::parent(natural option_number) {
	Menu_Item
		option_head = Menu_Item();
	natural
		head_number;
	if (option_number < size()) {
		head_number = options_store.at(option_number).parent;
		if (head_number < size())
			option_head = options_store.at(head_number);
	}
	return option_head;
}

// Ручная инициализация через перечисление пунктов
Menu_Store::Menu_Store() {
	options_store = vector<Menu_Item>();
}

// Добавление ещё одного пункта меню (если к родительскому пункту с подменю, то сразу после последнего подменю)
void Menu_Store::add(const string name, const string head_name, natural is_turned) {
	natural
		new_level = 0,
		insert_index = size() - 1,
		head_index = insert_index;

	if (head_name.empty())
		options_store.push_back({ name, is_turned, not_exist, new_level });
	// Случай добавления подменю
	else {
		// Переборка до первого подменю родительского пункта, или до него самого
		while (parent(insert_index).name != head_name
			   and at(insert_index).name != head_name
			   and insert_index != not_exist)
			insert_index--;

		// Добавление только если родительский пункт найден
		if (insert_index != not_exist) {
			head_index = insert_index;
			if (at(head_index).name != head_name)
				while (at(head_index).name != head_name)
					head_index--;

			new_level = at(head_index).level + 1;
			insert_index++;
			options_store.insert(options_store.begin() + insert_index,
				// Инициализация подменю
				Menu_Item(name, is_turned, head_index, new_level));
		}
		// Смещение указателя на родительские пункты, которые съехали вниз
		for (natural i = insert_index; i < size() - 1; i++) {
			Menu_Item
				current = at(i);
			if (current.parent != not_exist)
				if (current.parent >= insert_index)
					options_store.at(i).parent++;
		}
	}
}

// Ручная инициализация через перечисление
Menu_Store::Menu_Store(initializer_list<Menu_Item> list) {
	for (Menu_Item p : list)
		options_store.push_back(p);
}

// Переключение пункта (вкл/выкл)
void Menu_Store::change_state(natural option_number) {
	Menu_Item
		current_option;
	natural
		chosed_option = option_number,
		iterator_down = option_number + 1,
		iterator_up = option_number,
		options_on_level,
		identical_on_level = 0;

	if (option_number < size()) {
		// Переключение самого пункта
		options_store.at(chosed_option).is_turned ^= on;

		// Переключение всех подменю (по списку всегда ниже)
		for (iterator_down; iterator_down < size(); iterator_down++)
			if (is_wired(chosed_option, iterator_down)) {
				if (at(iterator_down).is_turned != at(chosed_option).is_turned)
					options_store.at(iterator_down).is_turned ^= on;
			}
			else
				break;

		// Изменение состояния родительских пунктов
		current_option = at(iterator_up);
		while (current_option.parent != not_exist) {
			identical_on_level = 0;
			options_on_level = 0;

			if (parent(iterator_up).is_turned != current_option.is_turned) {
				// Если пункт включался, то переключение родительского без проверки
				if (not current_option.is_turned)
					// Перебор с родительского пункта
					for (natural i = current_option.parent; i < size(); i++)
						if (current_option.parent == at(i).parent) {
							options_on_level++;
							if (current_option.is_turned == at(i).is_turned)
								identical_on_level++;
							else
								break;
						}
				// Также проходит если родительский был выключен, а подменю включено
				if (options_on_level == identical_on_level)
					options_store.at(current_option.parent).is_turned ^= on;
			}
			else
				break;
			iterator_up = current_option.parent;
			current_option = at(iterator_up);
		}
	}
}

// Вывод структуры пункта меню
Menu_Item Menu_Store::at(natural option_number) {
	if (option_number < this->size())
		return options_store.at(option_number);
	else
		return Menu_Item();
}

// Динамическое меню с возможностью изменять состояние пунктов, если опция выхода - пустая строка - считается, что можно нажать только 1 раз
string multilevel_menu(const Menu_Store* options, const string exit_option) {
	Menu_Store
		buffer = *options;
	return multilevel_menu(buffer, exit_option);
}

// Динамическое меню с возможностью изменять состояние пунктов, если опция выхода - пустая строка - считается, что можно нажать только 1 раз
string multilevel_menu(Menu_Store& options, const string exit_option) {
	natural
		current_option = 0;
	bool
		continue_work = true;

	cout << endl;

	set_cursor_visibility(false);
	print_leveled_menu(options, current_option, exit_option);

	do {
		// Управление положением указателя на текущий выбранный пункт меню
		switch (_getch()) {
		case KEY_END:
			current_option = options.size() - 1 * (exit_option.empty());
			print_leveled_menu(options, current_option, exit_option);
			break;
		case KEY_HOME:
			current_option = 0;
			print_leveled_menu(options, current_option, exit_option);
			break;
		case 214: // коды русских букв в CP1251
		case 246:
		case 'w':
		case 'W':
		case KEY_UP:
			if (current_option > 0)
				current_option--;
			else
				current_option = options.size() - 1 * (exit_option.empty());
			print_leveled_menu(options, current_option, exit_option);
			break;
		case 219: // коды русских букв в CP1251
		case 251:
		case 's':
		case 'S':
		case KEY_DOWN:
			if (current_option < options.size() - 1 * (exit_option.empty()))
				current_option++;
			else
				current_option = 0;
			print_leveled_menu(options, current_option, exit_option);
			break;
			// Переключение пункта
		case KEY_SPACE:
		case KEY_ENTER:
			if (not exit_option.empty() and current_option == options.size())
				continue_work = false;
			else {
				options.change_state(current_option);
				if (exit_option.empty())
					// Возвращается выбранный пункт, нажатие отменяется
					continue_work = false;
				else
					print_leveled_menu(options, current_option, exit_option);
			}
			break;
			// Завершение работы
		case KEY_ESCAPE:
			continue_work = false;
			break;
		default:
			continue;
			break;
		}
	} while (continue_work);

	// Удаление служебного пункта выхода из меню
	// Очистка консоли от меню
	print_leveled_menu(options, not_exist, exit_option);
	set_cursor_visibility(true);

	return options.at(current_option).name;
}

// Отрисовка пунктов меню, их значений и состояний
void print_leveled_menu(Menu_Store& options, natural current, const string exit_option) {
	for (int i = 0; i < options.size(); i++) {
		if (current != not_exist) {
			// Отрисовка указателя на текущий пункт
			if (i == current)
				cout << menu_pointer;
			else
				cout << string(menu_pointer.length(), ' ');

			// Отрисовка показателей сосотояния пункта
			if (not exit_option.empty()) {
				if (options.at(i).is_turned)
					color_print(option_on + one_space, console::green);
				else
					color_print(option_off + one_space, console::red);
			}

			// Отрисовка отступов для подпункта
			if (options.at(i).level) {
				cout << (options.at(i).level, sub_option_tab);
				if (i != current)
					color_print(sub_option_mark + one_space, console::dim);
				else
					color_print(sub_option_mark + one_space, console::yellow | console::dim);
			}

			// Отрисовка содержимого пункта
			if (i == current)
				color_print(options.at(i).name, console::yellow | console::dim);
			else
				color_print(options.at(i).name);
		}
		else
			clear_raw();
		cout << '\n';
	}
	// Отрисовка значка выхода
	if (current != not_exist) {
		if (options.size() == current) {
			cout << menu_pointer;
			color_print(exit_option, console::red | console::dim);
		}
		else
			cout << string(menu_pointer.length(), ' ') + exit_option;
		set_cursor_position(0);
	}
	else
		clear_raw();
	// Возвращение курсора вверх дл япоследующей отрисовки
	move_cursor(0, -(integer)options.size());
}
} // end namespace