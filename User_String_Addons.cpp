#include "User_String_Addons.h"

//
// Функции для работы со строкой, состоящей из лексем; дополнительные функции для редактирования строки
//

namespace std_usr {
// В следующей разработке переписать под методы класса string

// Получить число элементов в строке, разделённых спец. символом
natural element_quantity(const string& str, const string separator) {
	natural
		num = 0,
		index = 0;
	if (str.size()) {
		// Прибавка уже за наличие символов
		num++;
		// Прибавка если первый элемент пустой
		if (str.find(separator) == 0) {
			num++;
			index += separator.length();
		}
		index = str.find(separator, index);
		while (index != not_found) {
			num++;
			index = str.find(separator, index + separator.length());
		}
	}
	return num;
}

// Опредления индекса вхождения элемента (начиная с позиции разделителя)
natural element_begin(const string& str, natural element_number, const string separator) {
	natural
		start = 0;
	if (element_number < element_quantity(str, separator))
		for (int num = 0; num < element_number;)
			if (str.find(separator, start + separator.length()) != not_found) {
				start = str.find(separator, start + separator.length());
				num++;
			}
			else break;
	return start;
}

// Опредления индекса выхода элемента
natural element_end(const string& str, natural element_number, const string separator) {
	natural
		start = 0,
		end = 0;
	if (element_number == (element_quantity(str, separator) - 1))
		end = str.length();
	else if (element_number == 0)
		end = str.find(separator);
	else if (element_number < element_quantity(str, separator)) {
		start = element_begin(str, element_number, separator);
		end = str.find(separator, start + separator.length());
	}
	return end;
}

// Удалить лексему по её номеру
void element_erase(string& str, natural element_number, const string separator) {
	if (element_number == 0)
		str.erase(0, str.find(separator) + separator.length());
	else if (element_number < element_quantity(str)) {
		str.erase(element_begin(str, element_number, separator),
				  element_end(str, element_number, separator) - element_begin(str, element_number, separator));
	}
}

// Получить сам элемент по его номеру
string element_at(const string& str, natural element_number, const string separator) {
	natural
		element_size = element_quantity(str, separator),
		start = 0,
		end = 0;

	string
		element;

	if (element_number < element_size) {
		start = element_begin(str, element_number, separator);

		// Учёт длины разделителей, начиная с 1-го номера
		if (element_number > 0)
			start += separator.length();

		end = element_end(str, element_number, separator);

		element = str.substr(start, (end - start));

		// Если попался разделитель - очистка
		if (element == separator)
			element.clear();

		// Убрать лишние пробелы
		clear_str(element);
	}
	else
		element = string();

	return element;
}

// Получить номер элемента, зная индекс
natural numerize_index(const string& str, natural element_index, const string separator) {
	natural
		number = not_exist;

	if (str.size() and element_index == 0)
		number = 0;
	else if (element_index > 0 and element_index < str.length())
		number = element_quantity(str.substr(0, element_index), separator) - 1;

	return number;
}

// Получить индекс начала элемента, зная его номер
natural indexate_number(const string& str, natural element_number, const string separator) {
	natural
		str_size = element_quantity(str, separator),
		return_index;

	if (str_size <= element_number)
		return_index = not_found;
	else
		return_index = element_begin(str, element_number, separator);

	return return_index;
}

// Изменить регистр строки на нижний
void to_low_register(string& str) {
	natural
		letter_index = 0,
		length = str.length();

	for (int i = 0; i < length; i++) {
		letter_index = (letters_ru_cp1251_big + letters_en_big).find(str.at(i));
		if (letter_index != not_found)
			str.at(i) = (letters_ru_cp1251_small + letters_en_small).at(letter_index);
	}
}


// Добавить элемент в конец строки
void element_append(string& str, const string new_element, const string separator) {
	if (new_element.size()) {
		if (str.size())
			str += separator;

		str += new_element;
	}
}

// Удаление дубликатов
void duplicates_to_single(string& str, const string element) {
	string
		duplicate = element + element;
	size_t
		index = str.find(duplicate);

	while (index != not_found) {
		str.erase(index, element.length());
		index = str.find(duplicate, index);
	}
}

// Удаление пробелов (по умолчанию) по краям строки
void clear_str(string& str, const string erased) {
	string
		trimmed = str;

	// Удаление в начале строки
	while (trimmed.find(erased) == 0)
		trimmed.erase(0, erased.length());

	// Если строка ещё осталась - удаление части справа
	if (trimmed.size())
		while (trimmed.rfind(erased) == trimmed.length() - erased.length())
			trimmed.erase(trimmed.length() - erased.length());

	str = trimmed;
}

// Возвращает номер найденного элемента
natural element_find(const string& str, const string element, natural from_element, const string separator) {
	natural
		found_index,
		found_number;

	if (from_element >= element_quantity(str, separator))
		found_number = not_found;
	else {
		found_index = str.find(element, element_begin(str, from_element, separator));

		if (found_index != not_found)
			found_number = numerize_index(str, found_index, separator);
		else
			found_number = not_found;
	}
	return found_number;
}
} // end namespace