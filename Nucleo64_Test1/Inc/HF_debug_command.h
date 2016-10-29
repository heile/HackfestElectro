/*
 * HF_debug_command.h
 *
 *  Created on: 22 oct. 2016
 *      Author: HD-L
 */

#ifndef HF_DEBUG_COMMAND_H_
#define HF_DEBUG_COMMAND_H_

#include <stdio.h>
#include <string.h>
#include "HF_functions.h"


char HF_debug_command_buffer[50];

char HF_argv[5][15];
char HF_argc;

#define HF_DEBUG_COMMAND_RX_BUFFER_SIZE 32

extern uint8_t HF_debug_command_RxCount;

void HF_debugCommandSetBuffer(char c);

HF_FUNCTION_RETURN_STATE HF_debug_command_Read(uint8_t * fifo);

HF_FUNCTION_RETURN_STATE HF_debug_command_set_rx_buffer(uint8_t* Buf, uint32_t Len);

#endif /* HF_DEBUG_COMMAND_H_ */
