#ifndef _ISR_H_
#define _ISR_H_

#include "common_typedef.h"

void EPIT1_IRQn_Irq_Handler(void);
void EPIT2_IRQn_Irq_Handler(void);


void GPIO1_Combined_0_15_Irq_Handler(void);
void GPIO1_Combined_16_31_Irq_Handler(void);

void GPIO2_Combined_0_15_Irq_Handler(void);
void GPIO2_Combined_16_31_Irq_Handler(void);

void GPIO3_Combined_0_15_Irq_Handler(void);
void GPIO3_Combined_16_31_Irq_Handler(void);

void GPIO4_Combined_0_15_Irq_Handler(void);
void GPIO4_Combined_16_31_Irq_Handler(void);

void GPIO5_Combined_0_15_Irq_Handler(void);
void GPIO5_Combined_16_31_Irq_Handler(void);

#endif // !_ISR_H_
