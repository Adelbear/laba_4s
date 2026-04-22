#include "Main_Connector.h"

//
// Описание базового класса для работы с записями, общие методы и объявление констант
//

// Инициализация статического поля с записями
vector<Note> Note_Base::the_notes = vector<Note>();
string Note_Base::used_filenames = string();
string Note_Base::regime_prefix = str_prefix_base;

Note_Base::Note_Base() {
    current_state = regime_base;
}

ostream& Note_Base::cout_prefix() {
    color_print(regime_prefix, console::dim);
    return cout;
}

Menu_Store Note_Base::menu_regime_set{};
Menu_Store Note_Base::menu_search_filter{};

// Получить информацию о текущем состоянии
Executed_Regime Note_Base::state() {
    return current_state;
}

// Подготовка входных данных
void Note_Base::set_new_data() {
    integer
        init_state = init_went_good;
    string
        current_data,
        readed_filenames,
        current_mask,
        modules,
        programms;

    vector<string>
        readed_data,
        temp_filenames;

    Menu_Store
        menu_filenames = Menu_Store();

    readed_data.resize(note_quantity);

    // Вывод названий используемых файлов
    for (int i = 0; i < note_quantity; i++) {
        switch (i) {
        case index_programms:
            current_mask = str_filename_programms;
            break;
        case index_modules:
            current_mask = str_filename_modules;
            break;
        default:
            break;
        }
        temp_filenames = filenames_take(current_mask);

        menu_filenames.add(current_mask, string(), false);

        for (int j = 0; j < temp_filenames.size(); j++) {
            menu_filenames.add(temp_filenames.at(j), current_mask, false);
        }
    }

    do {
        init_state = init_went_good;
        readed_filenames.clear();
        system("pause");
        highlight(str_choose_filenames, console::dim);
        multilevel_menu(menu_filenames);

        // Считывание записей
        for (int i = 0; i < menu_filenames.size(); i++) {
            if (menu_filenames.at(i).parent != not_exist and menu_filenames.at(i).is_turned) {
                current_data = file_read(menu_filenames.at(i).name);

                if (not current_data.empty()) {
                    if (menu_filenames.at(i).name.find(str_filename_modules) != not_found)
                        element_append(readed_data.at(index_modules), current_data, separator_line);
                    else
                        element_append(readed_data.at(index_programms), current_data, separator_line);

                    element_append(readed_filenames, '\t' + menu_filenames.at(i).name, separator_line);
                }
            }
        }

        if (readed_filenames.find(str_filename_modules) != not_found and
            readed_filenames.find(str_filename_programms) != not_found) {
            cout << str_using_filenames << endl;
            color_print(readed_filenames + "\n", console::yellow | console::dim);
        }
        else
            init_state = init_have_not_files;

        if (init_state == init_went_good) {
            // Считается что и файл модулей и файл с программами оба и не пустые и присуствуют
            if (readed_data.at(index_programms).size() and readed_data.at(index_modules).size()) {
                // Для одного типа полей элементы шапки должны быть одинаковыми
                // Подготовка идентификаторов фильтра для структуры с хранением информации о модулях
                element_append(modules, str_note_modules_name);
                element_append(modules, str_note_modules_language);
                element_append(modules, str_note_modules_size + "\n");

                // Подготовка идентификаторов фильтра для структуры с хранением информации о программах
                element_append(programms, str_note_programms_name);
                element_append(programms, str_note_programms_modules);
                element_append(programms, str_note_programms_modules);
                element_append(programms, str_note_programms_modules + "\n");

                modules += readed_data.at(index_modules);
                programms += readed_data.at(index_programms);

                the_notes.clear();
                the_notes.resize(note_quantity);

                used_filenames = readed_filenames;

                // Запись в структуру хранения
                the_notes.at(index_programms).initialize(programms, str_note_programms_title);
                the_notes.at(index_modules).initialize(modules, str_note_modules_title);
            }
            else
                init_state = init_have_empty_files;
        }
        if (init_state == init_have_not_files)
            highlight(str_error_no_files, console::yellow | console::dim, console::red | console::dim);
        else if (init_state == init_have_empty_files)
            highlight(str_error_empty_files, console::red | console::dim);
    } while (init_state != init_went_good);
}

// Заполнить записи программ и модулей из файлов (Один раз)
void Note_Base::initialize() {
    // Инциализация меню фильтров
    menu_search_filter.add(str_note_programms);
    menu_search_filter.add(str_note_programms_name, str_note_programms);
    menu_search_filter.add(str_note_programms_modules, str_note_programms);
    menu_search_filter.add(str_note_modules);
    menu_search_filter.add(str_note_modules_name, str_note_modules);
    menu_search_filter.add(str_note_modules_language, str_note_modules);
    menu_search_filter.add(str_note_modules_size, str_note_modules);

    // Инициализация меню с режимами поиска
    menu_regime_set.add(str_search_higlight);
    menu_regime_set.add(str_search_not_accurate);
    menu_regime_set.add(str_search_register_independent);
}