/*										@@@@@@@@@@@@@@@@@@@      @@@@@@@@@@@@@@@@@@@@@@@
										@@@@@@@@@@@@@@@@@  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@
										@@@@@@@@@@@@@@@   @@@@@@@         @@@@@@@@@@@@@@
										@@@@@@@@@@@@@     @@@@@@@@  @@@@@@@@@@@@@@@@@@@@
										@@@@@@@@@@@@ @@@  (@@@@@@  @@@@@@@@@@@@@@@@@@@@@
										@@@@@@@@@@@@@@@@   @@@@/  @@@@@@@&         @@@@@
										@@@@@@@@@@@@@@@@   @@@&  @@@@@     @@@@@@@@ @@@@
										@@@@@@@@@@@@@@@@@   @   @@@.    &@@@@@@@@@@@@@@@
										@@@@@@@@@@@@@@@@@             @@@             @@
										@@@@@@@@@@@@@@@@@   @@@@@          @@@@@@@@@@@ @
										@@@@@@@@@@@@@@@@@@@@@@@.%@  @@@@@  @@@@@@@@@@@@@
										@@@@@@@@@@@@@@@@@@              @@@@@@@@@@@@@@@@
										@ @@@@@@@@@@@@@@                  @@@@@@@@@@@@@@
										@@  @@@@@@@@@                  @@@@@@@@@@@@@@@@@
										@@@@  @@@    @@@@@@@&         .@@@@@@@@@@@@@@@@@
										@@@@@@@#   ###@@@@( @        &@@@@@@@@@@@@@@@@@@
										@@@@@@@@@@@@@@@#     @@     (@     @@@@@@@@@@@@@
										@@@@@@@@@@@@@@     @@@@     @@     @@@@@@@@@@@@@
										@@@@@@@@@@@&     @@@@@@/   @@@@@@    @@@@@@@@@@@
										@@@@@@@@@@@*    @@@@@@@@  @@@@@@@@      @@@@@@@@
										@@@@@@@@@@@      @@@@@@@  @@@@@@@@   %  @@@@@@@@
										@@@@@@@@@@@@       /&@@@  @@@@@@&   @ @@@@@@@@@@
										@@@@@@@@@@@@@@&  ,@@@@@@@@@@@@  @ @@@@@@@@@@@@@@
										@@@@@@@@@@@@@@@@@@  @@@@@@@@@@@%@@@@@@@@@@@@@@@@													*/

#include "r308.h"
#include <string.h>
#include "st7735.h"
#include "stm32f407xx_usart.h"
#include <stdio.h>

static int16_t r308_usart_non_blocking_read_byte(void);
void r308_write_packet(R308_Handle_t *pR308Handle, uint8_t packettype, uint8_t *packet, uint16_t length);
int16_t r308_get_reply(R308_Handle_t *pR308Handle, uint8_t *replyBuf, uint16_t buffer_length, uint8_t *packetID);
static int16_t r308_read_ack_package(R308_Handle_t *pR308Handle, uint8_t *ack_package);
static int16_t r308_read_temp_num_bytes(R308_Handle_t *pR308Handle, uint8_t *template_count);
static void r308_irq_handler(void);


/*
 * USART related functions designed specifically fingerprint module functioning
 */
