#include "main.h"

// Define what part of demo will be compiled:
//   0 : disable
//   1 : enable
#define DEMO_RX_SINGLE      0 // Single address receiver (1 pipe)
#define DEMO_RX_MULTI       0 // Multiple address receiver (3 pipes)
#define DEMO_RX_SOLAR       0 // Solar temperature sensor receiver
#define DEMO_TX_SINGLE      1 // Single address transmitter (1 pipe)
#define DEMO_TX_MULTI       0 // Multiple address transmitter (3 pipes)
#define DEMO_RX_SINGLE_ESB  0 // Single address receiver with Enhanced ShockBurst (1 pipe)
#define DEMO_TX_SINGLE_ESB  0 // Single address transmitter with Enhanced ShockBurst (1 pipe)

#define Cam_Index 1 // Number of cam

uint32_t timer,time_now;
// Buffer to store a payload of maximum width
// Transmit result
nRF24_TXResult TX_result;
// Pipe number
nRF24_RXResult pipe;
// Buffer to read
uint8_t nRF24_payload[32];
// Length of received payload
uint8_t payload_length;
// flag of uart interrupt got data
int flag_got_data = 0;
// data store
uint8_t data_buf[2];
// Cam Led Status
int CamStatus;

int main()
{
	UART_Init(9600);
	Timer_Init();
	Initialize();
	
	#if (DEMO_TX_SINGLE)
		while (1)
		{
			if (flag_got_data == 1) 
			{
				TX_result = nRF24_TransmitPacket(data_buf, 1);
				UART_SendInt(TX_result);
			}
		}
	#endif
	
	#if (DEMO_RX_SINGLE)
		Simple_Receive_Init();
	// The main loop
    while (1) 
		{
    	//
    	// Constantly poll the status of the RX FIFO and get a payload if FIFO is not empty
    	//
    	// This is far from best solution, but it's ok for testing purposes
    	// More smart way is to use the IRQ pin :)
    	//
    	if (nRF24_GetStatus_RXFIFO() != nRF24_STATUS_RXFIFO_EMPTY) 
				{
    		// Get a payload from the transceiver
    		pipe = nRF24_ReadPayload(nRF24_payload, &payload_length);

    		// Clear all pending IRQ flags
			nRF24_ClearIRQFlags();

			// Print a payload contents to UART
			UART_SendStr("RCV PIPE#");
			UART_SendInt(pipe);
			UART_SendStr(" PAYLOAD:>");
			UART_SendBuf((char *)nRF24_payload, payload_length);
			UART_SendStr("<\r\n");
			CamStatus = GetStatus(nRF24_payload, Cam_Index);
			LedStatusOnOff(CamStatus);
    	}
    }
		#endif
}

extern "C" void TIM4_IRQHandler()
{
    if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
				timer ++;
				time_now++;
    }
}

extern "C" void USART1_IRQHandler(void)
{
    /* RXNE handler */
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
			data_buf[0] = '\0';
			data_buf[0] = USART_ReceiveData(USART1);
			flag_got_data = 1;
    }
     
    /* ------------------------------------------------------------ */
    /* Other USART1 interrupts handler can go here ...             */
}
