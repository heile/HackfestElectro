/*
 * HF_eeprom_ram.c
 *
 *  Created on: Nov 2, 2016
 *      Author: mdube
 */

#include "stdint.h"
#include "HF_eeprom_ram.h"
#include "HF_I2C_Software.h"
#include "HF_print.h"
#include "stm32f0xx_hal_spi.h"
#include "HF_shell.h"

extern SPI_HandleTypeDef hspi1;
extern SHELL_COMMAND_CONSOLE_TYPE;

/*
 * EEPROM
 */
void eeprom_write(MEM_ADDR* addr, uint8_t* in, uint8_t len){
	i2cMemWriteSequence((0xA0|((addr->page>>4)&0x70)), (((addr->page<<4)&0xF0)|(addr->addr&0x0F)), in, len);
}

void eeprom_read(MEM_ADDR* addr, uint8_t* out, uint8_t len){
	i2cRead((0xA0|((addr->page>>4)&0x70)), (((addr->page<<4)&0xF0)|(addr->addr&0x0F)), len, out);
}

void eeprom_cat24c16_read(uint16_t addr, uint8_t* out, uint16_t len){
	int i=0;
	printf("EEPROM Read:\r\n");
	printf("     0x00 0x01 0x02 0x03 0x04 0x05 0x06 0x07 0x08 0x09 0x0A 0x0B 0x0C 0x0D 0x0E 0x0F\r\n");
	for(i=(addr & 0x000F);i>0;i--){
		printf("     ");
	}
	i2cRead(0xA0|((addr>>4)&0x70),(addr&0xFF),len,out);
	if ((16-(addr & 0x000F))>=len){
//		i2cRead(0xA0|((addr>>4)&0x70),(addr&0xFF),len,out);
		printf("\r\n0x%02X  ",addr>>4);
		for(i=0;i<len;i++){
			printf("%02X   ",out[i]);
		}
		len = 0;
	}else{
//		i2cRead(0xA0|((addr>>4)&0x70),(addr&0xFF),16-(addr & 0x000F),out);
		printf("\r\n0x%02X  ",addr>>4);
		for(i=0;i<16-(addr & 0x000F);i++){
			printf("%02X   ",out[i]);
		}
		len -= 16-(addr & 0x000F);
		out += 16-(addr & 0x000F);
		addr += 16;
	}
	while(len>=16){
//		i2cRead(0xA0|((addr>>4)&0x70),(addr&0xFF),len,out);
		printf("\r\n0x%02X  ",addr>>4);
		for(i=0;i<16;i++){
			printf("%02X   ",out[i]);
		}
		len-=16;
		out+=16;
		addr += 16;
	}
	if (len>0){
//		i2cRead(0xA0|((addr>>4)&0x70),(addr&0xFF),len,out);
		printf("\r\n0x%02X  ",addr>>4);
		for(i=0;i<len;i++){
			printf("%02X ",out[i]);
		}
	}
	printf("\r\nEND\r\n");
}

void eeprom_cat24c16_write(uint16_t addr, uint8_t* out, uint16_t len){
	int i=0;
	printf("EEPROM Write:\r\n");
	printf("00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\r\n");
	for(i=(addr & 0x000F);i>0;i--){
		printf("** ");
	}
	if ((16-(addr & 0x000F))>=len){
		i2cMemWriteSequence(0xA0|((addr>>4)&0x70),(addr&0xFF), out, len);
		for(i=0;i<len;i++){
			printf("%02X ",out[i]);
		}
		printf("\r\n");
		len = 0;
	}else{
		i2cMemWriteSequence(0xA0|((addr>>4)&0x70),(addr&0xFF),out,16-(addr & 0x000F));
		for(i=0;i<16-(addr & 0x000F);i++){
			printf("%02X ",out[i]);
		}
		printf("\r\n");
		len -= 16-(addr & 0x000F);
		out += 16-(addr & 0x000F);
		addr += 16;
	}

	while(len>=16){
		i2cMemWriteSequence(0xA0|((addr>>4)&0x70),(addr&0xFF),out,len);
		for(i=0;i<16;i++){
			printf("%02X ",out[i]);
		}
		printf("\r\n");
		len-=16;
		out+=16;
		addr += 16;
	}
	if (len>0){
		i2cMemWriteSequence(0xA0|((addr>>4)&0x70),(addr&0xFF),out,len);
		for(i=0;i<len;i++){
			printf("%02X ",out[i]);
		}
		printf("\r\n");
	}
	printf("END\r\n");
}

