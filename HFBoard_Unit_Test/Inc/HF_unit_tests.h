/*
 * HF_Unit_Tests.h
 *
 *  Created on: Oct 29, 2016
 *      Author: mdube
 */

#ifndef HF_UNIT_TESTS_H_
#define HF_UNIT_TESTS_H_

#include "stdint.h"
#include "HF_shell.h"

extern SHELL_COMMAND_CONSOLE_TYPE;

void HFShellCommand(char * command, SHELL_COMMAND_CONSOLE_TYPE buffer_type);
void process_debug_command(char** argv, int argc);

void encrypt_32byte_str(char* input, uint8_t* output);
void decrypt_32byte_str(uint8_t* input, char* output);
void print_32byte_str(char* input);

void test_encrypt_cbc(void);
void test_decrypt_cbc(void);
void test_encrypt_message(char* input);

#endif /* HF_UNIT_TESTS_H_ */
