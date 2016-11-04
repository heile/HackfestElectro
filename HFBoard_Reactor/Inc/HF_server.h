/*
 * HF_Unit_Tests.h
 *
 *  Created on: Oct 29, 2016
 *      Author: mdube
 */

#ifndef HF_SERVER_H_
#define HF_SERVER_H_

#include "stdint.h"
#include <stdbool.h>
#include "HF_eeprom_ram.h"

bool debug_mode_enabled();
void hf_print_all_dbg(char* pBuffer);

void init_wifi();
void init_uart();
void init_flag_on_rom();
void send_wifi_flag();


void ir_callback(uint32_t input);
void ir_updatePlayerInput(uint32_t input);
bool ir_is_Valid(void);
void ir_printFlag(void);

#endif /* HF_SERVER_H_ */
