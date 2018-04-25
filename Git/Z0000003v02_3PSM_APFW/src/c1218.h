/**
 ******************************************************************************
 * \file c1218.c
 ******************************************************************************
 * <hr><br>
 * <b>&copy; Copyright 2011 by Softek Global eDesign S. de R.L. de C.V. All rights reserved.</b><br><br>
 * Softek Global eDesign Confidential.
 *
 * <hr>
 ******************************************************************************
 *
 * @defgroup 
 * @section DESCRIPTION DESCRIPTION:
 *
 * This file contains all specific user configuration for mm100 Library.
 *
 * @section ABBREVIATION ABBREVIATIONS:
 * - Abbreviation(s):
 * 
 *
 * @section TRACEABILITY TRACEABILITY INFORMATION:
 *   - Design Document(s):
 *
 *   - Requirements Document(s):
 *     - CFE: Specification CFE G0100-05. March 2010.
 *
 *   - Applicable Standards:
 *     - ANSI C12.18 - 2005
 *     - ANSI C12.19 - 2008
 *
 * @section DFS DEVIATIONS FROM STANDARDS:
 *     - None
 *
 * @section TEMPLATE_REVISION TEMPLATE REVISION:
 *   - Format: F-P06-DH-07
 *   - Revision: 02
 *   - Date: 2015-08-10
 *   - Author: Rodrigo Hurtado Rodriguez.
 ******************************************************************************
 */

#ifndef C1218_H
#define C1218_H

/*
 ******************************************************************************
 * Include Header Files                                                     
 ******************************************************************************
 */
#include "util.h"
#include "secr.h"

/*
 ******************************************************************************
 * Constant Definitions using #define
 ******************************************************************************
 */

/*
 *-----------------------------------------------------------------------------
 * Communication channel settings
 *-----------------------------------------------------------------------------
 */
#define C1218_NUM_PACKETS				(uint8_t)  46
#define C1218_PACKET_SIZE				(uint16_t) 150
#define C1218_PACKET_OVERHEAD			(uint8_t)  8
#define C1218_DLL_SIZE					(uint16_t) (C1218_PACKET_SIZE - C1218_PACKET_OVERHEAD)
#define C1218_APL_SIZE					(uint16_t) (C1218_DLL_SIZE * C1218_NUM_PACKETS)
#define C1218_CHANNEL_TRAFIC_TIMEOUT	TIMER_ms_to_tic(6000)
#define C1218_INTER_CHARACTER_TIMEOUT	TIMER_ms_to_tic(510)
#define C1218_RESPONSE_TIMEOUT			TIMER_ms_to_tic(3000)

