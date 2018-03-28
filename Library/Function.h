#ifndef _FUNCTION_H_
#define _FUNCTION_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "RF24.h"
#include "uart.h"
	
#define nRF24_WAIT_TIMEOUT         (uint32_t)0x000FFFFF

// Result of packet transmission
typedef enum {
	nRF24_TX_ERROR  = (uint8_t)0x00, // Unknown error
	nRF24_TX_SUCCESS,                // Packet has been transmitted successfully
	nRF24_TX_TIMEOUT,                // It was timeout during packet transmit
	nRF24_TX_MAXRT                   // Transmit failed with maximum auto retransmit count
} nRF24_TXResult;
	
void Timer_Init();	
void Initialize();
nRF24_TXResult nRF24_TransmitPacket(uint8_t *pBuf, uint8_t length);
void Simple_Receive_Init();

#ifdef __cplusplus
}
#endif

#endif /* __FUNCTION_H */
