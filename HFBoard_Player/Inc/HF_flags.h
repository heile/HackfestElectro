/*
 * HF_Flags.h
 *
 *  Created on: Nov 1, 2016
 *      Author: mdube
 */

#ifndef HF_FLAGS_H_
#define HF_FLAGS_H_

#define FLAG_LEN 33	// 32 char + \0

typedef enum {
	FLAG1,
	FLAG2,
	FLAG3,
	FLAG4,
	FLAG5,
	FLAG6,
	FLAG7,
	FLAG8,
	FLAG9,
	FLAG10
} FLAG_NUM;

void get_flag(FLAG_NUM flag, char* out);

void generate_flags();

#endif /* HF_FLAGS_H_ */