/*---------------------------------------------------------------------------*\
 *
 * 4.2. Layer 7—application layer
 *
 * 4.2.2. Language—protocol specifications for electric metering (“PSEM”)
 * The PSEM language consists of 9 services. Each service consists of a request and a response.
 * Each of these requests and responses is described in following service clauses.
 *
 * <requests>		::= <ident> | {Identification request}
 *						<read> | {Read request}
 *						<write> | {Write request}
 *						<logon> | {Logon request}
 *						<security> | {Security request}
 *						<logoff> | {Logoff request}
 *						<negotiate> | {Negotiate request}
 *						<wait> | {Wait request}
 *						<terminate> | {Terminate request}
 *
 * <responses>		::= <ident_r> | {Identification response}
 *						<read_r> | {Read response}
 *						<write_r> | {Write response}
 *						<logon_r> | {Logon response}
 *						<security_r> | {Security response}
 *						<logoff_r> | 333 {Logoff response}
 *						<negotiate_r> | {Negotiate response}
 *						<wait_r> | {Wait response}
 *						<terminate_r> | {Terminate response}
 *
 * 4.2.2.1. Request codes
 *
 * PSEM requests always include a one byte request code. Code numbers are assigned as follows:
 *
 * 00H-1FH Codes shall not be used to avoid confusion with response codes
 * 20H-7FH Codes are available for use within optical port protocol
 * 80H-FFH Codes shall be reserved for protocol extensions
 *
 * 4.2.2.2. Response codes
 *
 * PSEM responses always include a one byte response code. These codes are defined as follows:
 * 
 * <nok>	::= <err>|<sns>|<isc>|<onp>|<iar>|<bsy>|<dnr>|<dlk>|<rno>|<isss>
 *
 * <ok>		::= 00H		{Acknowledge—No problems, request accepted.}
 * <err>	::= 01H		{Error—This code is used to indicate rejection of the
 *						received service request. The reason for the rejection is
 *						not provided.}
 *
 * <sns>	::= 02H		{Service Not Supported—This application level error
 *						response will be sent to the device when the requested
 *						service is not supported. This error indicates that the
 *						message was valid, but the request could not be
 *						honored.}
 *
 * <isc>	::= 03H		{Insufficient Security Clearance—This application level
 *						error indicates that the current authorization level is
 *						insufficient to complete the request.}
 *
 * <onp>	::= 04H		{Operation Not Possible—This application level error will
 *						be sent to the device which requested an action that is
 *						not possible. This error indicates that the message was
 *						valid, but the message could not be processed. Covers
 *						conditions such as: Invalid Length, Invalid Offset}
 *
 * <iar>	::= 05H		{Inappropriate Action Requested—This application level
 *						error indicates that the action requested was
 *						inappropriate. Covers conditions such as write request
 *						to a read only table or an invalid table id.}
 *
 * <bsy>	::= 06H		{Device Busy—This application level error indicates that
 *						the request was not acted upon because the device was
 *						busy doing something else.}
 *
 * <dnr>	::= 07H		{Data Not Ready—This application level error indicates
 *						that request was unsuccessful because the requested
 *						data is not ready to be accessed.}
 *
 * <dlk>	::= 08H		{Data Locked—This application level error indicates that
 *						the request was unsuccessful because the data cannot
 *						be accessed.}
 *
 * <rno>	::= 09H		{Renegotiate Request—This application level error
 *						indicates that the responding device wishes to return to
 *						the ID or base state and re-negotiate communication
 *						parameters.}
 *
 * <isss>	::= 0AH		{Invalid Service Sequence State—This application level
 *						error indicates that the request is not accepted at the
 *						current service sequence state. For more information on
 *						service sequence states, see Annex D.}
 *
 *				0BH-1FH {Codes are currently undefined, but are available for use within
 *						optical port protocol}
 *
 *				20H-7FH {Codes shall not be used to avoid confusion with request
 *						codes}
 *
 *				80H-FFH {Codes shall be reserved for protocol extensions}
 *
\*---------------------------------------------------------------------------*/

/*===========================================================================*\
 * C12.18 Services
\*===========================================================================*/
#define IDENTIFICATION					0x20
#define FULL_READ						0x30
#define PREAD_OFFSET					0x3F
#define FULL_WRITE						0x40
#define PWRITE_OFFSET					0x4F
#define LOGON							0x50
#define SECURITY						0x51
#define LOGOFF							0x52
#define NEGOTIATE						0x60
#define TERMINATE						0x21

/*===========================================================================*\
 * C12.18 Response codes
\*===========================================================================*/
#define C1218_OK						0x00
#define C1218_ERR						0x01
#define C1218_SNS						0x02
#define C1218_ISC						0x03
#define C1218_ONP						0x04
#define C1218_IAR						0x05
#define C1218_BSY						0x06
#define C1218_DNR						0x07
#define C1218_DLK						0x08
#define C1218_RNO						0x09
#define C1218_ISSS						0x0A

#define C1218_PASS_LENGTH				20
#define C1218_USER_LENGTH				10
#define C1218_LOGON_TIMEOUT				(uint32_t)TIMER_ms_to_tic(90000)	// 1 min
#define C1218_LOGON_MFG_TIMEOUT			(uint32_t)TIMER_ms_to_tic(600000)	//10 min

#define SERVICE_INDEX					0x00
#define SERVICE_R_OK					0x00
#define SERVICE_R_INDEX					0x00
#define STD								0x00	// 0x00 = C12.18
#define STD_INDEX						0x01
#define VER								0x01
#define VER_INDEX						0x02
#define REV								0x00
#define REV_INDEX						0x03
#define END_OF_LIST						0x00
#define END_OF_LIST_INDEX				0x04

#define FULL_WRITE_DATA_INDEX			5
#define PWRITE_OFFSET_DATA_INDEX		8
#define READ_R_DATA_INDEX				3		
	 
