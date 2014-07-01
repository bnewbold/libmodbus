#include <modbus.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

modbus_t *mb;
uint16_t tab_reg[64];
uint8_t raw_buffer[256];

#define STREAM_DOORBELL 15

int main() {
    int i, rc;
    uint32_t *mag_ix, *mag_iy, *mag_iz;

    mag_ix = (uint32_t *)&(raw_buffer[4]);
    mag_iy = (uint32_t *)&(raw_buffer[8]);
    mag_iz = (uint32_t *)&(raw_buffer[12]);
    mb = modbus_new_ascii("/dev/ttyUSB0", 115200, 'N', 8, 1);

    if (mb == NULL) {
        fprintf(stderr, "Unable to create the libmodbus context\n");
        return -1;
    }

    modbus_set_slave(mb, 2);
    modbus_connect(mb);
    modbus_flush(mb);

    // Start up the streaming
    printf("Starting...\r\n");
    tab_reg[0] = 1;
    rc = modbus_write_registers(mb, STREAM_DOORBELL, 1, tab_reg);
    if (rc == -1 && errno != 112345695) {
        fprintf(stderr, "ERR: %d: %s\n", errno, modbus_strerror(errno));
        return -1;
    }

    for (i = 0; i < 100; i++) {
        rc = modbus_receive_confirmation(mb, raw_buffer);
        //printf("Got %d; start=%d, len=%d\r\n", rc, raw_buffer[1], raw_buffer[3]);
        printf("%lu, %lu, %lu\r\n", *mag_ix, *mag_iy, *mag_iz);
    }

    tab_reg[0] = 0;
    modbus_write_registers(mb, STREAM_DOORBELL, 1, tab_reg);
    modbus_flush(mb);

    modbus_close(mb);
    modbus_free(mb);
    return 0;
}
