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

char HF_debug_command_buffer[50];

char HF_argv[5][15];
char HF_argc;

void HF_debugCommandSetBuffer(char c);


#endif /* HF_DEBUG_COMMAND_H_ */