/********************************************************************************************************/
/* @function name 		- R308_USART_Init																*/
/*																										*/
/* @brief				- modified USART initialization function										*/
/*																										*/
/* @parameter[in]		- pointer to R308 handle structure												*/
/*																										*/
/* @parameter[in]		- USART baudrate																*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- initializes USART defined in r308.h as R308_USART								*/
/********************************************************************************************************/
void R308_USART_Init(USART_CircBuff_t * pR308Handle, uint32_t baudrate)
{
	GPIO_Handle_t R308_USART_GPIO = {0};

	if(USART1 == pR308Handle->USART_handle.pUSARTx)
	{
		//GPIO initialization
		R308_USART_GPIO.pGPIOx = GPIOA;
		R308_USART_GPIO.GPIO_Config.PinMode = GPIO_MODE_AF;
		R308_USART_GPIO.GPIO_Config.PinAltFuncMode = 7;
		R308_USART_GPIO.GPIO_Config.PinOPType = GPIO_OUTPUT_PP;
		R308_USART_GPIO.GPIO_Config.PinSpeed = GPIO_SPEED_HIGH;
		R308_USART_GPIO.GPIO_Config.PinPuPdControl = GPIO_PIN_PULL_UP;
		//TX
		R308_USART_GPIO.GPIO_Config.PinNumber = GPIO_PIN_9;
		GPIO_Init(&R308_USART_GPIO);
		//RX
		R308_USART_GPIO.GPIO_Config.PinNumber = GPIO_PIN_10;
		GPIO_Init(&R308_USART_GPIO);

		pR308Handle->USART_handle.USART_Config.USART_Baud = baudrate;
		pR308Handle->USART_handle.USART_Config.USART_WordLength = USART_WORDLEN_8BITS;
		pR308Handle->USART_handle.USART_Config.USART_NoOfStopBits = USART_STOPBITS_1;
		pR308Handle->USART_handle.USART_Config.USART_ParityControl = USART_PARITY_DISABLE;
		pR308Handle->USART_handle.USART_Config.USART_Mode = USART_MODE_TXRX;
		pR308Handle->USART_handle.USART_Config.USART_HWFlowControl = USART_HW_FLOW_CTRL_NONE;
		USART_Init(&pR308Handle->USART_handle);

		USART_IRQInterruptConfig(USART1_IRQn, 3, ENABLE);

	}else if(USART3 == pR308Handle->USART_handle.pUSARTx)
	{
		//GPIO initialization
		R308_USART_GPIO.GPIO_Config.PinMode = GPIO_MODE_AF;
		R308_USART_GPIO.GPIO_Config.PinAltFuncMode = 7;
		R308_USART_GPIO.GPIO_Config.PinOPType = GPIO_OUTPUT_PP;
		R308_USART_GPIO.GPIO_Config.PinSpeed = GPIO_SPEED_HIGH;
		R308_USART_GPIO.GPIO_Config.PinPuPdControl = GPIO_PIN_NO_PUPD;
		//TX
		R308_USART_GPIO.pGPIOx = GPIOB;
		R308_USART_GPIO.GPIO_Config.PinNumber = GPIO_PIN_10;
		GPIO_Init(&R308_USART_GPIO);
		//RX
		R308_USART_GPIO.pGPIOx = GPIOD;
		R308_USART_GPIO.GPIO_Config.PinNumber = GPIO_PIN_9;
		GPIO_Init(&R308_USART_GPIO);

		pR308Handle->USART_handle.USART_Config.USART_Baud = baudrate;
		pR308Handle->USART_handle.USART_Config.USART_WordLength = USART_WORDLEN_8BITS;
		pR308Handle->USART_handle.USART_Config.USART_NoOfStopBits = USART_STOPBITS_1;
		pR308Handle->USART_handle.USART_Config.USART_ParityControl = USART_PARITY_DISABLE;
		pR308Handle->USART_handle.USART_Config.USART_Mode = USART_MODE_TXRX;
		pR308Handle->USART_handle.USART_Config.USART_HWFlowControl = USART_HW_FLOW_CTRL_NONE;
		USART_Init(&pR308Handle->USART_handle);

		USART_IRQInterruptConfig(USART3_IRQn, 3, ENABLE);
	}

	pR308Handle->USART_handle.pUSARTx->CR1.bit.txeie = RESET;
	USART_ClearFlag(pR308Handle->USART_handle.pUSARTx, USART_SR_RXNE);
	pR308Handle->USART_handle.pUSARTx->CR1.bit.rxneie = SET;

	pR308Handle->TX_head = 0;
	pR308Handle->RX_head = 0;
	pR308Handle->TX_tail = 0;
	pR308Handle->RX_tail = 0;

	pR308Handle->Timeout = 500;
}


/********************************************************************************************************/
/* @function name 		- R308_USART_WriteByte															*/
/*																										*/
/* @brief				- write one byte to the data register 											*/
/*																										*/
/* @parameter[in]		- character																		*/
/*																										*/
/* @return				- returns 1 after writing to DR is done											*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
uint16_t R308_USART_WriteByte(uint8_t ch)
{
	uint16_t current_head = R308_USART_h.TX_head;
	uint16_t current_tail = R308_USART_h.TX_tail;

	// if TX DR is empty write to DR
	if((current_head == current_tail) && (RESET != USART_GetFlagStatus(R308_USART_h.USART_handle.pUSARTx, USART_SR_TXE)))
	{
		R308_USART_h.USART_handle.pUSARTx->DR.bit.dr = ch;
		return 1;
	}

	//circular buffer cycling mechanism
	uint16_t i = (current_head + 1) % TX_BUFFER_SIZE;

	// if buffer is full wait for empty place
	while(i == R308_USART_h.TX_tail)
	{

	}

	R308_USART_h.TX_Buffer[current_head] = ch;
	R308_USART_h.TX_head = i;

	R308_USART_h.USART_handle.pUSARTx->CR1.bit.txeie = SET;

	return 1;
}






/********************************************************************************************************/
/* @function name 		- R308_USART_ReadByte															*/
/*																										*/
/* @brief				- read one byte from data register												*/
/*								 																		*/
/* @return				- return one byte from TX data register; -1 if RX buffer is empty				*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
int16_t R308_USART_ReadByte(void)
{
	uint16_t current_head = R308_USART_h.RX_head;
	uint16_t current_tail = R308_USART_h.RX_tail;

	// if there is no data in RX buffer
	if( current_head == current_tail)
	{
		return -1;
	}
	else
	{
		uint8_t ch = R308_USART_h.RX_Buffer[current_tail];
		R308_USART_h.RX_tail = (uint16_t)(current_tail + 1) % RX_BUFFER_SIZE;
		return ch;
	}
}



/********************************************************************************************************/
/* @function name 		- R308_USART_Write																*/
/*																										*/
/* @brief				- USART data transmission non blocking function									*/
/*																										*/
/* @parameter[in]		- pointer to char byte in TX_Buffer												*/
/*																										*/
/* @parameter[in]		- length of data to be transmitted												*/
/*																										*/
/* @return				- number of bytes transmitted													*/
/*																										*/
/* @Note				- non-blocking function															*/
/********************************************************************************************************/
uint16_t R308_USART_Write(const uint8_t * byte, uint16_t length)
{
	uint16_t num = 0;

	while(length--)
	{
		if(R308_USART_WriteByte(*byte++))
		{
			num++;
		}
		else break;
	}

	return num;
}


