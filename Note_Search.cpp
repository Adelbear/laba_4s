#include "Main_Connector.h"

//
// Описание класса по работе с поиском записей по вводимым полям
//

using namespace std_usr;
using namespace input;
using namespace console;

Note_Search::Note_Search() {
    regime_prefix = str_prefix_search;
    current_state = regime_search;
    highlight(str_turn_search, console::green);
    cout << endl;
}

// Получение запроса от пользователя через ввод строки
void Note_Search::get_request() {
    string
        to_check,
        choice,
        input_value;
    bool
        continue_work = true;

    do {
        // Обработка не текстовых результатов ввода
        switch (getch_input(input_value, regime_prefix, all_signs)) {
        case GRV_F1:
            // Вывод помощи
            highlight(str_help_search, yellow | dim);
            cout << str_used_filenames << endl << endl;
            color_print(used_filenames, yellow | dim);
            cout << endl;
            break;
        case GRV_tabulation:
            // Меню - переключение режимов
            current_state = regime_assembly;
            continue_work = false;
            break;
        case GRV_F2:
            cout << str_search_filter_start;
            multilevel_menu(menu_search_filter);
            cout_prefix() << str_search_filter_end;
            // Меню - фильтры
            break;
        case GRV_F3:
            cout << str_search_regime_start;
            multilevel_menu(menu_regime_set);
            cout_prefix() << str_search_regime_end;
            break;
        case GRV_escape:
            // Меню с выбором - выход/нет 
            choice = multilevel_menu(&menu_exit);
            if (choice == str_exit_yes) {
                current_state = regime_escape;
                continue_work = false;
            }
            else if (choice == str_change_filenames)
                set_new_data();
            break;
        case GRV_enter:
            to_check = input_value;
            // Уборка лишних символов, чтобы не работать с пустой строкой
            duplicates_to_single(to_check, separator_word);
            clear_str(to_check, separator_word);

            if (to_check.size()) {
                cout << endl;
                current_state = regime_search;
                continue_work = false;
            }
            break;
        default:

            break;
        }
    } while (continue_work);

    request = input_value;
}

// Обработка запроса
void Note_Search::process_request() {
    natural
        // Количество результатов, удолветворяющих поиску
        new_size = 0,
        regime = 0;
    menu::Menu_Item
        current_option;
    string
        summary_filter,
        summary_regime,
        search_in_row;

    vector<string>
        search_result;

    if (current_state == regime_search) {

        // Настройка фильтров
        for (int i = 0; i < menu_search_filter.size(); i++)  {
            current_option = menu_search_filter.at(i);
            if (current_option.is_turned)
                element_append(summary_filter, current_option.name);
        }
        // Настройка режимов
        for (int i = 0; i < menu_regime_set.size(); i++) {
            current_option = menu_regime_set.at(i);
            if (current_option.is_turned)
                element_append(summary_regime, current_option.name);
        }
        if (summary_regime.find(str_search_not_accurate) != not_found)
            regime |= note::soft;
        else
            regime |= note::accurate;
        if (summary_regime.find(str_search_higlight) != not_found)
            regime |= note::color_mark;
        if (summary_regime.find(str_search_register_independent) != not_found)
            regime |= note::case_not_matter;

        // Поиск в записях (в полях и в модулях)
        for (int i = 0; i < the_notes.size(); i++) {
            search_result = the_notes.at(i).search(request, summary_filter, regime);

            if (not search_result.empty()) {
                // в выводимую строку добавляется заголовок
                search_in_row = the_notes.at(i).title() + "\n";

                for (int j = 0; j < search_result.size(); j++)
                    search_in_row += search_result.at(j) + "\n";

                new_size = search_result_note.size() + 1;

                search_result_note.resize(new_size);
                search_result_note.at(new_size - 1).initialize(search_in_row, string());
            }
        }
    }
}

// Вывод форматированного результата
void Note_Search::print_result() {
    const natural
        space_quantity = 2;
    string
        delim_line,
        output;
    natural
        result_quantity = 0,
        previous_index = 0,
        current_index = 0;

    if (current_state == regime_search) {

        if (search_result_note.empty())
            cout << str_search_no_result << endl;
        else {
            for (int i = 0; i < search_result_note.size(); i++) {
                delim_line = form_delim_row(element_quantity(search_result_note.at(i).title()));

                // Сборка заголовка
                output += delim_line + '\n';
                output += form_table_row(search_result_note.at(i).title(), element_quantity(search_result_note.at(i).title())) + '\n';
                output += delim_line + '\n';

                // Сборка остального тела
                for (int j = 0; j < search_result_note.at(i).size(); j++) {
                    output += form_table_row(search_result_note.at(i).at(j), element_quantity(search_result_note.at(i).title())) + '\n';
                    result_quantity++;
                }

               output += delim_line + '\n';
            }

            // Уведомлении о большом числе результатов
            if (result_quantity > note_print_limit) {
                static Menu_Store yes_no{
                    {str_search_overlimit_show},
                    {str_search_overlimit_deny}
                };

                cout_prefix() << str_search_overlimit << result_quantity;
                if (multilevel_menu(yes_no, string()) == str_search_overlimit_show) {
                    cout << str_search_result << endl;
                    highlight(output, green | dim);
                }
            }
            else {
                cout << str_search_result << endl;
                
                highlight(output, green | dim);
            }
            search_result_note.clear();
        }
    }
}