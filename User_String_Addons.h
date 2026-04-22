#pragma once

#include <string>
#include <iostream>
#include "Common.h"

//
// Функции для работы со строкой, состоящей из лексем; дополнительные функции для редактирования строки
//

using namespace std;

namespace std_usr {
// Изменить регистр строки на нижний
void to_low_register(string& str);

// Замена подряд идущих element на 1
void duplicates_to_single(string& str, const string element);

// Удаление пробелов (по умолчанию) по краям строки
void clear_str(string& str, const string erased = " ");

// === Функции работы с лексемами ===
// Получить номер лексемы, зная куда указывает индекс
natural numerize_index(const string&str, natural element_index, const string separator = separator_word);

// Получить число лексем в строке
natural element_quantity(const string&str, const string separator = separator_word);

// Возвращает номер найденной лексемы
natural element_find(const string& str, const string element, natural from_element = 0, const string separator = separator_word);

// Добавить ещё одну лексему в конец строки
void element_append(string& str, const string new_element, const string separator = separator_word);

// Получить лексему по её номеру
string element_at(const string&str, natural element_number, const string separator = separator_word);

// Удалить лексему по её номеру
void element_erase(string& str, natural element_number, const string separator = separator_word);

// === Вспомогательные функции ===
// Опредления индекса вхождения лексемы (начиная с позиции разделителя)
natural element_begin(const string &str, natural element_number, const string separator);

// Опредления индекса выхода элемента
natural element_end(const string &str, natural element_number, const string separator);

} // end namespace