/********************************************************************************************************/
/* @function name 		- R308_USART_Read																*/
/*																										*/
/* @brief				- USART data reception non blocking function									*/
/*																										*/
/* @parameter[in]		- pointer to char byte in RX_Buffer												*/
/*																										*/
/* @parameter[in]		- length of data to be received													*/
/*																										*/
/* @return				- number of bytes received														*/
/*																										*/
/* @Note				- non-blocking function															*/
/********************************************************************************************************/
uint16_t R308_USART_Read(uint8_t * byte, uint16_t length)
{
    uint16_t num = 0;

    while (num < length)
    {
        int16_t ch = r308_usart_non_blocking_read_byte();
        if (ch < 0)
        {
            break;
        }

        *byte++ = (uint8_t)ch;
        num++;
    }

    return num;
}


/********************************************************************************************************/
/* @function name 		- R308_USART_GetNumOfAvailableBytes												*/
/*																										*/
/* @return				- returns number of available bytes in RX_Buffer								*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
uint16_t R308_USART_GetNumOfAvailableRxBytes(void)
{
	return((uint16_t)(RX_BUFFER_SIZE + R308_USART_h.RX_head - R308_USART_h.RX_tail)) % RX_BUFFER_SIZE;
}


/********************************************************************************************************/
/* @function name 		- R308_USART_Standby															*/
/*																										*/
/* @brief				- blocks until transmission complete											*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- wait until the TXE interrupt is disabled and any ongoing transmission is 		*/
/*						  complete. TC flag should already be equal to 1 at RESET						*/
/********************************************************************************************************/
void R308_USART_Standby(void)
{
	// wait until TXE gets disabled and ongoing transmission is complete
	while((SET == R308_USART_h.USART_handle.pUSARTx->CR1.bit.txeie)
			|| (RESET == USART_GetFlagStatus(R308_USART_h.USART_handle.pUSARTx, USART_SR_TC)))
	{

	}
}



/********************************************************************************************************/
/* @function name 		- R308_SetTimeout																*/
/*																										*/
/* @brief				- Set Timeout value for R308													*/
/*																										*/
/* @parameter[in]		- timeout value																	*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void R308_SetTimeout(uint16_t timeout)
{
	R308_USART_h.Timeout = timeout;
}


/*
 * Initialization and de-initialization
 */
/********************************************************************************************************/
/* @function name 		- R308_Init																		*/
/*																										*/
/* @brief				- This function initializes R308 module											*/
/*																										*/
/* @parameter[in]		- pointer to R308 structure														*/
/*																										*/
/* @return				- 0 if OK and 1 if there's an error												*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
uint8_t R308_Init(R308_Handle_t *pR308Handle)
{
    uint32_t start = GetTick();
    while (GetTick() - start < 1000); // 1000 ms delay

    pR308Handle->buffer[0] = R308_VERIFY_PASSWORD;
    pR308Handle->buffer[1] = (pR308Handle->Password >> 24) & 0xFF;
    pR308Handle->buffer[2] = (pR308Handle->Password >> 16) & 0xFF;
    pR308Handle->buffer[3] = (pR308Handle->Password >> 8) & 0xFF;
    pR308Handle->buffer[4] = pR308Handle->Password & 0xFF;
    r308_write_packet(pR308Handle, R308_COMMAND_PACKET, pR308Handle->buffer, 5);

    uint8_t confirm_code = 0;
    int16_t length = r308_read_ack_package(pR308Handle, &confirm_code);

    if (length < 0 || R308_SUCCESS != confirm_code)
    {
    	return 0;
    }
    return 1;
}


/*
 *  Image related functions
 */
