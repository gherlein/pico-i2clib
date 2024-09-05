/*
    pico-imu

(c) 2024 Gregory Herlein


*/

#include "pico-i2clib.h"

static bool reserved_addr(uint8_t addr)
{
    return (addr & 0x78) == 0 || (addr & 0x78) == 0x78;
}

i2c_inst_t *pico_init_i2c(i2c_inst_t *i2c, uint baudrate, uint8_t sdaPin, uint8_t sclPin)
{
    printf("init_i2c: %x, baud: %d scl %d, sda %d\n", i2c, baudrate, sdaPin);
    int ret = i2c_init(i2c, baudrate);
    printf("i2c opened at %d baud\n", ret);
    gpio_set_function(sdaPin, GPIO_FUNC_I2C);
    gpio_set_function(sclPin, GPIO_FUNC_I2C);
    gpio_pull_up(sdaPin);
    gpio_pull_up(sclPin);
    return i2c;
}

uint8_t pico_scan_i2c(i2c_inst_t *i2c, uint8_t *addrs)
{
    uint8_t count = 0;
    printf("\n");
    printf("   0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\n");

    for (int addr = 0; addr < (1 << 7); ++addr)
    {
        if (addr % 16 == 0)
        {
            printf("%02x ", addr);
        }

        // Skip over any reserved addresses.
        int ret;
        uint8_t rxdata;
        if (reserved_addr(addr))
            ret = PICO_ERROR_GENERIC;
        else
            ret = i2c_read_blocking(i2c, addr, &rxdata, 1, false);

        if (ret >= 0)
        {
            addrs[count] = addr;
            count++;
            printf(ret < 0 ? "." : "@");
        }

        printf(addr % 16 == 15 ? "\n" : "  ");
    }
    return count;
}
