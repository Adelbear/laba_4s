#include "One_String_Note.h"

//
// Класс для удобной работы с записями и поиском полей среди них
//

using namespace std_usr::note;
using namespace std_usr::console;

namespace std_usr {
// Создание записи
// В первой линии обязательно указаны названия полей, а в последующих, начиная с первой - поля
void Note::initialize(const string& note_unstructured, const string title) {
    string
        editing_note,
        line_with_field_name = element_at(note_unstructured, 0, separator_line);

    // Первая строка с шапкой убирается
    editing_note = note_unstructured.substr(note_unstructured.find(separator_line) + separator_line.length());

    // Если есть лишние переводы строки, удаляем
    clear_str(editing_note, separator_line);
    clear_str(editing_note, separator_word);
    duplicates_to_single(editing_note, separator_word);
    duplicates_to_single(editing_note, separator_line);

    // Чтобы ининциализация точно была только 1 раз
    if (str_title.empty() and editing_note.size()) {
        if (element_quantity(line_with_field_name) == element_quantity(title))
            str_title = title;
        else
            str_title = line_with_field_name;
        field_name = line_with_field_name;
        all_notes = editing_note;
    }
}

// Вывести все записи, удовлетворяющие критерию поиска
vector<string> Note::search(const string request, const string filter, natural search_type) {
    vector<string>
        search_result = vector<string>();

    vector<natural>
        search_result_number;

    string
        current_field_type,
        // данные, приведенные в более удобный для поиска вид
        request_data = request,
        search_data = all_notes,
        current_request_element,
        current_field,
        current_note;

    natural
        current_field_index = 0,
        current_note_number = 0,
        // Число совпавших результатов поиска по записи с числом элементов запроса
        allow_factor = 0,
        request_quantity;

    // Предварительная очистка строки
    for (int i = 0; i < element_quantity(request_data);) {
        current_field = element_at(request_data, i);
        clear_str(current_field);
        if (current_field.empty())
            element_erase(request_data, i);
        else
            i++;
    }
    request_quantity = element_quantity(request_data);
    // Установка символа, который заведомо нельзя получить при чтении файла с записями, чтобы прервать ввод
    if (request_quantity == 0)
        request_data = '\a';

    if (search_type & case_not_matter) {
        to_low_register(request_data);
        to_low_register(search_data);
    }

    // Поиск
    do {
        allow_factor = 0;
        current_request_element = element_at(request_data, 0);
        current_note_number = element_find(search_data, current_request_element, current_note_number, separator_line);

        if (current_note_number != not_found) {
            current_note = element_at(search_data, current_note_number, separator_line);

            for (int i = 0; i < request_quantity; i++) {
                current_request_element = element_at(request_data, i);
                current_field_index = current_note.find(current_request_element);
                current_field_type = element_at(field_name, numerize_index(current_note, current_field_index));

                if (current_field_index != not_found)
                    // Проверка на попадание в фильтр
                    if (filter.find(current_field_type) != not_found)
                        // Мягкий поиск
                        if (not (search_type & accurate))
                            allow_factor++;
                        // Поиск на совпадение 1 в 1
                        else {
                            while (current_field_index != not_found) {
                                current_field = element_at(current_note, numerize_index(current_note, current_field_index));
                                if (current_field == current_request_element) {
                                    allow_factor++;
                                    current_field_index = not_found;
                                }
                                else
                                    current_field_index = current_note.find(current_request_element, current_field_index + current_request_element.length());
                            }
                        }
            }
            if (allow_factor == request_quantity)
                search_result_number.push_back(current_note_number);
            current_note_number++;
        }
    } while (current_note_number != not_found);

    // Заполнение
    for (int i = 0; i < search_result_number.size(); i++) {
        current_note = element_at(all_notes, search_result_number.at(i), separator_line);

        // Установка пометок вокруг найденного элемента
        if (search_type & color_mark) {
            string
                modified_note;
            size_t
                // Количественное различие между модифицируемой строкой и исходной
                oversize = 0,
                temp_index = 0;
                        
            for (int j = 0; j < request_quantity; j++) {
                current_request_element = element_at(request_data, j);

                modified_note = current_note;
                if (search_type & case_not_matter)
                    to_low_register(modified_note);

                temp_index = modified_note.find(current_request_element);
                oversize = 0;
                while (temp_index != not_found) {
                    // Проверка, что метка ставится не внутри другой метки
                    if (current_note.find(color_left_mark, temp_index + oversize)
                        <= current_note.find(color_right_mark, temp_index + oversize)) {
                        current_note.insert(temp_index + oversize, color_left_mark);
                        current_note.insert(temp_index + current_request_element.length() + strlen(color_left_mark) + oversize, color_right_mark);
                        oversize += strlen(color_left_mark) + strlen(color_right_mark);
                    }
                    temp_index = modified_note.find(current_request_element, temp_index + current_request_element.length());
                }
            }
            duplicates_to_single(current_note, color_left_mark);
            duplicates_to_single(current_note, color_right_mark);
        }
        search_result.push_back(current_note);
    }
    return search_result;
}

// Возвращает полный заголовок названий полей
string Note::title() {
    return str_title;
}

// Вывести запись по её номеру
string Note::at(natural line_number) {
    return element_at(all_notes, line_number, separator_line);
}

// Число записей
size_t Note::size() {
    return element_quantity(all_notes, separator_line);
}

} // end namespace