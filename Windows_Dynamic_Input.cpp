#include "Windows_Dynamic_Input.h"

//
// Функция для посимвольного ввода с клавиатуры для анализа считываемых команд
//

using namespace std_usr::input;

namespace std_usr {
// Функция для работы с вводом строки
integer getch_input(string& output, string prefix, Allowed_Dictionary allowed, natural str_limit, natural vis_limit) {
	key_index
		input_symbol = '\0';
	natural
		return_code,
		// Число символов перед печатаемой строкой
		start_x = 0;
	bool
		control_key_detected = false,
		to_continue_get_symbols = true;
	string
		// Физическая строка слева от каретки
		left_str = output,
		// Физическая строка справа от каретки
		right_str,
		summary;

	set_cursor_position(0);
	start_x = prefix.length();
	clear_raw();
	color_print(prefix, console::dim);
	cout << left_str;
	set_cursor_position(start_x + left_str.length());

	do {
		input_symbol = _getch();
		set_cursor_visibility(false);

		if (_kbhit()) {
			input_symbol = _getch();
			control_key_detected = true;
		}
		// Случай управляющих клавиш, передаваемых одним символом
		else {
			switch (input_symbol) {
			case GRV_enter:
			case GRV_escape:
			case GRV_backspace:
			case GRV_tabulation:
				control_key_detected = true;
				break;
			default:
				break;
			}
		}

		if (control_key_detected) {
			control_key_detected = false;
			switch (input_symbol) {
			// Прекращение ввода, возврат управляющего символа
			case GRV_F1:
			case GRV_F2:
			case GRV_F3:
			case GRV_escape:
			case GRV_tabulation:
				return_code = input_symbol;
				to_continue_get_symbols = false;
				break;
			case GRV_enter:
				// Строки состоящие из пробелов не принимаются
				clear_str(summary);
				if (summary.size() or allowed == only_command) {
					return_code = input_symbol;
					to_continue_get_symbols = false;
				}
				else
					break;
			default:
				// Добавление/удаление символа логика перемещения курсора
				getch_process(left_str, right_str, input_symbol);
				getch_print(left_str, right_str, vis_limit, start_x);
				break;
			}
		}
		else {
			if (dictionary.at(allowed).find(input_symbol) != not_found)
				if (summary.length() < str_limit) {
					left_str += input_symbol;
					getch_print(left_str, right_str, vis_limit, start_x);
				}
		}
		// Вывод всей строки
		// getch_print(left_str, right_str, vis_limit, start_x);

		summary = left_str + right_str;

		set_cursor_visibility(true);
	} while (to_continue_get_symbols);

	// Сброс настроек печати
	getch_print();

	set_cursor_position(prefix.length());
	cout << summary;
	output = summary;

	return return_code;
}

namespace input {
// Печать вводимой строки 
void getch_print(const string& left_str, const string& right_str, natural vis_limit, natural start_x) {
	string
		// Подстрока для вывода на экран
		to_print,
		summary_str = left_str + right_str;
	natural
		left = left_str.length(),
		right = right_str.length(),
		summary = left + right,
		// Число отображаемых символов
		visible;
	static natural
		// Число символов, скрытых слева за границей отображения
		left_overlimit = 0,
		// Число символов, скрытых справа а границей отображения
		right_overlimit = 0;

	// Обнуление
	if (summary == 0 and vis_limit == 0 and start_x == 0) {
		left_overlimit = 0;
		right_overlimit = 0;
	}
	else {
		// Условия на случай, если ввести символов можно больше, чем их возможно уместить
		if (summary >= vis_limit) {
			// Каретка максимально справа, команда на добавление символа или перемотка вправо
			if (left - left_overlimit > vis_limit and left_overlimit < left) {
				left_overlimit++;
				// если перемотка
				if (right_overlimit > 0 and
					summary - left_overlimit - right_overlimit < vis_limit)
					right_overlimit--;
			}
			// Аналогично слева
			else if (right - right_overlimit > vis_limit and right_overlimit < right) {
				right_overlimit++;
				if (left_overlimit > 0 and
					summary - left_overlimit - right_overlimit < vis_limit)
					left_overlimit--;
			}
			// Каретка максимально справа, команда на удаление символа
			else if (summary - left_overlimit < vis_limit)
				left_overlimit--;
			// Аналогично слева
			else if (summary - right_overlimit < vis_limit)
				right_overlimit--;
			// Картека не с краю, команда на добавление символа
			else if (summary - left_overlimit - right_overlimit > vis_limit)
				right_overlimit++;
				// left_overlimit++;
			// Аналогично команда на удаление
			else if (summary - left_overlimit - right_overlimit < vis_limit)
				right_overlimit--;
				// left_overlimit--;
		}

		visible = summary - right_overlimit - left_overlimit;

		if (get_console_size().X > start_x + visible) {
			set_cursor_position(start_x);

			to_print = summary_str.substr(left_overlimit, vis_limit);

			// Печать с ' ' на конце для скрытия оставшихся символов при уменьшении размера строки
			cout << to_print + ' ';

			// Курсор перемещается на место раздела левой и правой частей строки (относительно картеки)
			set_cursor_position(start_x + left - left_overlimit);
		//	move_cursor(right_overlimit - 1 - right);
		}
		// Уведомление о необходимости расширить консоль
		else {
			move_cursor(0, -(integer)(start_x + visible - 1) / get_console_size().X);
			set_cursor_position(start_x);
			highlight(str_width_lack, console::red | console::dim);
			cout << string(vis_limit - str_width_lack.length(), ' ');
			move_cursor(0, -(integer)(start_x + vis_limit)/get_console_size().X );
			set_cursor_position(start_x + str_width_lack.length());
			
			do {
				system("pause > NUL");
			} while (get_console_size().X <= start_x + visible);
		}
	}
}

// Действия со строкой в соответвтие с коммандой
void getch_process(string& left_str, string& right_str, key_index symbol) {
	switch (symbol) {
	case GRV_left:
		if (left_str.size()) {
			right_str.insert(0, 1, left_str[left_str.length() - 1]);
			left_str.erase(left_str.length() - 1);
		}
		break;
	case GRV_right:
		if (right_str.size()) {
			left_str.append(right_str.substr(0, 1));
			right_str.replace(0, 1, string());
		}
		break;
	case GRV_delete:
		if (right_str.size())
			right_str.replace(0, 1, string());
		break;
	case GRV_backspace:
		if (left_str.size())
			left_str.erase(left_str.length() - 1);
		break;
	case GRV_nothing:
	default:
		break;
	}
}
} // end internal namespace
} // end namespace