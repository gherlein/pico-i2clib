

#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "pico/binary_info.h"
#include "pico/stdlib.h"
#include "pico/time.h"
#include "pico/types.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

i2c_inst_t *pico_init_i2c(i2c_inst_t *i2c, uint baudrate, uint8_t sdaPin, uint8_t sclPin);
uint8_t pico_scan_i2c(i2c_inst_t *i2c, uint8_t *addrs);
