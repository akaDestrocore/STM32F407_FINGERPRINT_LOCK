/*												@@@@@@@@@@@@@@@@@@@      @@@@@@@@@@@@@@@@@@@@@@@
 @file     fingerprint.h						@@@@@@@@@@@@@@@@@  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 @brief    R308 module specific header file for @@@@@@@@@@@@@@@   @@@@@@@     @   @@@@@@@@@@@@@@
  		   STM32F4xx based Discovery board		@@@@@@@@@@@@@     @@@@@@@@  @@@@@@@@@@@@@@@@@@@@
 @author   destrocore							@@@@@@@@@@@@ @@@  (@@@@@@  @@@@@@@@@@@@@@@@@@@@@
 @version  V1.0									@@@@@@@@@@@@@@@@   @@@@/  @@@@@@@&   &@@.  @@@@@
												@@@@@@@@@@@@@@@@   @@@&  @@@@@     @@@@@@@@ @@@@
This file provides functions initialize and		@@@@@@@@@@@@@@@@@   @   @@@.    &@@@@@@@@@@@@@@@
control R308 fingerprint scanner module		 	@@@@@@@@@@@@@@@@@             @@@         %   @@
Module:											@@@@@@@@@@@@@@@@@   @@@@@          @@@@@@@@@@@ @
+ Initialization and de-initialization function @@@@@@@@@@@@@@@@@@@@@@@.%@  @@@@@  @@@@@@@@@@@@@
+ Module Control functions						@@@@@@@@@@@@@@@@@@              @@@@@@@@@@@@@@@@
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
#ifndef R308_H_
#define R308_H_

#include <stdint.h>
#include <simple_delay.h>
#include <stm32f407xx.h>

#define TX_BUFFER_SIZE       512
#define RX_BUFFER_SIZE       512


/*
 * USART ring buffer
 */
typedef struct
{
	USART_Handle_t USART_handle;
	uint16_t Timeout;
	volatile uint16_t TX_head;
	volatile uint16_t TX_tail;
	volatile uint16_t RX_head;
	volatile uint16_t RX_tail;
	volatile uint8_t TX_Buffer[TX_BUFFER_SIZE];
	volatile uint8_t RX_Buffer[RX_BUFFER_SIZE];
}USART_CircBuff_t;

extern USART_CircBuff_t R308_USART_Handle;

#define R308_USART_h R308_USART_Handle
#define R308h	R308_Config_Handle


/*
 * @R308_CMD
 */
typedef enum
{
	R308_SUCCESS 					= 0x0U,
	R308_HANDSHAKE_SUCCESS 			= 0x55U,
	R308_PACKET_RECIEVE_ERR 		= 0x1U,
	R308_FINGER_NOT_DETECTED    	= 0x2U,
	R308_FP_COLLECTION_FAIL			= 0x3U,
	R308_IMAGE_MESS         		= 0x6U,
	R308_CHAR_POINTS_LACKING		= 0x7U,
	R308_NO_MATCH           		= 0x8U,
	R308_NOT_FOUND          		= 0x9U,
	R308_ENROLL_MISMATCH    		= 0xAU,
	R308_BAD_LOCATION       		= 0xBU,
	R308_DB_READ_FAIL       		= 0xCU,
	R308_UPLOAD_FEATURE_FAIL		= 0xDU,
	R308_PACKET_RESPONSE_FAIL   	= 0xEU,
	R308_UPLOAD_FAIL            	= 0xFU,
	R308_DELETE_FAIL            	= 0x10U,
	R308_DB_CLEAR_FAIL          	= 0x11U,
	R308_PASSWORD_FAIL          	= 0x13U,
	R308_INVALID_IMAGE          	= 0x15U,
	R308_FLASH_ERR                	= 0x18U,
	R308_INVALID_REG              	= 0x1AU,
	R308_ADDR_CODE                	= 0x20U,
	R308_PASS_VERIFY              	= 0x21U
}R308_Confirmation_Code_t;