/********************************************************************************************************/
/* @function name 		- R308_GetImage																	*/
/*																										*/
/* @brief				- This function sends GetImage command to R308 module							*/
/*																										*/
/* @return				- 0 if OK and error code if there's an error									*/
/*																										*/
/* @Note				- Command to detect finger and store the detected finger image in ImageBuffer 	*/
/*						  while returning successful confirmation code; if there is no finger, returned */
/*						  confirmation code	(rc) would be “can’t detect finger”							*/
/********************************************************************************************************/
int16_t R308_GetImage(void)
{
	R308h.buffer[0] = R308_GET_IMAGE;
	r308_write_packet(&R308h, R308_COMMAND_PACKET, R308h.buffer, 1);
	uint8_t confirm_code = 0;
	int16_t ack_package = r308_read_ack_package(&R308h, &confirm_code);

	if(ack_package < 0)
	{
		return ack_package;
	}

	return confirm_code;
}

/********************************************************************************************************/
/* @function name 		- R308_GenChar																	*/
/*																										*/
/* @brief				- This function sends Genchar command to R308 module							*/
/*																										*/
/* @parameter[in]		- BufferID (1 or 2) character file buffer number								*/
/*																										*/
/* @return				- 0 if OK and error code if there's an error									*/
/*																										*/
/* @Note				- Command to generate character file from the original finger image in 			*/
/* 					      ImageBuffer and store the file in CharBuffer1 or CharBuffer2. BufferID of 	*/
/*						  CharBuffer1 and CharBuffer2 are 1h and 2h respectively. Other values(except 	*/
/*						  1h, 2h) would be processed as CharBuffer2.									*/
/********************************************************************************************************/
int16_t R308_GenChar(uint8_t bufferID)
{
	R308h.buffer[0] = R308_GEN_CHAR;
	R308h.buffer[1] = bufferID;
	r308_write_packet(&R308h, R308_COMMAND_PACKET, R308h.buffer, 2);
	uint8_t confirm_code = 0;
	int16_t ack_package = r308_read_ack_package(&R308h, &confirm_code);
	if(ack_package < 0 )
	{
		return ack_package;
	}
	return confirm_code;
}


/********************************************************************************************************/
/* @function name 		- R308_RegModel																	*/
/*																										*/
/* @brief				- This function sends RegModel command to R308 module							*/
/*																										*/
/* @return				- 0 if OK and error code if there's an error									*/
/*																										*/
/* @Note				- Command to combine information of character files from CharBuffer1 and 		*/
/*						  CharBuffer2andgenerate a template which is stored back in both CharBuffer1 and*/
/*						  CharBuffer2																	*/
/********************************************************************************************************/
int16_t R308_RegModel(void)
{
	R308h.buffer[0] = R308_REG_MODEL;
	r308_write_packet(&R308h, R308_COMMAND_PACKET, R308h.buffer, 1);
	uint8_t confirm_code = 0;
	int16_t ack_package = r308_read_ack_package(&R308h, &confirm_code);
	if(ack_package < 0)
	{
		return ack_package;
	}

	return confirm_code;
}


/********************************************************************************************************/
/* @function name 		- R308_Empty																	*/
/*																										*/
/* @brief				- This function sends RegModel command to R308 module							*/
/*																										*/
/* @return				- 0 if OK and error code if there's an error									*/
/*																										*/
/* @Note				- Command to combine information of character files from CharBuffer1 and 		*/
/*						  CharBuffer2andgenerate a template which is stored back in both CharBuffer1 and*/
/*						  CharBuffer2																	*/
/********************************************************************************************************/
int16_t R308_Empty(void)
{
	R308h.buffer[0] = R308_EMPTY_DATABASE;
	r308_write_packet(&R308h, R308_COMMAND_PACKET, R308h.buffer, 1);
	uint8_t confirm_code = 0;
	int16_t ack_package = r308_read_ack_package(&R308h, &confirm_code);
	if(ack_package < 0)
	{
		return ack_package;
	}

	return confirm_code;
}


