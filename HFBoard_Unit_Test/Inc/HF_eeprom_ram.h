/*
 * HF_eeprom_ram.h
 *
 *  Created on: Nov 2, 2016
 *      Author: mdube
 */

#ifndef HF_EEPROM_RAM_H_
#define HF_EEPROM_RAM_H_

#include <stdint.h>
#include <stdbool.h>
#include "HF_shell.h"

#define MEM_ACCESS_MAX_LEN 80

typedef union
{
	uint16_t fifo;
	struct{
		uint8_t addr:4;
		uint16_t page:7;
	};
} MEM_ADDR;

typedef union{
	uint16_t data;
	struct{
		uint8_t device;
		uint8_t reg;
	};
	struct{
		uint8_t addr:4;
		uint8_t addr_page_lsb:4;
		uint8_t w_r_mask:1;
		uint8_t addr_page_msb:3;
		uint8_t addr_device:4;
	};
}EEPROM_CAT24C16;


typedef struct{
	union{
		uint16_t addr;
		struct{
			uint8_t address : 5;
			uint16_t page : 8;
		};
		struct{
			uint8_t addr_lsb;
			uint8_t addr_msb;
		};
	};
	union{
		uint8_t status_register;
		struct{
			uint8_t n_hold:1;
			uint8_t reserved:5;
			uint8_t mode_operation:2;
		};
	};
}RAM_23K640;

enum{
	RAM_23K640_MODE_OPERATION_BYTE_MODE = 0x00,
	RAM_23K640_MODE_OPERATION_SEQUENTIAL_MODE = 0x01,
	RAM_23K640_MODE_OPERATION_PAGE_MODE = 0x10,
}RAM_23K640_MODE_OPERATION;

enum{
	RAM_23K640_INSTRUCTION_STATUS_REGISTER_WRITE = 0x01,
	RAM_23K640_INSTRUCTION_DATA_WRITE = 0x02,
	RAM_23K640_INSTRUCTION_DATA_READ = 0x03,
	RAM_23K640_INSTRUCTION_STATUS_REGISTER_READ = 0x05,
}RAM_23K640_INSTRUCTION;

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
RAM_23K640 ram_23k640;

bool ram_23k640_init();
void init_ram();
void ram_write(MEM_ADDR* addr, uint8_t* in, uint8_t len);
void ram_read(MEM_ADDR* addr, uint8_t* out, uint8_t len);

void ram_write_str(MEM_ADDR* addr, char* in);
void ram_read_str(MEM_ADDR* addr, char* out, uint8_t len);
void ram_print_str(MEM_ADDR* addr, uint8_t len, SHELL_COMMAND_CONSOLE_TYPE buffer_type);
void ram_print_hex(MEM_ADDR* addr, uint8_t len, SHELL_COMMAND_CONSOLE_TYPE buffer_type);


#endif /* HF_EEPROM_RAM_H_ */
