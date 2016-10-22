/*
 * HF_debug_command.c
 *
 *  Created on: 22 oct. 2016
 *      Author: HD-L
 */

#include "HF_debug_command.h"

void HF_debugCommandSetBuffer(char c) {
	static uint8_t i = 0;
	if (c == 0x7F) {
		if (i > 0)
			i -= 1;
		HF_debug_command_buffer[i] = 0x00;
//		putch(0x7F);
	} else if (c >= ' ' && c <= '~') {
		HF_debug_command_buffer[i++] = c;
		HF_debug_command_buffer[i] = 0x00;
//		printf("%c", c);
	} else {
		//printf(" %#X ", c);
	}
	if (c == '\r') {
//		debugCommand(HF_debug_command_buffer);
		i = 0;
	}
}


void HFsetArg(char * message) {
    int i = 0;
    HF_argc = 0;
    memset(HF_argv[HF_argc], 0x00, 15);
    while ((*message != '\n')&&(*message != '\r')&&(*message != '\0')) {
        if (*message != ' ') {
        	HF_argv[HF_argc][i++] = *message;
            message++;
        } else if ((*(message + 1) > ' ')&&(*(message + 1) <= '~')) {
        	HF_argc++;
            memset(HF_argv[HF_argc], 0x00, 15);
            i = 0;
            message++;
        } else {
            message++;
        }
        if ((HF_argc > 4) || i > 15) {
            return;
        }
    }
    HF_argc++;
}


void HFdebugCommand(char * command) {
    int index = 0;

    //                    printf("\n\rDebug Command message: %s\n\r\n\r", debug_command_buffer);

    setArg(command);

//    printf("\n\rDebug Command:  ");
    for (index = 0; index < HF_argc; index++) {
//        printf("%s ", argv[index]);
    }
//    printf("\n\r");

    if (strcmp(HF_argv[0], "command") == 0) {

    }
}