/********************************************************************************************************/
/* @function name 		- R308_Store																	*/
/*																										*/
/* @brief				- This function sends Store command to R308 module								*/
/*																										*/
/* @parameter[in]		- BufferID (1 or 2) character file buffer number								*/
/*																										*/
/* @parameter[in]		- templateID （Flash location of the template,two bytes with high byte front and */
/*						  low byte behind）																*/
/*																										*/
/* @return				- 0 if OK and error code if there's an error									*/
/*																										*/
/* @Note				- Command to store the template of specified buffer (Buffer1/Buffer2) at the 	*/
/*						  designated location of Flash library. BufferID of CharBuffer1 and CharBuffer2 */
/*						  are 1h and 2h respectively. Other values(except 1h, 2h) would be processed as */
/*						  CharBuffer2																	*/
/********************************************************************************************************/
int16_t R308_Store(uint8_t bufferID, uint16_t templateID)
{
	R308h.buffer[0] = R308_STORE;
	R308h.buffer[1] = bufferID;
	R308h.buffer[2] = templateID >> 8;
	R308h.buffer[3] = templateID & 0xFF;
	r308_write_packet(&R308h, R308_COMMAND_PACKET, R308h.buffer, 4);
	uint8_t confirm_code = 0;
	int16_t ack_package = r308_read_ack_package(&R308h, &confirm_code);
	if(ack_package < 0)
	{
		return ack_package;
	}

	return confirm_code;
}


/*
 * Buffer related functions
 */
/********************************************************************************************************/
/* @function name 		- R308_LoadChar																	*/
/*																										*/
/* @brief				- This function sends Load command to R308 module								*/
/*																										*/
/* @parameter[in]		- BufferID (1 or 2) character file buffer number								*/
/*																										*/
/* @parameter[in]		- Page ID （Flash location of the template,two bytes with high byte front and low*/
/*						  byte behind）																	*/
/*																										*/
/* @return				- 0 if OK and error code if there's an error									*/
/*																										*/
/* @Note				- Command to load the template of specified buffer (Buffer1/Buffer2) from the 	*/
/*						  designated location of Flash library											*/
/********************************************************************************************************/
int16_t R308_LoadChar(uint8_t bufferID, uint16_t pageID)
{
	R308h.buffer[0] = R308_LOAD;
	R308h.buffer[1] = bufferID;
	R308h.buffer[2] = pageID >> 8;
	R308h.buffer[3] = pageID & 0xFF;
	r308_write_packet(&R308h, R308_COMMAND_PACKET, R308h.buffer, 4);
	uint8_t confirm_code = 0;
	int16_t ack_package = r308_read_ack_package(&R308h, &confirm_code);
	if(ack_package < 0)
	{
		return ack_package;
	}

	return confirm_code;
}


/********************************************************************************************************/
/* @function name 		- R308_UpChar																	*/
/*																										*/
/* @brief				- This function uploads the character file or template of CharBuff1/CharBuff2 to*/
/*						  upper computer																*/
/*																										*/
/* @parameter[in]		- BufferID (1 or 2) character file buffer number								*/
/*																										*/
/* @return				- 0 if OK and error code if there's an error									*/
/*																										*/
/* @Note				- Command to load the template of specified buffer (Buffer1/Buffer2) from the 	*/
/*						  designated location of Flash library. BufferID of CharBuffer1 and CharBuffer2 */
/*						  are 1h and 2h respectively. Other values(except 1h, 2h) would be processed as */
/*						  CharBuffer2																	*/
/********************************************************************************************************/
int16_t R308_UpChar(uint8_t bufferID)
{
	R308h.buffer[0] = R308_UP_CHAR;
	R308h.buffer[1] = bufferID;
	r308_write_packet(&R308h, R308_COMMAND_PACKET, R308h.buffer, 2);
	uint8_t confirm_code = 0;
	int16_t ack_package = r308_read_ack_package(&R308h, &confirm_code);
	if(ack_package < 0)
	{
		return ack_package;
	}

	return confirm_code;
}


/********************************************************************************************************/
/* @function name 		- R308_DownChar																	*/
/*																										*/
/* @brief				- This function downloads character file or template from upper computer to the */
/*						  specified buffer of Module													*/
/*																										*/
/* @parameter[in]		- BufferID (1 or 2) character file buffer number								*/
/*																										*/
/* @return				- 0 if OK and error code if there's an error									*/
/*																										*/
/* @Note				- Command to load the template of specified buffer (Buffer1/Buffer2) from the 	*/
/*						  designated location of Flash library. BufferID of CharBuffer1 and CharBuffer2 */
/*						  are 1h and 2h respectively. Other values(except 1h, 2h) would be processed as */
/*						  CharBuffer2																	*/
/********************************************************************************************************/
int16_t R308_DownChar(uint8_t bufferID)
{
	R308h.buffer[0] = R308_DOWN_CHAR;
	R308h.buffer[1] = bufferID;
	r308_write_packet(&R308h, R308_COMMAND_PACKET, R308h.buffer, 2);
	uint8_t confirm_code = 0;
	int16_t ack_package = r308_read_ack_package(&R308h, &confirm_code);
	if(ack_package < 0)
	{
		return ack_package;
	}

	return confirm_code;
}