void eeprom_write_str(MEM_ADDR* addr, char* in){
	eeprom_write(addr, (uint8_t *)in, strlen(in));
}

void eeprom_read_str(MEM_ADDR* addr, char* out, uint8_t len){
	eeprom_read(addr, (uint8_t *)out, len);
}

void eeprom_print_str(MEM_ADDR* addr, uint8_t len, SHELL_COMMAND_CONSOLE_TYPE buffer_type){
	char buf[MEM_ACCESS_MAX_LEN];
	char fmt_char[4];

	if (len < MEM_ACCESS_MAX_LEN){
		memset(buf, 0x00, MEM_ACCESS_MAX_LEN);
		eeprom_read_str(addr, buf, len);
		hf_print_back("Read: ", buffer_type);
		hf_print_back(buf, buffer_type);
		hf_print_back("\r\n", buffer_type);
	} else {
		sprintf(fmt_char, "%i", MEM_ACCESS_MAX_LEN);
		hf_print_back("Message too long. Max is ", buffer_type);
		hf_print_back(fmt_char, buffer_type);
		hf_print_back("\r\n", buffer_type);
	}
}

void eeprom_print_hex(MEM_ADDR* addr, uint8_t len, SHELL_COMMAND_CONSOLE_TYPE buffer_type){

	char buf[MEM_ACCESS_MAX_LEN];
	char fmt_char[4];

	memset(fmt_char, 0x00, 4);

	if (len <= MEM_ACCESS_MAX_LEN){
		memset(buf, 0x00, MEM_ACCESS_MAX_LEN);
		eeprom_read_str(addr, buf, len);

		for(int i=0; i<len; i++){
		  sprintf(fmt_char, "0x%02x", buf[i]);
		  hf_print_back(fmt_char, buffer_type);
		  if (i < strlen(buf)-1){
			  hf_print_back(", ", buffer_type);
		  }

		  if ((i % 16) == 15){
			  hf_print_back("\r\n ", buffer_type);
		  }
		  memset(fmt_char, 0x00, 4);
		}
		hf_print_back("\r\n", buffer_type);

	} else {
		sprintf(fmt_char, "%i", MEM_ACCESS_MAX_LEN);
		hf_print_back("Message too long. Max is ", buffer_type);
		hf_print_back(fmt_char, buffer_type);
		hf_print_back("\r\n", buffer_type);
	}
}


/*
 * RAM
 */

