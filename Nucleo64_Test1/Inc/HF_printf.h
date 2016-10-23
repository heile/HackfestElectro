/*
 * HF_printf.h
 *
 *  Created on: 22 oct. 2016
 *      Author: HD-L
 */

#ifndef HF_PRINTF_H_
#define HF_PRINTF_H_

#include <stdio.h>
#include <sys/stat.h>

#include "usbd_cdc_if.h"

int _fstat(int fd, struct stat *pStat);

int _close(int a);

int _write(int fd, char *pBuffer, int size);

int _isatty(int fd);

int _lseek(int a, int b, int c);

int _read(int fd, char *pBuffer, int size);

caddr_t _sbrk(int increment);

#endif /* HF_PRINTF_H_ */