/*
 * functions to transfer template from BufferID to MCU
 */
/********************************************************************************************************/
/* @function name 		- R308_UpImage2Host																*/
/*																										*/
/* @brief				- This function upload the image in Img_Buffer to the host						*/
/*																										*/
/* @return				-  0 if OK and error code if there's an error									*/
/*																										*/
/* @Note				- Module shall transfer the following data packet after responding to the host	*/
/********************************************************************************************************/
int16_t R308_UpImage2Host(void)
{
	R308h.buffer[0] = R308_IMG_UPLOAD;
	r308_write_packet(&R308h, R308_COMMAND_PACKET, R308h.buffer, 1);
	uint8_t confirm_code = 0;
	int16_t ack_package = r308_read_ack_package(&R308h, &confirm_code);
	if(ack_package < 0)
	{
		return ack_package;
	}

	return confirm_code;
}


/********************************************************************************************************/
/* @function name 		- R308_DownImageFromHost														*/
/*																										*/
/* @brief				- This function downloads image from host to Img_Buffer							*/
/*																										*/
/* @return				-  0 if OK and error code if there's an error									*/
/*																										*/
/* @Note				- Module shall transfer the following data packet after responding to the host.	*/
/*						  Data package length must be 64, 128, or 256									*/
/********************************************************************************************************/
int16_t R308_DownImageFromHost(void)
{
	R308h.buffer[0] = R308_IMG_DOWNLOAD;
	r308_write_packet(&R308h, R308_COMMAND_PACKET, R308h.buffer, 1);
	uint8_t confirm_code = 0;
	int16_t ack_package = r308_read_ack_package(&R308h, &confirm_code);
	if(ack_package < 0)
	{
		return ack_package;
	}

	return confirm_code;
}

/********************************************************************************************************/
/* @function name 		- R308_SearchDatabase															*/
/*																										*/
/* @brief				- This function searches for fingerprint in database							*/
/*																										*/
/* @parameter[in]		- finger ID																		*/
/*																										*/
/* @parameter[in]		- BufferID (1 or 2) character file buffer number								*/
/*																										*/
/* @return				-  0 if OK and error code if there's an error									*/
/*																										*/
/* @Note				- to search the whole finger library for the template that matches the one in 	*/
/*						  CharBuffer1orCharBuffer2. When found, PageID will be returned					*/
/********************************************************************************************************/
int16_t R308_SearchDatabase(uint16_t * fingerID, uint8_t bufferID)
{
	R308h.buffer[0] = R308_SEARCH;
	R308h.buffer[1] = bufferID;
	R308h.buffer[2] = 0x0;	//starting page 0
	R308h.buffer[3] = 0x0;	//0 searching numbers
	R308h.buffer[4] = (uint8_t)(500 >> 8);	// capacity
	R308h.buffer[5] = (uint8_t)(500 & 0xFF);

	r308_write_packet(&R308h, R308_COMMAND_PACKET, R308h.buffer, 6);
	uint8_t confirm_code = 0;
	int16_t length = r308_read_ack_package(&R308h, &confirm_code);
	if(length < 0)
	{
		return length;
	}
	if(R308_SUCCESS != confirm_code)
	{
		return confirm_code;
	}
	if(4 != length)
	{
		return -2;
	}

	*fingerID = R308h.buffer[1];
	*fingerID = (*fingerID << 8);
	*fingerID |= R308h.buffer[2];

	return confirm_code;
}


/********************************************************************************************************/
/* @function name 		- R308_DeletChar																*/
/*																										*/
/* @brief				- This function deletes a segment (N) of templates of Flash library started from*/
/*						  the specified location (or fingerID)											*/
/*																										*/
/* @parameter[in]		- template ID (template index in Flash)											*/
/*																										*/
/* @parameter[in]		- N (number of templates to be deleted)											*/
/*																										*/
/* @return				- 0 if OK and error code if there's an error									*/
/*																										*/
/* @Note				- to search the whole finger library for the template that matches the one in 	*/
/*						  CharBuffer1orCharBuffer2. When found, PageID will be returned					*/
/********************************************************************************************************/
int16_t R308_DeletChar(uint16_t templateID, uint16_t num_to_delete)
{
	R308h.buffer[0] = R308_DELETE;
	R308h.buffer[1] = templateID >> 8;
	R308h.buffer[2] = templateID & 0xFF;
	R308h.buffer[3] = num_to_delete >> 8;
	R308h.buffer[4] = num_to_delete & 0xFF;

	r308_write_packet(&R308h, R308_COMMAND_PACKET, R308h.buffer, 5);
	uint8_t confirm_code = 0;
	int16_t ack_package = r308_read_ack_package(&R308h, &confirm_code);
	if(ack_package < 0)
	{
		return ack_package;
	}

	return confirm_code;
}


