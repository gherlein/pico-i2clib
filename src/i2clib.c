/*
    pico-imu

(c) 2024 Gregory Herlein


*/

#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "pico/binary_info.h"
#include "pico/stdlib.h"
#include "pico/time.h"
#include "pico/types.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "pico-i2clib.h"

typedef struct i2c_vars_t
{
    i2c_inst_t *i2c;
    int baudrate;
    uint8_t sdaPin;
    uint8_t sdcPin;
    bool ready;
} i2c_var;

static bool reserved_addr(uint8_t addr)
{
    return (addr & 0x78) == 0 || (addr & 0x78) == 0x78;
}

i2c_inst_t *pico_init_i2c(i2c_inst_t *i2c, uint baudrate, uint8_t sdaPin, uint8_t sclPin)
{
    i2c_var.i2c = i2c;
    i2c_var.baudrate = buadrate;
    i2c_var.sdaPin = sdaPin;
    i2c_var.sdcPin = sdcPin;

    i2c_init(i2c, 100 * 1000);
    gpio_set_function(sdaPin, GPIO_FUNC_I2C);
    gpio_set_function(sclPin, GPIO_FUNC_I2C);
    gpio_pull_up(sdaPin);
    gpio_pull_up(sclPin);
    // Make the I2C pins available to picotool
    bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C));

    i2c_var.ready = true;

    return i2c;
}

void pico_scan_i2c(i2c_inst_t *i2c)
{
    printf("\nI2C Bus Scan\n");
    printf("   0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\n");

    for (int addr = 0; addr < (1 << 7); ++addr)
    {
        if (addr % 16 == 0)
        {
            printf("%02x ", addr);
        }

        // Perform a 1-byte dummy read from the probe address. If a slave
        // acknowledges this address, the function returns the number of bytes
        // transferred. If the address byte is ignored, the function returns
        // -1.

        // Skip over any reserved addresses.
        int ret;
        uint8_t rxdata;
        if (reserved_addr(addr))
            ret = PICO_ERROR_GENERIC;
        else
            ret = i2c_read_blocking(i2c, addr, &rxdata, 1, false);

        printf(ret < 0 ? "." : "@");
        printf(addr % 16 == 15 ? "\n" : "  ");
    }
    printf("Done.\n");
}
