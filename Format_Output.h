#pragma once
#include <string>
#include <vector>
#include "One_string_Note.h"
#include "User_String_Addons.h"
#include "Windows_Console_Interaction.h"

//
// Формирование строк для табличного вывода информации
//

using namespace std;

namespace std_usr {
// Внутреннее пространство имён с используемыми в модуле значениями
namespace format {
// Флаги для редактирования выводимой информации о времени
enum Date_Options {
	year		= 0x01,
	month		= 0x02,
	day			= 0x04,
	hour		= 0x08,
	min			= 0x10,
	sec			= 0x20,
	only_date	= year		| month		| day,
	only_time	= hour		| min		| sec,
	all			= only_date	| only_time
};

const natural
	space_width	= 3,
	print_width = 26;

const string
	boundary_vertical = "|",
	boundary_intersection = "+";
} // end internal namespace

// Формирование горизонтальной разделительной строки с перекркестиями
string form_table_row(const string one_row, natural cell_quantity, bool to_mark = true, natural cell_width = format::print_width);

// Формирование строки со значениями, отгорожденными друг от друга и выровненными посередине
string form_delim_row(natural cell_quantity, natural cell_width = format::print_width);

// Преобразования текущего времени в секундах в дату string
string time_str(natural options = format::all, natural time_factor = NULL);
} // end namespace