/********************************************************************************************************/
/* @function name 		- R308_TemplateNum																*/
/*																										*/
/* @brief				- This function reads the current valid template number of the Module			*/
/*																										*/
/* @return				- 0 if OK and error code if there's an error									*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
int16_t R308_TemplateNum(void)
{
	R308h.buffer[0] = R308_TEMPLATE_NUM;
	r308_write_packet(&R308h, R308_COMMAND_PACKET, R308h.buffer, 1);
	uint8_t template_count = 0;
	int16_t ack_package = r308_read_temp_num_bytes(&R308h, &template_count);
	if(ack_package < 0)
	{
		return ack_package;
	}

	return template_count;
}




void USART1_IRQHandler(void)
{
    r308_irq_handler();
}

void USART3_IRQHandler(void)
{
	r308_irq_handler();
}


//helper APIs
static int16_t r308_usart_non_blocking_read_byte(void)
{
	int16_t ch;
	uint32_t Tick = GetTick();

	do{
		ch = R308_USART_ReadByte();
		if( ch >= 0 )
		{
			return ch;
		}
	}while((GetTick() - Tick) < R308_USART_h.Timeout);

	// return -1 is timed out
	return -1;
}


void r308_write_packet(R308_Handle_t *pR308Handle, uint8_t packettype, uint8_t *packet, uint16_t length)
{
	length += 2;

	uint8_t preamble[] = {
							(uint8_t)(R308_START_CODE >> 8),
							(uint8_t)R308_START_CODE,
	                        (uint8_t)(pR308Handle->Address >> 24),
							(uint8_t)(pR308Handle->Address >> 16),
	                        (uint8_t)(pR308Handle->Address >> 8),
							(uint8_t)(pR308Handle->Address),
							(uint8_t)packettype,
							(uint8_t)(length >> 8),
							(uint8_t)(length)
						};

    R308_USART_Write((uint8_t *)preamble, sizeof(preamble));

    uint16_t sum = (length >> 8) + (length & 0xFF) + packettype;
	for (uint8_t i = 0; i < length - 2; i++)
	{
		R308_USART_Write(&packet[i], 1);
		sum += packet[i];
	}

	R308_USART_Write((uint8_t *)(&sum) + 1, 1);
	R308_USART_Write((uint8_t *)&sum, 1);
}

int16_t r308_get_reply(R308_Handle_t *pR308Handle, uint8_t *replyBuf, uint16_t buffer_length, uint8_t *packetID)
{
	R308_State_t state = R308_STATE_READ_HEADER;

    uint16_t header = 0;
    uint16_t length = 0;
    uint8_t pID = 0;
    uint16_t checksum = 0;
    uint16_t remain = 0;

    uint32_t last_read = GetTick();

    while ((uint32_t)(GetTick() - last_read) < 2000)
    {
        switch(state)
        {
            case R308_STATE_READ_HEADER:
            {
                if(0 == R308_USART_GetNumOfAvailableRxBytes())
                {
                	continue;
                }

                last_read = GetTick();
                uint8_t byte;
                R308_USART_Read(&byte, 1);

                header = (header << 8);
                header |= byte;
                if(R308_START_CODE != header)
                {
                	break;
                }

                state = R308_STATE_READ_ADDRESS;
                header = 0;
                break;
            }
            case R308_STATE_READ_ADDRESS:
            {
                if(R308_USART_GetNumOfAvailableRxBytes() < 4)
                {
                	continue;
                }

                last_read = GetTick();
                R308_USART_Read(R308h.buffer, 4);
                uint32_t addr = R308h.buffer[0];
                addr = (addr << 8);
                addr |= R308h.buffer[1];
                addr = (addr << 8);
                addr |= R308h.buffer[2];
                addr = (addr << 8);
                addr |= R308h.buffer[3];

                if (addr != R308h.Address)
                {
                    state = R308_STATE_READ_HEADER;
                    //wrong address
                    break;
                }

                state = R308_STATE_READ_PID;
                break;
			}
            case R308_STATE_READ_PID:
            {
                if(0 == R308_USART_GetNumOfAvailableRxBytes())
                {
                	continue;
                }

                last_read = GetTick();
                R308_USART_Read(&pID, 1);
                checksum = pID;
                *packetID = pID;

                state = R308_STATE_READ_LENGTH;
                break;
			}
            case R308_STATE_READ_LENGTH:
            {
                if(R308_USART_GetNumOfAvailableRxBytes() < 2)
                {
                	continue;
                }

                last_read = GetTick();
                R308_USART_Read(R308h.buffer, 2);
                length = R308h.buffer[0];
                length = (length << 8);
                length |= R308h.buffer[1];

                //TODO: possible issue here
                if(length > 256 + 2 || (length > buffer_length + 2))
                {
                    state = R308_STATE_READ_HEADER;
                    //packet too long
                    continue;
                }

                /* num of bytes left to read */
                remain = length;

                checksum = checksum + R308h.buffer[0];
                checksum = checksum + R308h.buffer[1];
                state = R308_STATE_READ_CONTENTS;
                break;
            }
            case R308_STATE_READ_CONTENTS:
			{
				if (remain <= 2) {
					state = R308_STATE_READ_CHECKSUM;
					break;
				}

				if (R308_USART_GetNumOfAvailableRxBytes() == 0)
				{
					continue;
				}

				last_read = GetTick();

				uint8_t byte;
				R308_USART_Read(&byte, 1);

				*replyBuf++ = byte;

				checksum += byte;
				remain--;
				break;
			}
            case R308_STATE_READ_CHECKSUM:
            {
                if(R308_USART_GetNumOfAvailableRxBytes() < 2)
                {
                	continue;
                }

                last_read = GetTick();
                uint8_t temp[2];
                R308_USART_Read(temp, 2);
                uint16_t to_check = temp[0];
                to_check = (to_check << 8);
                to_check |= temp[1];

                if(to_check != checksum)
                {
                    state = R308_STATE_READ_HEADER;
					//wrong checksum
                    continue;
                }

                //without checksum
                return length - 2;
            }
        }
    }

