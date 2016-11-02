/*
 * HF_eeprom_ram.h
 *
 *  Created on: Nov 2, 2016
 *      Author: mdube
 */

#ifndef HF_EEPROM_RAM_H_
#define HF_EEPROM_RAM_H_

#include "HF_shell.h"

#define MEM_ACCESS_MAX_LEN 80

typedef struct
{
  uint16_t page;	//Usually the low part
  uint16_t addr;	//Usually the high part
} MEM_ADDR;

extern SHELL_COMMAND_CONSOLE_TYPE;

/*
 * EEPROM
 */
void eeprom_write(MEM_ADDR* addr, uint8_t* in, uint8_t len);
void eeprom_read(MEM_ADDR* addr, uint8_t* out, uint8_t len);

void eeprom_write_str(MEM_ADDR* addr, char* in);
void eeprom_read_str(MEM_ADDR* addr, char* out, uint8_t len);
void eeprom_print_str(MEM_ADDR* addr, uint8_t len, SHELL_COMMAND_CONSOLE_TYPE buffer_type);
void eeprom_print_hex(MEM_ADDR* addr, uint8_t len, SHELL_COMMAND_CONSOLE_TYPE buffer_type);


/*
 * RAM
 */
void init_ram();
void ram_write(MEM_ADDR* addr, uint8_t* in, uint8_t len);
void ram_read(MEM_ADDR* addr, uint8_t* out, uint8_t len);

void ram_write_str(MEM_ADDR* addr, char* in);
void ram_read_str(MEM_ADDR* addr, char* out, uint8_t len);
void ram_print_str(MEM_ADDR* addr, uint8_t len, SHELL_COMMAND_CONSOLE_TYPE buffer_type);
void ram_print_hex(MEM_ADDR* addr, uint8_t len, SHELL_COMMAND_CONSOLE_TYPE buffer_type);


#endif /* HF_EEPROM_RAM_H_ */
