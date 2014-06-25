#include <modbus.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

modbus_t *mb;
uint16_t tab_reg[64];

int main() {
    int i, rc;
    mb = modbus_new_ascii("/dev/ttyUSB0", 115200, 'N', 8, 1);

    if (mb == NULL) {
        fprintf(stderr, "Unable to create the libmodbus context\n");
        return -1;
    }

    modbus_set_slave(mb, 2);
    modbus_connect(mb);

    rc = modbus_read_registers(mb, 0, 20, tab_reg);
    if (rc == -1) {
        fprintf(stderr, "%s\n", modbus_strerror(errno));
        return -1;
    }

    for (i=0; i < rc; i++) {
        printf("reg[%d]=%d (0x%X)\n", i, tab_reg[i], tab_reg[i]);
    }

    modbus_close(mb);
    modbus_free(mb);
    return 0;
}