//	ST7735_FillScreen(ST7735_RED);
//	ST7735_DrawString(35, 50, "RESPONSE        TIMEOUT", Font_11x18, ST7735_WHITE, ST7735_RED);
	delay_ms(1000);
    return -1;
}



static int16_t r308_read_ack_package(R308_Handle_t *pR308Handle, uint8_t *ack_package)
{
	uint8_t packetID = 0;
	int16_t length = r308_get_reply(pR308Handle, R308h.buffer, 33, &packetID);

	//timed out
	if(length < 0)
	{
		return length;
	}

	// wrong packet ID
	if (R308_ACK_PACKET != packetID)
	{
		return -2;
	}

	*ack_package = pR308Handle->buffer[0];


	// confirmation code
	return --length;
}


static int16_t r308_read_temp_num_bytes(R308_Handle_t *pR308Handle, uint8_t *template_count)
{
	uint8_t packetID = 0;
	int16_t length = r308_get_reply(pR308Handle, R308h.buffer, 33, &packetID);

	//timed out
	if(length < 0)
	{
		return length;
	}

	// wrong packet ID
	if (R308_ACK_PACKET != packetID)
	{
		return -2;
	}

	// get template count from 11th and 12th bytes
	*template_count = (pR308Handle->buffer[1] << 8) | pR308Handle->buffer[2];

	// confirmation code
	return --length;
}


static void r308_irq_handler(void)
{
    USART_RegDef_t *h_r308_usart = R308_USART_h.USART_handle.pUSARTx;

    // Check for Overrun Error first
    if (SET == USART_GetFlagStatus(h_r308_usart, USART_SR_ORE))
    {
        // Handle Overrun Error (e.g., clear the flag)
        USART_ClearFlag(h_r308_usart, USART_SR_ORE);
    }

    // Check for Receive Data Register Not Empty
    if (SET == USART_GetFlagStatus(h_r308_usart, USART_SR_RXNE))
    {
        unsigned char ch = h_r308_usart->DR.bit.dr;
        uint16_t k = (uint16_t)(R308_USART_h.RX_head + 1) % RX_BUFFER_SIZE;

        // Prevent overflow
        if (k != R308_USART_h.RX_tail)
        {
            R308_USART_h.RX_Buffer[R308_USART_h.RX_head] = ch;
            R308_USART_h.RX_head = k;
        }
        else
        {
            // Handle buffer overflow (e.g., discard the oldest data)
        }
    }

    // Check for Transmit Data Register Empty
    if (SET == USART_GetFlagStatus(h_r308_usart, USART_SR_TXE))
    {
        if (R308_USART_h.TX_head != R308_USART_h.TX_tail)
        {
            unsigned char ch = R308_USART_h.TX_Buffer[R308_USART_h.TX_tail];
            R308_USART_h.TX_tail = (R308_USART_h.TX_tail + 1) % TX_BUFFER_SIZE;
            h_r308_usart->DR.bit.dr = ch;
        }

        if (R308_USART_h.TX_head == R308_USART_h.TX_tail)
        {
            // Buffer is empty; disable TXE
            h_r308_usart->CR1.bit.txeie = RESET;
        }
    }
}

/****************************************************** End of file *************************************************************/
