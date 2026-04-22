#include "Main_Connector.h"

//
// Описание класса для сборки итогового файла из записей программ и модулей
//

using namespace input;
using namespace console;

Note_Assembly::Note_Assembly() {
    regime_prefix = str_prefix_assembly;
    current_state = regime_assembly;
    highlight(str_turn_asssembly);
    cout << endl;
}

// Получение команды от пользователя
void Note_Assembly::get_request() {
    string
        choice,
        input_value;
    bool
        continue_work = true;

    do {
        // Обработка не текстовых результатов ввода
        switch (getch_input(input_value, regime_prefix, only_command)) {
        case GRV_F1:
            // Вывод поомщи
            highlight(str_help_assemble, yellow | dim);
            cout << endl;
            cout << str_used_filenames << endl;
            highlight(used_filenames, yellow | dim);
            break;
        case GRV_tabulation:
            // Меню - переключение режимов
            current_state = regime_search;
            continue_work = false;
            break;
        case GRV_enter:
            current_state = regime_assembly;
            system("pause");
            continue_work = false;
            break;
        case GRV_escape:
            // Меню с выбором - выход/нет 
            choice = multilevel_menu(&menu_exit, string());
            if (choice.find(str_exit_yes) != not_found) {
                current_state = regime_escape;
                continue_work = false;
            }
            else if (choice == str_change_filenames)
                set_new_data();
            break;
        default:

            break;
        }
    } while (continue_work);
}

// Формирование файла
void Note_Assembly::process_request() {
    static const natural
        space_quantity = 2;
    natural
        // Число модулей, которые относятся к программе
        on_line_modules;
    string
        modules_row = string(),
        delim_line = form_delim_row(code_rows + 1),
        middle_output,
        current_field,
        current_note;

    vector<string>
        module_search;

    if (current_state == regime_assembly) {
        current_filename = str_filename_output;
        // Текущая дата + время
        current_filename += time_str();
        current_filename += str_extension_default;

        // Справочная информация в начале файла
        file_write(current_filename, str_assembly_date);
        file_write(current_filename, time_str());
        file_write(current_filename, str_assembly_files);
        file_write(current_filename, used_filenames);

        // Перечисление программ и модулей к ним относящихся
        for (int i = 0; i < the_notes.at(index_programms).size(); i++) {
            modules_row.clear();
            on_line_modules = 0;

            current_note = the_notes.at(index_programms).at(i);

            middle_output = '\n';
            middle_output += string(space_quantity, ' ') + str_assembly_programm + element_at(current_note, programm_name) + '\n';

            // Проход по полям программы, где хранятся модули
            for (int j = module_1; j <= module_3; j++) {
                current_field = element_at(current_note, j);
                clear_str(current_field);
                if (current_field.size()) {
                    module_search = the_notes.at(index_modules).search(current_field, str_note_modules_name, note::accurate);

                    if (module_search.size() > 0) {
                        on_line_modules += module_search.size();

                        for (int k = 0; k < module_search.size(); k++)
                            modules_row += form_table_row(module_search.at(k), code_rows + 1, false) + '\n';
                    }                    
                    else {
                        on_line_modules++;

                        for (int k = 0; k < code_rows - module_name; k++)
                            element_append(current_field, absent_data);
                        modules_row += form_table_row(current_field, code_rows + 1) + '\n';
                    }
                }
            }

            // Строка с числом модулей в программе
            middle_output += string(space_quantity, ' ') + str_assembly_quantity + to_string(on_line_modules) + '\n' + delim_line + '\n';
            if (on_line_modules == 0)
                middle_output += "| " + str_assembly_no_modules + string(delim_line.length() - str_assembly_no_modules.length() - 3, ' ') + "|\n";
            // Добавление легенды к перечислениям модулей
            else
                middle_output += form_table_row(the_notes.at(index_modules).title(), code_rows + 1) + '\n' + delim_line + '\n';
            middle_output += modules_row;

            middle_output += delim_line;
            file_write(current_filename, middle_output);
        }
    }
}

// Вывод уведомления
void Note_Assembly::print_result() {
    if (current_state == regime_assembly)
        cout_prefix() << str_assembly_report + current_filename << endl;
}