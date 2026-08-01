#include <util/delay.h>

#include "mci.h"
#include "uart.h"

int main() {

    init_uart();

    put_str("Hello world!\n\r");

    init_mci();

    discover_modules();

    run_automatic_test();

    while (1);

}