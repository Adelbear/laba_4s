#include "Windows.h"
#include "Main_Connector.h"

using namespace console;

int main() {
    Note_Base*
        start_data;

    // Подклюение необходимой кодовой страницы для вывода кирилиических символов
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    highlight(str_start, green | dim);

    // Первоначальное чтение записей из файлов в класс хранения
    start_data = new Note_Base();
    start_data->initialize();
    start_data->set_new_data();
    delete start_data;

    highlight(str_start_help, green | dim, yellow);

    system("pause");

    while (main_algorithm());

    system("pause");

    return 0;
}