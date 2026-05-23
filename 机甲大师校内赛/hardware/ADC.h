#ifndef __ADC_H
#define __ADC_H

void ADCDetect_Init(void);
uint32_t forward1_GetValue(void);
uint32_t forward2_GetValue(void);
uint32_t forward3_GetValue(void);
uint32_t forward4_GetValue(void);
uint32_t forward5_GetValue(void);
uint32_t forward6_GetValue(void);
uint32_t forward7_GetValue(void);
uint32_t forward8_GetValue(void);

uint32_t backLeft_GetValue(void);
uint32_t backRight_GetValue(void);
uint32_t middleLeft_GetValue(void);
uint32_t middleRight_GetValue(void);
void AD_Detect(uint16_t ADCx, uint8_t ADC_Channel);

#endif