typedef enum
{
	R308_START_CODE               	= 0xEF01U,
	R308_COMMAND_PACKET           	= 0x1U,
	R308_DATA_PACKET              	= 0x2U,
	R308_ACK_PACKET               	= 0x7U,
	R308_END_DATA_PACKET           	= 0x8U
}R308_PackageID_t;

typedef enum
{
	R308_GET_IMAGE                	= 0x1U,
	R308_GEN_CHAR                	= 0x2U,
	R308_SEARCH                  	= 0x4U,
	R308_REG_MODEL                	= 0x5U,
	R308_STORE                   	= 0x6U,
	R308_LOAD                    	= 0x7U,
	R308_UP_CHAR                  	= 0x8U,
	R308_DOWN_CHAR                	= 0x9U,
	R308_IMG_UPLOAD               	= 0xAU,
	R308_IMG_DOWNLOAD				= 0xBU,
	R308_DELETE                  	= 0xCU,
	R308_EMPTY_DATABASE     	    = 0xDU,
	R308_VERIFY_PASSWORD          	= 0x13U,
	R308_TEMPLATE_NUM           	= 0x1DU
}R308_Instruction_t;

/*
 * @R308_Baudrate
 */
typedef enum
{
	R308_BAUDRATE_57600	= 57600
}R308_Baudrate_t;

/*
 * @R308_State
 */
typedef enum {
    R308_STATE_READ_HEADER,
    R308_STATE_READ_ADDRESS,
    R308_STATE_READ_PID,
    R308_STATE_READ_LENGTH,
    R308_STATE_READ_CONTENTS,
    R308_STATE_READ_CHECKSUM
} R308_State_t;

/*
 *  @R308 Settings
 */
typedef struct
{
	uint32_t Password;
	uint32_t Address;
	uint8_t buffer[33];
}R308_Handle_t;



// extern from main.c R308 configuration handle structure
extern R308_Handle_t R308_Config_Handle;


/*
 * R308 USART related functions
 */
void R308_USART_Init(USART_CircBuff_t * pR308Handle, uint32_t baudrate);
uint16_t R308_USART_WriteByte(uint8_t ch);
uint16_t R308_USART_Write(const uint8_t * byte, uint16_t length);
int16_t R308_USART_ReadByte(void);
uint16_t R308_USART_Read(uint8_t * byte, uint16_t length);
uint16_t R308_USART_GetNumOfAvailableRxBytes(void);
void R308_USART_Standby(void);
void R308_SetTimeout(uint16_t timeout);

/*
 *  R308 Initialization
 */
uint8_t R308_Init(R308_Handle_t * pR308Handle);

/*
 *  Image related functions
 */
int16_t R308_GetImage(void);
int16_t R308_GenChar(uint8_t bufferID);
int16_t R308_RegModel(void);
int16_t R308_Empty(void);
int16_t R308_Store(uint8_t bufferID, uint16_t templateID);

/*
 * Buffer related functions
 */
int16_t R308_LoadChar(uint8_t bufferID, uint16_t pageID);
int16_t R308_UpChar(uint8_t bufferID);
int16_t R308_DownChar(uint8_t bufferID);

/*
 * functions to transfer template from BufferID to MCU
 */
int16_t R308_UpImage2Host(void);
int16_t R308_DownImageFromHost(void);
int16_t R308_SearchDatabase(uint16_t * fingerID, uint8_t bufferID);
int16_t R308_DeletChar(uint16_t templateID, uint16_t num_to_delete);
int16_t R308_TemplateNum(void);


void r308_write_packet(R308_Handle_t *pR308Handle, uint8_t packettype, uint8_t *packet, uint16_t length);
int16_t r308_get_reply(R308_Handle_t *pR308Handle, uint8_t *replyBuf, uint16_t buffer_length, uint8_t *packetID);

#endif /* R308_H_ */
