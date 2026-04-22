#include <iostream>
#include "Common.h"
#include "Console.h"
#include "FileWork.h"
#include <Windows.h>

using namespace std;

int main()
{
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    ConsoleBase
        start_data;

    int
        work_result = -1;

    colorize_substrings(str_start, Green | Dim);

    init_return
        // Завод данных из файлов в класс хранения
        init_result = (init_return)start_data.init();

    system("pause");

    if (init_result == init_went_good)
        do {
            work_result = main_algorithm();
        } while (work_result > 0);

    else if (init_result == init_have_not_files)
        colorize_substrings(str_error_no_files, Yellow | Dim, Red | Dim);

    else
        color_print(str_error_empty_files, Red | Dim);
    

    // Обработка возвращаемого значения после прекращения работы
    if (work_result == 0)
        color_print(str_good_ending, Green | Dim);
    else
        color_print(str_bad_ending, Red | Dim);

    system("pause");

    return work_result;
}