/*---------------------------------------------------------------------------*\
 * C1218 Packet definition:
 *
 * <stp> <identity> <ctrl> <seq_nbr> <length> <data> <crc>
 * 1byte   1byte    1byte   1byte    2byte    n-byte  2byte
 *                                               
 * <stp>		::= EEH		{Start of packet character.}
 * <identity>	::= <byte>	{ C12.19 Device (end-device, table driven
 *							communication card, etc.) identity. It identifies
 *							the C12.19 Device in both the request and
 *							response packets.
 * <ctrl>		::= <byte>	{Control field.
 *							Bit 7. If true (1H) then this packet is part of a
 *							multiple packet transmission.
 *							
 *							Bit 6. If true (1H) then this packet is the first
 *							packet of a multiple packet transmission.
 *							
 *							Bit 5. Represents a toggle bit to reject duplicate
 *							packets. This bit shall be toggled for each new
 *							packet sent. Retransmitted packets keep the
 *							same state as the original packet sent. It should
 *							be noted that the initial state of the toggle bit is
 *							not specified and could initially be either 0 or 1.
 *							Bits 4 to 2, Reserved. The bits shall be set to
 *
 *							zero by the transmitter.
 *							Bit 0 to 1: DATA_FORMAT
 *							0 = C12.18 or C12.21
 *							1 = C12.22
 *							2 = Reserved
 *							3 = Reserved
 *
 *							C12.18 Compliant implementations shall set Bits
 *							0 and 1 to 0.}
 *
 * <length>		::= <word16> {Number of bytes of data in packet.}
 *
 * <data>		::= <byte>+ 1130 {<length> number of bytes of actual packet
 *							data. This value is limited by the maximum
 *							packet size minus the packet overhead of 8
 *							bytes. This value can never exceed 8183.}
 *
 * <crc>		::= <word16> {CCITT CRC standard polynomial X^16 + X^12 + X^5 + 1}
 *
 *
 * A duplicate packet is defined as a packet whose identity, toggle bit and valid
 * CRC are identical to those of the immediate previous packet. If a duplicate packet
 * is received by the target device, the target should disregard the duplicate packet
 * and return an <ack>. At the start of session, the toggle bit in the first packet
 * may assume either state.
 *
\*---------------------------------------------------------------------------*/
#define C1218_FULL_WRITE_OVERHEAD_SIZE			6
#define C1218_FULL_READ_OVERHEAD_SIZE			6

#define C1218_HEADER_LENGTH						0x07
#define C1218_STP								0xEE
#define C1218_ACK								0x06
#define C1218_NOK								0x15

#define C1218_STP_INDEX							0x00
#define C1218_IDENTITY_INDEX					0x01
#define C1218_CTRL_INDEX						0x02
#define C1218_SEQ_NBR_INDEX						0x03
#define C1218_LENGTH_H_INDEX					0x04
#define C1218_LENGTH_L_INDEX					0x05
#define C1218_DATA_INDEX						0x06

/*===========================================================================*\
 * C1219 Procedure definition
\*===========================================================================*/
#define PROCEDURE_OVERHEAD_SIZE					3
#define PROCEDURE_ID_INDEX						5
#define PROCEDURE_SEQ_NBR_INDEX					7
#define PROCEDURE_PARAMETER_INDEX				8

#define PROC_COMPLETED							0
#define PROC_ACCEPTED_NOT_COMPLETED				1
#define INVALID_PARAMETER						2
#define PROC_CONFLICT_WITH_SET_UP				3
#define PROC_IGNORED_TIME_CONSTRAIN				4
#define PROC_IGNORED_NO_AUTHORIZATION			5
#define PROC_IGNORED_UNRECOGNIZED_PROC			6

/*
 ******************************************************************************
 * Macro Definitions using #define
 ******************************************************************************
 */

/*
 ******************************************************************************
 * Enumerations and Structures and Typedefs                          
 ******************************************************************************
 */

/*===========================================================================*\
 * C12.18 Communication states
\*===========================================================================*/
typedef enum c1218_communication_state
{
	BASE_STATE		 = 0x00,
	ID_STATE,						
	SESSION_STATE,
	NBR_C1218_STATES				
} C1218_COMMUNICATION_STATE;

/*===========================================================================*\
 * C1218 Data link layer state
\*===========================================================================*/
typedef enum c1218_dll_state
{
    C1218_RX_IDLE = 0x00,
 	C1218_RX_STP,
    C1218_RX_IDENTITY,
	C1218_RX_CTRL,
	C1218_RX_SEQ_NBR,
	C1218_RX_LENGTH_H,
	C1218_RX_LENGTH_L,
	C1218_RX_DATA,
	C1218_RX_CRC_H,
	C1218_RX_CRC_L,
	C1218_RX_WAITING_ACK,
	
 	C1218_TX_IDLE,
	C1218_TX_STP,
	C1218_TX_IDENTITY,
	C1218_TX_CTRL,
	C1218_TX_SEQ_NBR,
	C1218_TX_LENGTH_H,
	C1218_TX_LENGTH_L,
	C1218_TX_DATA,
	C1218_TX_CRC_H,
	C1218_TX_CRC_L,
} C1218_DLL_STATE;

