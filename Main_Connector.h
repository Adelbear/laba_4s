#pragma once
#include <string>
#include <vector>
#include "Common.h"
#include "Format_Output.h"
#include "Windows_Dynamic_Input.h"
#include "Multilevel_Dynamic_Menu.h"
#include "Windows_Console_Interaction.h"
#include "Windows_File_Work.h"
#include "One_String_Note.h"
#include "Text_Source.h"

//
// Каркас логики программы
//

using namespace std;
using namespace std_usr;

const natural
	// Число записей, выводимых на экран без запроса подтверждения на это у пользователя
	note_print_limit = 8,
	// Число типов записей
	note_quantity = 2;

// Результаты работы функции с инициализацией
const enum init_return {
	init_went_good			= 0,
	init_have_not_files		= 0x1,
	init_have_empty_files	= 0x2
};

// Команды для распределения по режимам
const enum Executed_Regime {
	regime_escape	= 0x01,
	regime_error	= 0x02,
	regime_base		= 0x04,
	regime_search	= 0x08,
	regime_assembly = 0x10
};

// Индексы записей в векторе
const enum note_type_index {
	index_programms = 0,
	index_modules
};

// Индексы полей в записе программы
const enum Programms_Note_Index {
	programm_name = 0,
	module_1,
	module_2,
	module_3
};

// Индексы полей в записе модуля
const enum Modules_Note_Index {
	module_name = 0,
	language,
	code_rows
};

// Меню подвтерждения выхода
const Menu_Store menu_exit{
	{str_exit_no},
	{str_change_filenames},
	{str_exit_yes}
};

// Распределение по классам
bool main_algorithm();

// Абстрактный базовый класс
class Note_Base {
protected:
	static Menu_Store
		// Меню с настройкой режимов поиска
		menu_regime_set,
		// Меню с настройкой фильтров поиска
		menu_search_filter;
	Executed_Regime
		// Текущее состояние переключателся режимов
		current_state;
	static string
		regime_prefix;

	static string
		used_filenames;
	static vector<Note>
		// Обрабатываемые записи
		the_notes;

public:
	// Получение запроса от пользователя
	virtual void get_request() {};

	// Обработка запроса
	virtual void process_request() {};

	// Вывод результата запроса
	virtual void print_result() {};

	ostream& cout_prefix();

	// Заполнить записи программ и модулей из файлов
	void initialize();

	// Выбор файлов с записями
	void set_new_data();

	// Получить информацию о текущем состоянии
	Executed_Regime state();

	Note_Base();
};

// Класс для работы с поисковым запросом
class Note_Search : public Note_Base {
private:
	string
		// Запрос пользователя для поиска
		request;
	vector<Note>
		// Результаты поиска по запросу
		search_result_note;
public:
	// Получение команды либо поискового запроса от пользователя
	virtual void get_request() override;

	// Проведение поиска запроса среди записей
	virtual void process_request() override;

	// Форматированный вывод результата поиска
	virtual void print_result() override;

	Note_Search();
};

// Класс для обработки записей, форматирования и вывода в файл
class Note_Assembly : public Note_Base {
private:
	string
		// Название файла в который вывводится итоговая информация
		current_filename;
public:
	// Получение команды от пользователя
	virtual void get_request() override;
	// Формирование файла
	virtual void process_request() override;
	// Вывод уведомления
	virtual void print_result() override;

	Note_Assembly();
};
