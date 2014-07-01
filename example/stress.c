#include <modbus.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

modbus_t *mb;
uint16_t tab_reg[64];

int main() {
    int i, rc;
    printf("Connecting...\r\n");
    mb = modbus_new_ascii("/dev/ttyUSB0", 115200, 'N', 8, 1);

    if (mb == NULL) {
        fprintf(stderr, "Unable to create the libmodbus context\r\n");
        return -1;
    }


    modbus_set_slave(mb, 2);
    modbus_connect(mb);

    //modbus_set_response_timeout(mb, 0, 25000);

    // Works fairly reliably?
    modbus_set_response_timeout(mb, 0, 40000);

    // Sort of works, usually fails in <30 seconds
    //modbus_set_response_timeout(mb, 0, 32000);

    // Doesn't work
    //modbus_set_response_timeout(mb, 0, 25000);

    printf("Starting Stress Test\r\n");
    fflush(NULL);
    i = 0;
    while (1) {
        rc = modbus_read_registers(mb, 0, 20, tab_reg);
        if (rc == -1) {
            fprintf(stderr, "%s\n", modbus_strerror(errno));
            return -1;
        }
        i++;
        if (i % 50 == 0) {
            printf("Count: %d\r\n", i);
            fflush(NULL);
        }
    }

    modbus_close(mb);
    modbus_free(mb);
    return 0;
}