/*
 ******************************************************************************
 * Global Variables and Const Variables using extern declaration.
 ******************************************************************************
 */

/*
 ******************************************************************************
 * Enumerations and Structures and Typedefs
 ******************************************************************************
 */
typedef struct c1218_dll_type
{
	xTaskHandle os_task_id;
	
	uint8_t rx_state;
	uint8_t rx_timer;
	uint8_t rx_error;
	bool rx_eop;
	
	uint8_t *rx_data;
	uint16_t rx_data_length;
	uint8_t rx_data_bytes;
	uint16_t rx_data_bytes_remaining;
	uint16_t rx_data_offset;
	
	uint8_t rx_buffer[C1218_PACKET_SIZE];
	uint16_t rx_buffer_bytes;
	
	uint16_t rx_buffer_previous_crc;
	uint8_t rx_buffer_previous_identity;
	uint8_t rx_buffer_previous_toggle_bit;
	
	void_fptr rx_abort;
	void_fptr io_configure;
	
	uint8_t tx_state;
	uint8_t tx_timer;
	uint8_t tx_error;
	bool tx_eop;
	
	uint8_t *tx_data;
	uint16_t tx_data_length;
	uint8_t tx_data_bytes;
	uint16_t tx_data_bytes_remaining;
	uint16_t tx_data_offset;
	
	uint8_t tx_buffer[C1218_PACKET_SIZE];
	uint16_t tx_buffer_bytes;
	
	uint8_t st_08_data[C1218_PACKET_SIZE];
	uint16_t st_08_length;
	
	void_fptr tx_abort;
	void_fptr tx_retry;
	bool wait_for_ack;
	uint8_t tx_nbr_retries;
	uint8_t response_timeout_timer;
	
	Uart *uart;
	bool ongoing_transmission;
    bool interrupt_context;
	
} C1218_DLL_TYPE;

typedef struct c1218_apl_type
{
	C1218_COMMUNICATION_STATE communication_state;
	
	uint16_t user_id;
	char user[10];
	char password[MAX_PASSWORD_LEN];
	uint8_t access_permissions;
	bool logon_session_expired;
	
} C1218_APL_TYPE;

/*
 ******************************************************************************
 * Global Function Definitions using extern declaration.
 ******************************************************************************
 */
extern void C1218_dll_reset(C1218_DLL_TYPE *c1218);
extern void C1218_send_response(C1218_DLL_TYPE *c1218);
extern void C1218_send_next_packet(C1218_DLL_TYPE *c1218);
extern void C1218_resend_packet(C1218_DLL_TYPE *c1218);
extern void C1218_send_ack(C1218_DLL_TYPE *c1218);
extern void C1218_send_nok(C1218_DLL_TYPE *c1218);
extern void C1218_send_ok(C1218_DLL_TYPE *c1218);
extern void C1218_send_err(C1218_DLL_TYPE *c1218);
extern void C1218_send_sns(C1218_DLL_TYPE *c1218);
extern void C1218_send_isc(C1218_DLL_TYPE *c1218);
extern void C1218_send_onp(C1218_DLL_TYPE *c1218);
extern void C1218_send_iar(C1218_DLL_TYPE *c1218);
extern void C1218_send_bsy(C1218_DLL_TYPE *c1218);
extern void C1218_send_dnr(C1218_DLL_TYPE *c1218);
extern void C1218_send_dlk(C1218_DLL_TYPE *c1218);
extern void C1218_send_rno(C1218_DLL_TYPE *c1218);
extern void C1218_send_isss(C1218_DLL_TYPE *c1218);
extern void C1218_rx_abort(C1218_DLL_TYPE *c1218);
extern void C1218_tx_abort(C1218_DLL_TYPE *c1218);
extern void C1218_rx_int_handler(C1218_DLL_TYPE *c1218);
extern void C1218_tx_int_handler(C1218_DLL_TYPE *c1218);

/**
 ******************************************************************************
 * \section REVISION FIRMWARE REVISION HISTORY (top to bottom: last revision to first revision)
 * --------------------------------------------------------------------------------------------
 * - Revision 01 / Author / YYYY-MM-DD
 *   - Change 1:  Description.
 *   - Change 2:  Description.
 ******************************************************************************
 */
#endif
