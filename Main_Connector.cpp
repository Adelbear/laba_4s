#include "Main_Connector.h"

//
// Каркас логики программы
//

// Распределение по классам
bool main_algorithm() {
    static Note_Base
        // Переменная с классом, распределяющим работу по режимам
        * work_class = new Note_Search;
    static Executed_Regime
        current_state = regime_search;
    static bool  
        continue_work = true;

    // Распределине по режимам при наличии команды на это
    if (current_state != work_class->state()) {
        current_state = work_class->state();
        delete work_class;

        switch (current_state) {
        case regime_base:
        case regime_search:
            work_class = new Note_Search();
            break;
        case regime_assembly:
            work_class = new Note_Assembly();
            break;
        case regime_escape:
        default:
            current_state = regime_escape;
            continue_work = false;
            break;
        }
    }

    // Алгоритм работы режимов
    if (current_state != regime_escape) {
        current_state = work_class->state();

        work_class->get_request();

        work_class->process_request();

        work_class->print_result();
    }
    return continue_work;
}