#include "HF_printf.h"

extern UART_HandleTypeDef huart1;

int _fstat(int fd, struct stat *pStat) {
	pStat->st_mode = S_IFCHR;
	return 0;
}

int _close(int a) {
	return -1;
}

int _write(int fd, char *pBuffer, int size) {
	CDC_Transmit_FS(pBuffer, size);
	HAL_UART_Transmit(&huart1, pBuffer, size, 10);//Uart send message "System Start!"
	return size;
}

int _isatty(int fd) {
	return 1;
}

int _lseek(int a, int b, int c) {
	return -1;
}

int _read(int fd, char *pBuffer, int size) {
	for (int i = 0; i < size; i++) {
		while (HF_debug_command_RxCount > 0) {
		}
		HF_debug_command_Read(&pBuffer[i]);
	}
	return size;
}

caddr_t _sbrk(int increment) {
	extern char end asm("end");
	register char *pStack asm("sp");

	static char *s_pHeapEnd;

	if (!s_pHeapEnd)
		s_pHeapEnd = &end;

	if (s_pHeapEnd + increment > pStack)
		return (caddr_t) -1;

	char *pOldHeapEnd = s_pHeapEnd;
	s_pHeapEnd += increment;
	return (caddr_t) pOldHeapEnd;
}

