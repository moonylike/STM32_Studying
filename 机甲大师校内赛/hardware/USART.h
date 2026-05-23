#ifndef __PID_H
#define __PID_H
void LanYa_Init(void);
void Key1IT_Init(void);
void Serial_SendByte(uint16_t Byte);
void Serial_Printf (char *format, ...);
void Serial_DisplayPrintf (uint16_t Line, uint16_t Column, uint8_t Size, char *String);
void Serial_DisplayClear (void);
void USART1_IRQHandler(void);
#endif
