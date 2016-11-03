#include "HF_print.h"
#include "HF_shell.h"

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart3;

extern SHELL_COMMAND_CONSOLE_TYPE;

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
	HAL_UART_Transmit(&huart3, pBuffer, size, 10);//Uart send message "System Start!"
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
		while (HF_usb_vcp_RxCount > 0) {
		}
		HF_shell_command_Read(&pBuffer[i],SHELL_CONSOLE_TYPE_USB_VCP);
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


void hf_print_back(char* pBuffer, SHELL_COMMAND_CONSOLE_TYPE buffer_type){
	switch (buffer_type) {
		case SHELL_CONSOLE_TYPE_USB_VCP: hf_print_usb(pBuffer); break;
		case SHELL_CONSOLE_TYPE_RS232: hf_print_rs232(pBuffer); break;
		case SHELL_CONSOLE_TYPE_HACKER_UART_PORT: hf_print_hacker(pBuffer); break;
		case SHELL_CONSOLE_TYPE_ALL: hf_print_all(pBuffer); break;
	}
}

void hf_print_all(char* pBuffer){
	hf_print_usb(pBuffer);
	hf_print_rs232(pBuffer);
	hf_print_hacker(pBuffer);
	//HAL_Delay(2);
}


void hf_print_usb(char* pBuffer){
	CDC_Transmit_FS(pBuffer, strlen(pBuffer));
	HAL_Delay(1);
}

void hf_print_rs232(char* pBuffer){
	HAL_UART_Transmit(&huart3, pBuffer, strlen(pBuffer), 10);
}

void hf_print_hacker(char* pBuffer){
	HAL_UART_Transmit(&huart1, pBuffer, strlen(pBuffer), 10);
}

void print_on_start(){
	hf_print_all("\r\n"); HAL_Delay(2);
	hf_print_all("Hello all!\r\n"); HAL_Delay(2);
	hf_print_usb("Hello USB!\r\n"); HAL_Delay(2);
	hf_print_rs232("Hello RS232!\r\n"); HAL_Delay(2);
}