void init_ram(){
	char dataOut[2];

	dataOut[0] = 0x05;	//Read status register
	dataOut[1] = 0x00;
	HAL_GPIO_WritePin(SPI1_CS_RAM_GPIO_Port, SPI1_CS_RAM_Pin, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(&hspi1, dataOut, dataOut, 2, 100);
	HAL_GPIO_WritePin(SPI1_CS_RAM_GPIO_Port, SPI1_CS_RAM_Pin, GPIO_PIN_SET);
	HAL_Delay(2);

	dataOut[0] = 0x01;	//Write status register
	dataOut[1] = 0x41;	//Set Sequential mode
	HAL_GPIO_WritePin(SPI1_CS_RAM_GPIO_Port, SPI1_CS_RAM_Pin, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(&hspi1, dataOut, dataOut, 2, 100);
	HAL_GPIO_WritePin(SPI1_CS_RAM_GPIO_Port, SPI1_CS_RAM_Pin, GPIO_PIN_SET);
	HAL_Delay(2);
}

void ram_write(MEM_ADDR* addr, uint8_t* in, uint8_t len){
	int i;
	char dataOut[64];

	memset(dataOut, 0x00, 64);

	dataOut[0] = 0x02;	//Write Instruction
	dataOut[1] = addr->page;  //addr low
	dataOut[2] = addr->addr;  //addr high

	for (i=0; i<len; i++){
		dataOut[i+3] = in[i];
	}

	HAL_GPIO_WritePin(SPI1_CS_RAM_GPIO_Port, SPI1_CS_RAM_Pin, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(&hspi1, dataOut, dataOut, len, 100);
	HAL_GPIO_WritePin(SPI1_CS_RAM_GPIO_Port, SPI1_CS_RAM_Pin, GPIO_PIN_SET);
	//HAL_Delay(50);
}

void ram_read(MEM_ADDR* addr, uint8_t* out, uint8_t len){
	int i;
	char dataOut[64];

	memset(dataOut, 0x00, 64);

	dataOut[0] = 0x03;	//Read Instruction
	dataOut[1] = addr->page;  //addr low
	dataOut[2] = addr->addr;  //addr high

	HAL_GPIO_WritePin(SPI1_CS_RAM_GPIO_Port, SPI1_CS_RAM_Pin, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(&hspi1, dataOut, dataOut, len, 100);
	HAL_GPIO_WritePin(SPI1_CS_RAM_GPIO_Port, SPI1_CS_RAM_Pin, GPIO_PIN_SET);
}

void ram_write_str(MEM_ADDR* addr, char* in){
	ram_write(addr, (uint8_t *)in, strlen(in));
}

void ram_read_str(MEM_ADDR* addr, char* out, uint8_t len){
	ram_read(addr, (uint8_t *)out, len);
}

void ram_print_str(MEM_ADDR* addr, uint8_t len, SHELL_COMMAND_CONSOLE_TYPE buffer_type){
	char buf[MEM_ACCESS_MAX_LEN];
	char fmt_char[4];

	if (len < MEM_ACCESS_MAX_LEN){
		memset(buf, 0x00, MEM_ACCESS_MAX_LEN);
		ram_read_str(addr, buf, len);
		hf_print_back("Read: ", buffer_type);
		hf_print_back(buf, buffer_type);
		hf_print_back("\r\n", buffer_type);
	} else {
		sprintf(fmt_char, "%i", MEM_ACCESS_MAX_LEN);
		hf_print_back("Message too long. Max is ", buffer_type);
		hf_print_back(fmt_char, buffer_type);
		hf_print_back("\r\n", buffer_type);
	}
}

void ram_print_hex(MEM_ADDR* addr, uint8_t len, SHELL_COMMAND_CONSOLE_TYPE buffer_type){
	char buf[MEM_ACCESS_MAX_LEN];
	char fmt_char[4];

	memset(fmt_char, 0x00, 4);

	if (len <= MEM_ACCESS_MAX_LEN){
		memset(buf, 0x00, MEM_ACCESS_MAX_LEN);
		ram_read_str(addr, buf, len);

		for(int i=0; i<len; i++){
		  sprintf(fmt_char, "%#x", buf[i]);
		  hf_print_back(fmt_char, buffer_type);
		  if (i < strlen(buf)-1){
			  hf_print_back(", ", buffer_type);
		  }

		  if ((i % 16) == 15){
			  hf_print_back("\r\n ", buffer_type);
		  }
		  memset(fmt_char, 0x00, 4);
		}
		hf_print_back("\r\n", buffer_type);

	} else {
		sprintf(fmt_char, "%i", MEM_ACCESS_MAX_LEN);
		hf_print_back("Message too long. Max is ", buffer_type);
		hf_print_back(fmt_char, buffer_type);
		hf_print_back("\r\n", buffer_type);
	}
}
