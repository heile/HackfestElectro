/*
 * HF_Unit_Tests.h
 *
 *  Created on: Oct 29, 2016
 *      Author: mdube
 */

#ifndef HF_PLAYER_H_
#define HF_PLAYER_H_

#define PLAYER_ID 1

#include "stdint.h"
#include "HF_eeprom_ram.h"
#include <stdbool.h>

bool debug_mode_enabled();
void hf_print_all_dbg(char* pBuffer);

void init_wifi();
void init_uart();
void init_flag_on_rom();
void send_wifi_flag();

#endif /* HF_PLAYER_H_ */
