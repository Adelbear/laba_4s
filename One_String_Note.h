#pragma once

#include <vector>
#include <string>
#include "User_String_Addons.h"
#include "Windows_Console_Interaction.h"
#include "Common.h"

//
// Класс для удобной работы с записями и поиском полей среди них
//

using namespace std;

namespace std_usr {
// Внтуреннее пространство имён с режимами поиска
namespace note {
const enum Search_Type {
    soft            = 0x1,
    accurate        = 0x2,
    case_not_matter = 0x4,
    color_mark       = 0x8
};
} // end internal namespace

class Note {
private:
    string
        // Содержание полей одного типа (содержатся в одной строке, перечисление идёт через ';')
        all_notes;
    string
        // Названия полей, которые будут выводится на экран (по умлочанию совпадают с field_name)
        str_title,
        // Названия полей
        field_name;

public:
    // Вывести все записи, удовлетворяющие критерию поиска
    vector<string> search(const string request, const string filter, natural search_type);

    // Вывести запись по её номеру
    string at(natural line_number);

    // Число записей
    size_t size();

    // Вывод названия полей
    string title();

    // В первой линии обязательно указаны названия полей, а в последующих, начиная с первой - поля (линии разделены '\n', поля - ';')
    void initialize(const string& note_unstructured, const string title);
};
} // end namespace