/*******************************************************************************
  Ethernet Library Interface Definition

  Summary:
    This file contains the Application Program Interface (API) definition  for 
    the Ethernet peripheral library.
    
  Description:
    This library provides a low-level abstraction of the Ethernet module 
    on Microchip PIC32MX family microcontrollers with a convenient C language 
    interface.  It can be used to simplify low-level access to the module 
    without the necessity of interacting directly with the module's registers, 
    thus hiding differences from one microcontroller variant to another.
*******************************************************************************/
//DOM-IGNORE-BEGIN
/*******************************************************************************
FileName:       eth.h
Processor:      PIC32MX
Compiler:       Microchip MPLAB C32 v1.06 or higher

Copyright © 2008-2009 released Microchip Technology Inc.  All rights
reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED “AS IS” WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
*******************************************************************************/
//DOM-IGNORE-END


#ifndef _ETH_H_
#define _ETH_H_

#include <stdlib.h>
#include <p32xxxx.h>

#ifdef _ETH	// the Ethernet device is present

// interface definitions
// 




// *****************************************************************************
// *****************************************************************************
// Section: Constants & Data Types
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Ethernet Operation Result Codes

  Summary:
    Defines the possible results of Ethernet operations that can succeed or fail

  Description:
    This enumeration defines the possible results of any of the I2C operations
    that have the possiblity of failing.  This result should be checked to 
    ensure that the operation achieved the desired result.
*/
typedef enum
{
    // Everything ok
    ETH_RES_OK,

    // Eth Rx, TX, acknowledge packets:

    // No such packet exist
    ETH_RES_NO_PACKET,

    // Packet is queued (not transmitted or received and not processed)
    ETH_RES_PACKET_QUEUED,

    // Eth buffers, descriptors errors:

    // Some memory allocation failed
    ETH_RES_OUT_OF_MEMORY,

    // Not enough descriptors available
    ETH_RES_NO_DESCRIPTORS,
    
    // We don't support user space buffers.
    ETH_RES_USPACE_ERR,

    // The size of the receive buffers too small
    ETH_RES_RX_SIZE_ERR,

    // A received packet spans more buffers/descriptors than supplied 
    ETH_RES_RX_PKT_SPLIT_ERR,

    // Negotiation errors:

    // No negotiation support
    ETH_RES_NEGOTIATION_UNABLE,

    // No negotiation active
    ETH_RES_NEGOTIATION_INACTIVE,

    // Negotiation not started yet
    ETH_RES_NEGOTIATION_NOT_STARTED,

    // Negotiation active
    ETH_RES_NEGOTIATION_ACTIVE,

    // Link down after negotiation, negotiation failed
    ETH_RES_NEGOTIATION_LINKDOWN,

    // PHY errors:

    // No Phy was detected or it failed to respond to reset command
    ETH_RES_DTCT_ERR,

    // No match between the capabilities: the Phy supported and the open 
    // requested ones
    ETH_RES_CPBL_ERR,

    // Hardware configuration doesn't match the requested open mode
    ETH_RES_CFG_ERR,

} eEthRes;


// *****************************************************************************
/* Ethernet Configuration Settings

  Summary:
    Supported configuration flags for the Ethernet module (EthMACOpen).

  Description:
    This enumeration defines the various configuration options for the I2C 
    module.  These values can be OR'd together to create a configuration mask
    passed to the I2CConfigure routine.
    
  Remarks:
    When Auto negotiation is specified:
        - If multiple capability flags are set (ETH_OPEN_FDUPLEX, 
          ETH_OPEN_HDUPLEX, ETH_OPEN_100, ETH_OPEN_10 ) they are all advertised
          as this link side capabilities.
        - If no setting is passed, the lowest one is taken, i.e. 
          ETH_OPEN_HDUPLEX and ETH_OPEN_10.
        - Auto-MDIX requires Auto-Negotiation; ETH_OPEN_MDIX_NORM or 
          ETH_OPEN_MDIX_SWAP setting irrelevant.
          
    When No Auto negotiation is specified:
        - If multiple settings, the highest priority setting is taken, i.e. 
          ETH_OPEN_FDUPLEX over ETH_OPEN_HDUPLEX and ETH_OPEN_100 over 
          ETH_OPEN_10.
        - If no setting, the lowest setting is taken, i.e. ETH_OPEN_HDUPLEX and
          ETH_OPEN_10.
        - The MDIX is set based on the ETH_OPEN_MDIX_NORM/ETH_OPEN_MDIX_SWAP
          setting.
*/
typedef enum
{
    // Link capabilities flags:

    // Use auto negotiation. set the following flags to specify your choices
    ETH_OPEN_AUTO
        /*DOM-IGNORE-BEGIN*/ = 0x1 /*DOM-IGNORE-END*/,

    // Use full duplex or full duplex negotiation capability needed
    ETH_OPEN_FDUPLEX
        /*DOM-IGNORE-BEGIN*/ = 0x2 /*DOM-IGNORE-END*/,

    // Use half duplex or half duplex negotiation capability needed
    ETH_OPEN_HDUPLEX
        /*DOM-IGNORE-BEGIN*/ = 0x4 /*DOM-IGNORE-END*/,

    // Use 100MBps or 100MBps negotiation capability needed
    ETH_OPEN_100
        /*DOM-IGNORE-BEGIN*/ = 0x8 /*DOM-IGNORE-END*/,

    // Use 10MBps or 10MBps negotiation capability needed
    ETH_OPEN_10
        /*DOM-IGNORE-BEGIN*/ = 0x10 /*DOM-IGNORE-END*/,

    // MAC flags:

    // Allow huge packets rx/tx
    ETH_OPEN_HUGE_PKTS
        /*DOM-IGNORE-BEGIN*/ = 0x20 /*DOM-IGNORE-END*/,

    // Loopbacked at the MAC level
    ETH_OPEN_MAC_LOOPBACK
        /*DOM-IGNORE-BEGIN*/ = 0x40 /*DOM-IGNORE-END*/,

    // When PHY is loopback-ed, negotiation will be disabled!
    ETH_OPEN_PHY_LOOPBACK
        /*DOM-IGNORE-BEGIN*/ = 0x80 /*DOM-IGNORE-END*/,

    // MDIX flags:

    // Use Auto MDIX
    ETH_OPEN_MDIX_AUTO
        /*DOM-IGNORE-BEGIN*/ = 0x100 /*DOM-IGNORE-END*/,

    // Use normal MDIX when Auto MDIX disabled
    ETH_OPEN_MDIX_NORM
        /*DOM-IGNORE-BEGIN*/ = 0x0 /*DOM-IGNORE-END*/,

    // Use swapped MDIX when Auto MDIX disabled
    ETH_OPEN_MDIX_SWAP
        /*DOM-IGNORE-BEGIN*/ = 0x200 /*DOM-IGNORE-END*/,

    // All capabilities default
    ETH_OPEN_DEFAULT = (ETH_OPEN_AUTO|ETH_OPEN_FDUPLEX|ETH_OPEN_HDUPLEX|
                        ETH_OPEN_100|ETH_OPEN_10|ETH_OPEN_MDIX_AUTO)

} eEthOpenFlags;


// *****************************************************************************
/* Ethernet Link Status Codes

  Summary:
    Defines the possible status flags of Ethernet link.

  Description:
    This enumeration defines the flags describing the status of the Ethernet 
    link.
  
  Remarks:
    Multiple flags can be set
*/
typedef enum
{
    // No connection to the LinkPartner
    ETH_LINK_ST_DOWN
        /*DOM-IGNORE-BEGIN*/ = 0x0 /*DOM-IGNORE-END*/,

    // Link is up
    ETH_LINK_ST_UP
        /*DOM-IGNORE-BEGIN*/ = 0x1 /*DOM-IGNORE-END*/,

    // LP non negotiation able
    ETH_LINK_ST_LP_NEG_UNABLE
        /*DOM-IGNORE-BEGIN*/ = 0x2 /*DOM-IGNORE-END*/,

    // LP fault during negotiation
    ETH_LINK_ST_REMOTE_FAULT
        /*DOM-IGNORE-BEGIN*/ = 0x4 /*DOM-IGNORE-END*/,

    // Parallel Detection Fault encountered (when ETH_LINK_ST_LP_NEG_UNABLE)
    ETH_LINK_ST_PDF
        /*DOM-IGNORE-BEGIN*/ = 0x8 /*DOM-IGNORE-END*/,

    // LP supports symmetric pause
    ETH_LINK_ST_LP_PAUSE
        /*DOM-IGNORE-BEGIN*/ = 0x10 /*DOM-IGNORE-END*/,

    // LP supports asymmetric TX/RX pause operation
    ETH_LINK_ST_LP_ASM_DIR
        /*DOM-IGNORE-BEGIN*/ = 0x20 /*DOM-IGNORE-END*/,

    // LP not there
    ETH_LINK_ST_NEG_TMO
        /*DOM-IGNORE-BEGIN*/ = 0x1000 /*DOM-IGNORE-END*/,

    // An unexpected fatal error occurred during the negotiation
    ETH_LINK_ST_NEG_FATAL_ERR
        /*DOM-IGNORE-BEGIN*/ = 0x2000 /*DOM-IGNORE-END*/,
    
} eEthLinkStat;


// *****************************************************************************
/* Ethernet Controller Status Codes

  Summary:
    Defines the possible status codes of Ethernet controller.

  Description:
    This enumeration defines the flags describing the status of the Ethernet 
    controller.
*/
typedef enum
{
    // A packet is currently received
    ETH_ST_RXBUSY   /*DOM-IGNORE-BEGIN*/ = 0x1 /*DOM-IGNORE-END*/,

    // A packet is currently transmitted
    ETH_ST_TXBUSY   /*DOM-IGNORE-BEGIN*/ = 0x2 /*DOM-IGNORE-END*/,

    // Module is on or completing a transaction
    ETH_ST_BUSY     /*DOM-IGNORE-BEGIN*/ = 0x4 /*DOM-IGNORE-END*/,

} eEthStat;


// *****************************************************************************
/* Ethernet Module Disable

  Summary:
    Defines the possible disable codes of Ethernet controller "EthClose" call.

  Description:
    This enumeration defines the close capabilities of the Ethernet module.
*/
typedef enum
{
    // Wait for the current tx/rx op to finish
    ETH_CLOSE_GRACEFUL  /*DOM-IGNORE-BEGIN*/ = 0x1 /*DOM-IGNORE-END*/,

    // Default close options
    ETH_CLOSE_DEFAULT = (0)

} eEthCloseFlags;


// *****************************************************************************
/* Ethernet MAC Pause Types

  Summary:
    Defines the possible Ethernet MAC pause types.

  Description:
    This enumeration defines the Ethernet MAC's pause capabilities.
*/
typedef enum
{
    // No PAUSE capabilities
    MAC_PAUSE_TYPE_NONE     /*DOM-IGNORE-BEGIN*/ = 0x0 /*DOM-IGNORE-END*/,

    // Supports symmetric PAUSE
    MAC_PAUSE_TYPE_PAUSE    /*DOM-IGNORE-BEGIN*/ = 0x1 /*DOM-IGNORE-END*/,

    // Supports ASM_DIR
    MAC_PAUSE_TYPE_ASM_DIR  /*DOM-IGNORE-BEGIN*/ = 0x2 /*DOM-IGNORE-END*/,

    // The previous two values converted to tx/rx capabilities:

    // Enable MAC tx pause support
    MAC_PAUSE_TYPE_EN_TX    /*DOM-IGNORE-BEGIN*/ = 0x4 /*DOM-IGNORE-END*/,

    // Enable MAC rx pause support
    MAC_PAUSE_TYPE_EN_RX    /*DOM-IGNORE-BEGIN*/ = 0x8 /*DOM-IGNORE-END*/,

    // All types of pause
    MAC_PAUSE_ALL       = (MAC_PAUSE_TYPE_PAUSE|MAC_PAUSE_TYPE_ASM_DIR|
                           MAC_PAUSE_TYPE_EN_TX|MAC_PAUSE_TYPE_EN_RX),

    // All pause capabilities our MAC supports
    MAC_PAUSE_CPBL_MASK = MAC_PAUSE_ALL

} eMacPauseType;


// *****************************************************************************
// *****************************************************************************
// Section: Ethernet Peripheral Library Interface Routines
// *****************************************************************************
// *****************************************************************************

// interface functions
// 


/**************************************
 *  Open and configuration functions
 **************************************************/

/****************************************************************************
 * Function:        EthInit
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          ETH_RES_OK
 *
 * Side Effects:    None
 *
 * Overview:        This function performs the initialization of the ETHC.
 *
 * Note:            This function should be called before the EthMACOpen()
 *****************************************************************************/
eEthRes		EthInit(void);


/****************************************************************************
 * Function:        EthClose
 *
 * PreCondition:    None
 *
 * Input:           cFlags - closing flags
 *
 * Output:          ETH_RES_OK  
 *
 * Side Effects:    None
 *
 * Overview:        This function performs the abort of any current Eth transaction.
 *                  It disables the Eth controller and the Eth interrupts.
 *                  It should be called after the PHY close/reset procedure. 
 *
 * Note:            It does not free any allocated memory.
 *****************************************************************************/
eEthRes		EthClose(eEthCloseFlags cFlags);

/****************************************************************************
 * Function:        EthMACOpen
 *
 * PreCondition:    EthInit called!
 *
 * Input:           oFlags - open flags
 *                  pauseType - pause type to be supported by the MAC
 *
 * Output:          None  
 *
 * Side Effects:    None
 *
 * Overview:        This function performs the open and configuration of the Eth MAC.
 *                  It needs to be called after an EthInit() was called and the PHY initialized.
 *                  
 *
 * Note:            - If no auto-negotiation is performed, the EthMACOpen() could be called immediately after EthInit() and PHY initialization.
 *                  - Otherwise the open flags and the pause type should be the ones obtained as a result of the PHY negotiation procedure! 
 *****************************************************************************/
void EthMACOpen(eEthOpenFlags oFlags, eMacPauseType pauseType);


/****************************************************************************
 * Function:        EthMACSetAddress
 *
 * PreCondition:    None
 *
 * Input:           bAddress - standard MAC address, 6 bytes, Network order!
 *
 * Output:          None  
 *
 * Side Effects:    None
 *
 * Overview:        This function sets the MAC address.
 *
 * Note:            The default MAC address is loaded by the device at reset from the fuses.
 *****************************************************************************/
void		EthMACSetAddress(unsigned char bAddress[6]);

/****************************************************************************
 * Function:        EthMACSetMaxFrame
 *
 * PreCondition:    Should be called after EthMACOpen()
 *
 * Input:           maxFrmSz - maximum frame for the MAC to tx/rx
 *
 * Output:          None  
 *
 * Side Effects:    None
 *
 * Overview:        This function sets the MAC maximum frame size.
 *
 * Note:            The default MAC frame size (0x600) is set by the EthMACOpen() call.
 *****************************************************************************/
void		EthMACSetMaxFrame(unsigned short maxFrmSz);



/**************************************
 *  Descriptors functions
 **************************************************/


typedef void* (*pEthDcptAllocF)(size_t nitems, size_t size);	// Eth descriptor allocation function
								// Allocates the requested memory and returns a pointer to it.
								// The requested size is nitems each size bytes long (total memory requested is nitems*size).
								// The space is initialized to all zero bits.
								// On success a pointer to the requested space is returned. On failure a null pointer is returned.
								// Note: this function will be called for allocation of descriptors only.
								// Therefore, the size parameter will be the one returned by EthDescriptorsGetSize();

typedef void (*pEthDcptFreeF)(void* ptr);			// Eth descriptor free function
								// Deallocates the memory previously allocated by a pEthDcptAllocF function.
								// The argument ptr points to the space that was previously allocated with a pEthDcptAllocF function.
								// No value is returned.

typedef enum
{
	ETH_DCPT_TYPE_RX	= 0x1,		// rx descriptor
	ETH_DCPT_TYPE_TX	= 0x2,		// tx descriptor
	//
	ETH_DCPT_TYPE_ALL	= (ETH_DCPT_TYPE_RX|ETH_DCPT_TYPE_TX)		// all/both types. Some descriptor operations support multiple types.
}eEthDcptType;		// descriptor types



typedef struct
{
	int	dIdle;			// the number of idle (unused) descriptors available in the pool of descriptors.
	int	dUnack;			// the number of descriptors not acknowledged
	int	dQueued;		// the number of queued descriptors 
}sEthDcptQuery;		// structure used for a query of the Eth descriptors


		
/****************************************************************************
 * Function:        EthDescriptorsAdd
 *
 * PreCondition:    EthMACOpen should have been called.
 *
 * Input:           nDescriptors - number of descriptors to be added
 *                  dType        - tx/rx descriptors requested
 *                  fAlloc       - function to be called to allocate the needed memory or NULL. 	
 *
 * Output:          the number of descriptors succesfully created
 *
 * Side Effects:    None
 *
 * Overview:        This function adds the specified number of transmit/receive descriptors to the pool of descriptors.
 *                  These descriptors are used whenever a new transmission or a reception is initiated.
 *                  The needed memory per descriptor is returned by EthDescriptorsGetSize();
 *                  Descriptors can be freed with EthDescriptorsRemove().
 *
 * Note:            - The number of created descriptors is different from the number of requested ones only if the memory allocation failed.
 *                  If fAlloc is 0 then the standard calloc() is used to get the needed memory.
 *                  - The complete clean-up is done by EthDescriptorsCleanUp();
 *                  - ETH_DCPT_TYPE_ALL use is invalid. A proper descriptor type (RX or TX) has to be used.
 *****************************************************************************/
int		EthDescriptorsAdd(int nDescriptors, eEthDcptType dType, pEthDcptAllocF fAlloc);

/****************************************************************************
 * Function:        EthDescriptorsRemove
 *
 * PreCondition:    EthMACOpen should have been called.
 *
 * Input:           nDescriptors - number of descriptors to be removed
 *                  dType        - tx/rx descriptors requested
 *                  fFree        - function to release the the memory or NULL
 *
 * Output:          the number of descriptors removed
 *
 * Side Effects:    None
 *
 * Overview:        This function tries to remove the specified number of descriptors from the pool of transmit/receive descriptors.
 *                  These descriptors that are NOT used at the time of call can be freed (i.e. the Tx descriptors not queued for a transmission
 *                  or the not busy Rx descriptors).
 *
 * Note:            - If fFree is 0 then the standard free() is used to release the allocated memory.
 *                  The allocation/deallocation memory functions should be consistent.
 *                  - The complete clean-up is done by EthDescriptorsCleanUp();
 *                  - ETH_DCPT_TYPE_ALL use is invalid. A proper descriptor type (RX or TX) has to be used.
 *****************************************************************************/
int		EthDescriptorsRemove(int nDescriptors, eEthDcptType dType, pEthDcptFreeF fFree);


/****************************************************************************
 * Function:        EthDescriptorsCleanUp
 *
 * PreCondition:    EthClose should have been called. 
 *
 * Input:           dType        - tx/rx/all descriptors requested
 *                  fFree        - function to release the the memory or NULL
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function performs descriptors clean-up and removes all the specified descriptors from the pool of transmit/receive descriptors.
 *
 * Note:            - If fFree is 0 then the standard free() is used to release the allocated memory.
 *                  The allocation/deallocation memory functions should be consistent.
 *                  - It does not free the memory allocated for the application supplied "sticky" buffers.
 *                  It is the calling app responsibility.
 *                  - EthClose should have been called or no tx/rx activity should be enabled at the time this function is called.
 *                  - If ETH_DCPT_TYPE_ALL is supplied the function will perform the clean-up of both types of descriptors.
 ******************************************************************************/
void		EthDescriptorsCleanUp(eEthDcptType dType, pEthDcptFreeF fFree);


/****************************************************************************
 * Function:        EthDescriptorsGetSize
 *
 * PreCondition:    None
 *
 * Input:           dType    - tx/rx/all descriptors requested
 *
 * Output:          the size of a descriptor
 *
 * Side Effects:    None
 *
 * Overview:        This function returns the number of bytes needed for the storage
 *                  of a descriptor in the transmit/receive engine.
 *
 * Note:            If ETH_DCPT_TYPE_ALL supplied the function will return them size that accomodates both descriptors (max). 
 *****************************************************************************/
int		EthDescriptorsGetSize(eEthDcptType dType);


/****************************************************************************
 * Function:        EthDescriptorsQuery
 *
 * PreCondition:    - EthMACOpen should have been called.
 *                  - pQuery valid pointer
 *
 * Input:           dType   - tx/rx descriptors requested
 *                  pQuery  - address to store the result of the query
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function takes a snapshot of the current status of RX/TX descriptors.
 *                  It returns the query result.
 *                  If dType == ETH_DCPT_TYPE_RX
 *                         dIdle: returns the number of unused (idle) descriptors
 *                                   available in the pool of RX descriptors.
 *                         dUnack: returns the number of descriptors that received a valid
 *                                   packet but have not been acknowledged yet
 *                         dQueued: returns the number of descriptors that are just queued for
 *                                   receiving, without a valid load.
 *                  If dType == ETH_DCPT_TYPE_TX
 *                         dIdle: returns the number of unused (idle) descriptors
 *                                   available in the pool of TX descriptors.
 *                         dUnack: returns the number of descriptors that have transmitted
 *                                   packets but have not been acknowledged yet
 *                         dQueued: returns the number of descriptors that are just queued for
 *                                   transmission, but not transmitted yet
 *
 * Note:            - The returned numbers are info only. They can dynamically change.
 *                  - No protection against interrupts, multithreading, etc.
 *                  - ETH_DCPT_TYPE_ALL use is invalid. A proper descriptor type (RX or TX) has to be used.
 *                  - Whenever the automatic flow control is turned on, the reception will come to a halt once the number of
 *                    unacknowledged RX descriptors (dUnack for query using ETH_DCPT_TYPE_RX) reaches 256.
 *                    Also the number of currently received and unacknowledged descriptors
 *                    control the behavior of the automatic flow control.
 *                    See EthFCEnable() for details.
 *****************************************************************************/
void		EthDescriptorsQuery(eEthDcptType dType, sEthDcptQuery* pQuery);


/****************************************************************************
 * Function:        EthDescriptorsGetRxUnack
 *
 * PreCondition:    EthMACOpen should have been called.
 *
 * Input:           None
 *
 * Output:          the number of rx descriptors that are currently unacknowledged
 *
 * Side Effects:    None
 *
 * Overview:        This function returns the number of rx descriptors that are properly received by the receive engine
 *                  and not acknowledged.
 *                  These descriptors have to be acknowledged by sw by calling EthRxAckPacket(). 
 *
 * Note:            - Fast shortcut for getting the number of un-acknowledged RX descriptors;
 *                  - Whenever the automatic flow control is turned on, the reception will come to a halt once this number reaches 256.
 *                  The number of currently received descriptors control the behavior of the automatic flow control.
 *                  See EthFCEnable() for details.
 *****************************************************************************/
extern __inline__ int __attribute__((always_inline)) EthDescriptorsGetRxUnack(void)
{
	return ETHSTATbits.BUFCNT;
}



/**************************************
 *  Tx/Rx functions
 **************************************************/


typedef struct _tag_sBuffDcpt 
{
	struct _tag_sBuffDcpt*	next;		// next descriptor in chain. NULL to end
	void*			pBuff;		// buffer to be transmitted
	unsigned short int	nBytes;		// number of bytes in the buffer 0-2047 allowed
}__attribute__ ((__packed__)) sBuffDcpt;	// descriptor of a buffer accepted by the Tx/Rx Ethernet engine
/*
A packet handled by the Ethernet Tx/Rx engine is a list of buffer descriptors.
A packet consists of multiple buffers and each buffer needs a descriptor.
Although the number of buffers per packet is not limited, note that the hardware
overhead is higher when many buffers have to be handled for one packet.
The list ends when the next field is NULL or when the pBuff is 0 
*/


typedef enum
{
	BUFF_FLAG_RX_STICKY	= 0x1,	// the receive buffer is sticky, going to be used across multiple rx operations once it's acknowledged
}eBuffFlags;	// buffer flags

typedef union
{
	struct
	{
		unsigned short totTxBytes;		// total bytes transmitted
		unsigned txCtrl		: 1;		// control frame transmitted
		unsigned txPause	: 1;		// pause control frame transmitted
		unsigned txBPres	: 1;		// transmit backpressure applied
		unsigned txVLAN		: 1;		// transmit VLAN tagged frame
		unsigned		: 12;
		//
		unsigned short bCount;			// transmit bytes count
		unsigned collCount	: 4;		// transmit collision count
		unsigned crcError	: 1;		// transmit CRC error
		unsigned lenError	: 1;		// transmit length check error
		unsigned lenRange	: 1;		// transmit length out of range
		unsigned txDone		: 1;		// transmit done
		unsigned mcast		: 1;		// transmit multicast
		unsigned bcast		: 1;		// transmit broadcast
		unsigned defer		: 1;		// transmit packet defer
		unsigned excDefer	: 1;		// transmit excessive packet defer
		unsigned maxColl	: 1;		// transmit maximum collision
		unsigned lateColl	: 1;		// transmit late collision
		unsigned giant		: 1;		// transmit giant frame (set when pktSz>MaxFrameSz && HugeFrameEn==0)
		unsigned underrun	: 1;		// transmit underrun
	}__attribute__ ((__packed__));
	unsigned long long		w;		// status is 2 words always
}sTxPktStat;	// transmitted packet status

typedef union
{
	struct 
	{
		unsigned pktChecksum	:16;		// packet payload checksum
		unsigned		: 8;
		unsigned runtPkt	: 1;		// runt packet received
		unsigned notMeUcast	: 1;		// unicast, not me packet
		unsigned htMatch	: 1;		// hash table match
		unsigned magicMatch	: 1;		// magic packet match
		unsigned pmMatch	: 1;		// pattern match match
		unsigned uMatch		: 1;		// unicast match
		unsigned bMatch		: 1;		// broadcast match
		unsigned mMatch		: 1;		// multicast match
		//
		unsigned rxBytes	:16;		// received bytes
		unsigned prevIgnore	: 1;		// packet previously ignored
		unsigned prevDV		: 1;		// rx data valid event previously seen
		unsigned prevCarrier	: 1;		// carrier event previously seen
		unsigned rxCodeViol	: 1;		// rx code violation
		unsigned crcError	: 1;		// CRC error in packet
		unsigned lenError	: 1;		// receive length check error
		unsigned lenRange	: 1;		// receive length out of range
		unsigned rxOk		: 1;		// receive OK
		unsigned mcast		: 1;		// multicast packet
		unsigned bcast		: 1;		// broadcast packet
		unsigned dribble	: 1;		// dribble nibble
		unsigned rxCtrl		: 1;		// control frame received
		unsigned rxPause	: 1;		// pause control frame received
		unsigned rxCodeErr	: 1;		// received unsupported code
		unsigned rxVLAN		: 1;		// received VLAN tagged frame
		unsigned 		: 1;
	}__attribute__ ((__packed__));
	unsigned long long		w;		// status is 2 words always	
}sRxPktStat;	// received packet status



typedef void (*pEthPktAckF)(void* pPktBuff, int	buffIx);		// Eth buffer acnowledge callback function
									// This function is meant to be used for Tx acnowledge
									// However, the Rx acknowledge accepts a callback as a parameter
									// the 1st parameter is a pointer to the currently acknowledged transmitted/received  buffer
									// the 2nd parameter represents the 0 based buffer index for a packet that spans multiple buffers

/****************************************************************************
 * Function:        EthTxSendBuffer
 *
 * PreCondition:    EthMACOpen, EthDescriptorsAdd should have been called.
 *                  buffers specified should be <2048 bytes in size
 *
 * Input:           pBuff   - adress of buffer to be sent
 *                  nBytes  - size of the buffer in bytes
 *
 * Output:          ETH_RES_OK for success,
 *                  an error code otherwise
 *
 * Side Effects:    None
 *
 * Overview:        This function schedules the supplied buffer for transmission.
 *                  To transmit this buffer an associated transmit descriptor is needed.
 *                  Therefore, a transmit descriptor should be available for this function to succeed.
 *                  Once a transmission is scheduled and done, EthTxGetBufferStatus() can be called to check the transmission result.  
 *
 * Note:            - Not multithreaded safe. Don't call from from both ISR -non ISR code or multiple ISR's!
 *                  - This function enables the Ethernet transmission.
 *                  - This function is for transmission of packets completely assembled in one buffer.
 *****************************************************************************/
eEthRes		EthTxSendBuffer(const void* pBuff, unsigned short int nBytes);


/****************************************************************************
 * Function:        EthTxSendPacket
 *
 * PreCondition:    EthMACOpen, EthDescriptorsAdd should have been called.
 *                  buffers specified should be <2048 bytes in size
 *
 * Input:           pPkt   - packet descriptor consisting of a list of buffer descriptors, terminated by a NULL pointer.
 *
 * Output:          ETH_RES_OK for success,
 *                  an error code otherwise
 *
 * Side Effects:    None
 *
 * Overview:        This function schedules the supplied packet for transmission.
 *                  Each packet can consist of multiple buffers, each having a different size, as specified in the packet descriptor.
 *                  Each buffer will need an associated transmit descriptor. Therefore, the number of needed
 *                  transmit descriptors should be available for this function to succeed.
 *                  Once a transmission is scheduled and done, EthTxGetPacketStatus() can be called to check the transmission result.  
 *
 * Note:            - Not multithreaded safe. Don't call from from both ISR -non ISR code or multiple ISR's!
 *                  - This function enables the Ethernet transmission.
 *                  - The buffers assembled for the packet stops when a NULL buffer descriptor is retrieved or the list ends
 *****************************************************************************/
eEthRes		EthTxSendPacket(const sBuffDcpt* pPkt);


/****************************************************************************
 * Function:        EthTxGetBufferStatus
 *
 * PreCondition:    EthTxSendPacket should have been called.
 *                  pBuff valid pointer
 *
 * Input:           pBuff   - buffer to query the status for
 *                  pTxStat - address to store the pointer to the packet status or NULL if status not needed
 *
 * Output:          ETH_RES_OK if the packet was transmitted OK,
 *                  ETH_RES_PACKET_QUEUED if the packet is not done yet
 *                  ETH_RES_NO_PACKET if no such packet exists
 *
 * Side Effects:    None
 *
 * Overview:        This function returns the status of a transmitted buffer/packet.
 *                  The transmission of this packet has to have been completed otherwise the call will fail.
 *
 * Note:            The returned pointer to the packet status is invalid after the EthTxAckPacket() is called.
 *                  pBuff must be the pointer to the first buffer in the packet, if the packet spans multiple buffers. 
 *****************************************************************************/
eEthRes		EthTxGetBufferStatus(const void* pBuff, const sTxPktStat** pTxStat);

/****************************************************************************
 *   Helper to retrieve the status of packet using the full packet descriptor
 *****************************************************************************/
extern __inline__ eEthRes __attribute__((always_inline)) EthTxGetPacketStatus(const sBuffDcpt* pPkt, const sTxPktStat** pTxStat)
{
	return 	EthTxGetBufferStatus(pPkt?pPkt->pBuff:0, pTxStat);
}


/****************************************************************************
 * Function:        EthTxAckBuffer
 *
 * PreCondition:    EthTxSendPacket should have been called.
 *
 * Input:           pBuff  - buffer/packet to be acknowledged or NULL.
 *                  ackFnc - function to be called for the acknowledged buffers	or NULL
 *
 * Output:          ETH_RES_OK - success
 *                  ETH_RES_PACKET_QUEUED - there are packets scheduled to be transmitted
 *                  ETH_RES_NO_PACKET - no packets scheduled for transmit
 *
 * Side Effects:    None
 *
 * Overview:        This function acknowledges a transmitted buffer/packet.
 *                  The transmission of this packet has to have been completed otherwise the call will fail.
 *                  When pBuff==NULL, all currently transmitted packets will be acknowledged.
 *                  The ackFnc, if !NULL, will be called for each buffer within the packet in turn.  
 *
 * Note:            - Any transmitted packet has to be acknowledged, otherwise the Eth API will run out of transmission descriptors.
 *                  - pBuff must be the pointer to the first buffer in the packet, if the packet spans multiple buffers.
 *                  - ackFnc is just a helper that allows the application to acknowledge the transmitted buffers without the need
 *                  to maintain a list of buffers scheduled for transmission (this list is maintained by the Eth library anyway).
 *                  Useful especially when the transmission result is not really needed and the function is called with pPkt=0.
 *****************************************************************************/
eEthRes		EthTxAckBuffer(const void* pBuff, pEthPktAckF ackFnc);

/****************************************************************************
 *   Helper to acknowledge a transmitted packet using the full packet descriptor
 *****************************************************************************/
extern __inline__ eEthRes __attribute__((always_inline)) EthTxAckPacket(const sBuffDcpt* pPkt, pEthPktAckF ackFnc)
{
	return 	EthTxAckBuffer(pPkt?pPkt->pBuff:0, ackFnc); 
}

/****************************************************************************
 * Function:        EthRxSetBufferSize
 *
 * PreCondition:    0 < rxBuffSize <= 2032
 *
 * Input:           rxBuffSize - size of the rx buffers
 *
 * Output:          ETH_RES_OK for success,
 *                  an error code otherwise
 *
 * Side Effects:    None
 *
 * Overview:        This function sets the required buffer size for the receive operation.
 *                  In this implementation, all receive descriptors use the same buffer size (unlike the transmission flow
 *                  where each descriptor can have a different buffer size).
 *
 * Note:            - This function should be part of the initialization process and shoult NOT be called when the rx process is active!
 *                  - The receive buffer size is always TRUNCATED to a multiple of 16 bytes.
 *****************************************************************************/
eEthRes		EthRxSetBufferSize(int rxBuffSize);


/****************************************************************************
 * Function:        EthRxBuffersAppend
 *
 * PreCondition:    EthMACOpen, EthDescriptorsAdd, EthRxSetBufferSize should have been called.
 *                  each buffer supplied should be >= EthRxSetBufferSize().
 *
 * Input:           ppBuff - pointer to an array of buffers (could be NULL terminated) to be appended to the receiving process
 *                  nBuffs - number of buffers supplied (or 0 if ppBuff is NULL terminated)
 *                  rxFlags  - flags applied to all RX buffers passed
 *
 * Output:          ETH_RES_OK for success,
 *                  an error code otherwise
 *
 * Side Effects:    None
 *
 * Overview:        This function supplies buffers to the receiving process and enables the receiving part of the controller.
 *                  As soon as the controller starts receiving data packets these will be stored into memory
 *                  at the addresses specified by these buffers. 
 *                  A received packet can consist of multiple buffers, split across buffers with the SAME size, as specified in the EthRxSetBufferSize().
 *                  Each buffer needs an associated receive descriptor. Therefore, the number of needed
 *                  receive descriptors should be available for this function to succeed.
 *                  Once a receive operation is scheduled, EthRxPacket() can be called to get the received packet.  
 *
 * Note:            - Not multithreaded safe. Don't call from from both ISR -non ISR code or multiple ISR's!
 *                  - This function enables the Ethernet receiving.
 *                  - When a packet is split into multiple buffers, all buffers have the same size, set by the EthRxSetBufferSize().
 *                  - The append process continues until a NULL buffer pointer is retrieved or nBuffs buffers are appended.
 *                  - Only RX eBuffFlags are relevant for this function 
 *****************************************************************************/
eEthRes		EthRxBuffersAppend(unsigned char* ppBuff[], int nBuffs, eBuffFlags rxFlags);

/****************************************************************************
 * Function:        EthRxGetBuffer
 *
 * PreCondition:    EthRxSetBufferSize, EthRxBuffersAppend should have been called.
 *
 * Input:           ppBuff  - address of a pointer to packet buffer data or NULL
 *                  pRxStat - address to store the pointer to the packet status or NULL
 *
 * Output:          ETH_RES_OK - for success,
 *                  ETH_RES_PACKET_QUEUED - there are packets queued for receive but not completed yet
 *                  ETH_RES_NO_PACKET - no packets available in the receiving queue
 *                  ETH_RES_RX_PKT_SPLIT_ERR - packet spans accross multiple buffers 
 *
 * Side Effects:    None
 *
 * Overview:        This function returns a pointer to the data and the status of the next available received packet.
 *                  If there's no available fully received packet the call will fail (ETH_RES_PACKET_QUEUED/ETH_RES_NO_PACKET).
 *                  If the received packet spans multiple buffers/descriptors (i.e. when the size of the buffer,
 *                  EthRxSetBufferSize() is less than the packet size) ETH_RES_RX_PKT_SPLIT_ERR will be returned.
 *                  However, if the ppBuff is NULL, than the caller can use the returned pRxStat to check the
 *                  total size of the packet. Then using the size of the set buffers, the number of buffers used in
 *                  the packet will result.
 *
 * Note:            - The returned status pointer is invalid after the EthRxAckBuffer() is called.
 *                    Also the associated packet buffer if it was appended with a BUFF_FLAG_RX_STICKY flag.
 *                  - The returned buffer, if existent, will be marked as reported only when the result returned is ETH_RES_OK.
 *                    In this case a new call to the function will not return the same buffer.
 *                  - If the call failed for some reason (result!=ETH_RES_OK) the returned buffer is not marked as reported.
 *                    A new call to this function will return the same buffer.
 *****************************************************************************/
eEthRes		EthRxGetBuffer(void** ppBuff, const sRxPktStat** pRxStat);


/****************************************************************************
 * Function:        EthRxGetPacket
 *
 * PreCondition:    EthRxSetBufferSize, EthRxBuffersAppend should have been called.
 *
 * Input:           pPkt    - pointer to a packet descriptor: a list of buffer descriptors large enough to receive the packet layout or NULL
 *                  pnBuffs - pointer to store the number of buffers in the packet or NULL.
 *                  pRxStat - address to store the pointer to the packet status or NULL
 *
 * Output:          ETH_RES_OK - for success,
 *                  ETH_RES_PACKET_QUEUED - there are packets queued for receive but not completed yet
 *                  ETH_RES_NO_PACKET - no packets available in the receiving queue
 *                  ETH_RES_RX_PKT_SPLIT_ERR - packet descriptor not large enough to store the whole packet 
 *
 * Side Effects:    None
 *
 * Overview:        This function returns the layout and the status of the next available received packet.
 *                  If there's no available fully received packet the call will fail (ETH_RES_PACKET_QUEUED/ETH_RES_NO_PACKET).
 *                  The supplied pPkt has to be large enough to accomodate the packet layout in the case
 *                  of packets split across multiple buffers/descriptors (i.e. when the size of the buffer,
 *                  EthRxSetBufferSize() is less than the packet size) otherwise ETH_RES_RX_PKT_SPLIT_ERR will be returned.
 *                  However, if the pPkt is NULL, than the caller can use the returned pRxStat to check the
 *                  total size of the packet. Then using the size of the set buffers, the number of buffers in
 *                  pPkt will result.
 *                  pnBuffs can be used for the same purpose. If pnBuffs is supplied, the numbers of buffers
 *                  in the packet will be returned.
 *
 * Note:            - The packet list, when supplied, is terminated with a NULL buffer
 *                  - The returned status pointer is invalid after the EthRxAckPacket() is called.
 *                    Also any buffers that were appended with a BUFF_FLAG_RX_STICKY flag associated to the packet buffers. 
 *                  - The returned packet, if existent, will be marked as reported only when the result returned is ETH_RES_OK.
 *                    In this case a new call to the function will not return the same packet.
 *                  - If the call failed for some reason (result!=ETH_RES_OK) the returned packet is not marked as reported.
 *                    A new call to this function will return the same packet.
 *****************************************************************************/
eEthRes		EthRxGetPacket(sBuffDcpt* pPkt, int* pnBuffs, const sRxPktStat** pRxStat);


/****************************************************************************
 * Function:        EthRxAckBuffer
 *
 * PreCondition:    EthRxSetBufferSize, EthRxBuffersAppend, EthRxGetPacket should have been called.
 *
 * Input:           pBuff  - buffer/packet to be acknowledged or NULL
 *                  ackFnc - function to be called for the acknowledged buffers or NULL
 *
 * Output:          ETH_RES_OK - success
 *                  ETH_RES_PACKET_QUEUED - there are packets in the receiving queue
 *                  ETH_RES_NO_PACKET - no packets available in the receiving queue
 *
 * Side Effects:    None
 *
 * Overview:        This function acknowledges a received buffer/packet.
 *                  The supplied packet has to have been previously received otherwise the call will fail or the packet will be discarded.
 *                  When pBuff==NULL, all currently received packets will be acknowledged.  
 *                  The ackFnc, if !NULL, will be called for each buffer within the packet in turn.  
 *
 * Note:            - Any received packet has to be acknowledged, otherwise the Eth API will run out of descriptors.
 *                  - pBuff must be the pointer to the first buffer in the packet, if the packet spans multiple buffers.
 *                  - ackFnc is just a helper that allows the application to call an acknowledge function for each received buffer in turn.
 *****************************************************************************/
eEthRes		EthRxAckBuffer(const void* pBuff, pEthPktAckF ackFnc);


/****************************************************************************
        Helper to acknowledge a received packet using the full packet descriptor
 *****************************************************************************/
extern __inline__ eEthRes __attribute__((always_inline)) EthRxAckPacket(const sBuffDcpt* pPkt, pEthPktAckF ackFnc)
{
	return EthRxAckBuffer(pPkt?pPkt->pBuff:0, ackFnc);
}

/**************************************
 *  Status functions
 **************************************************/


/****************************************************************************
 * Function:        EthStatusGet
 *
 * PreCondition:    EthMACOpen should have been called.
 *
 * Input:           None
 *
 * Output:          current ethernet controller status  
 *
 * Side Effects:    None
 *
 * Overview:        This function returns the current controller status.
 *
 * Note:            None
 *****************************************************************************/
eEthStat	EthStatusGet(void);


/**************************************
 *  Flow Control functions
 **************************************************/

typedef enum
{
	ETH_FC_AUTO	= 0,		// auto flow control
	ETH_FC_SOFTWARE,		// software flow control
}eEthFc;	// types of Eth supported flow control

/****************************************************************************
 * Function:        EthFCSetPauseValue
 *
 * PreCondition:    EthMACOpen should have been called.
 *                  pauseBytes > 64, pauseBytes < 4194304
 *
 * Input:           pauseBytes	- the number of bytes to pause
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function sets the value of the pause value to be used with manual or auto flow control.
 *                  The pause time is calculated as being the time needed to transmit pauseBytes bytes.
 *
 * Note:            The pauseBytes value is relevant for full duplex mode only.
 *                  In half duplex, back pressure is applied.
 *                  Should be called only when receive is not enabled (i.e. before EthRxBuffersAppend() is called, for example).
 *****************************************************************************/
void		EthFCSetPauseValue(unsigned int pauseBytes);

/***************************************************************************
 * Function:        EthFCEnable
 *
 * PreCondition:    EthMACOpen, EthFCSetPauseValue should have been called.
 *
 * Input:           fcType - type of the flow control desired: auto or software (manual)
 *                  enable - boolean to enable/disable the flow control
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        - When enabling software flow control:
 *                    In full duplex mode, this function immediately enables the manual flow control and sends a pause frame
 *                    with the desired pause time.
 *                    Further on, a pause frame is resent, every interval corresponding to pause time/2.
 *                    In half duplex mode, back pressure is applied.
 *                  - When disabling software flow control:
 *                    In full duplex, this function immediately disables the manual flow control and sends a pause frame with the pause time set to 0.
 *                    In half duplex, the back pressure is removed.
 *                  - When enabling the auto flow control:
 *                    As soon as the number of the unacknowledged received descriptors (BUFCNT=EthDescriptorsGetRxUnack()) becomes >= fullWM:
 *                      In full duplex mode a pause frame is sent with the specified pause time (EthFCSetPauseValue).
 *                      Further on, a pause frame is resent, every interval corresponding to pause time/2.
 *                      In half duplex mode, back pressure is applied.
 *                    As soon as the BUFCNT becomes = emptyWM:
 *                      In full duplex mode a pause frame is sent with the pause value set to 0.
 *                      In half duplex mode, the back pressure is removed.
 *                  - When disabling suto flow control:
 *                    In full duplex, this function immediately disables the flow control and sends a pause frame with the pause time set to 0.
 *                    In half duplex, the back pressure is removed.
 *
 * Note:            BUFCNT is decremented by a call to EthRxAckPacket().
 *****************************************************************************/
void		EthFCEnable(eEthFc fcType, int enable);


/****************************************************************************
 * Function:        EthFCSetRxWMark
 *
 * PreCondition:    EthMACOpen, should have been called.
 *                  0<= emptyWM < fullWM < 256
 *
 * Input:           fullWM	- the full watermark value
 *                  emptyWM	- the empty watermark value
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function sets the values for the watermarks used in the automatic flow control.
 *
 * Note:            See the description of the automatic flow control in the EthFCEnable description.
 *                  To take into account the delays inherent to the traffic stop, the value for fullWM
 *                  should be set in such a way that the automatic flow control kicks in
 *                  when there's less than 3 frames (3*1536 bytes) available in the receive descriptors.
 *****************************************************************************/
void		EthFCSetRxWMark(int fullWM, int emptyWM);


/**************************************
 *  Rx filtering functions
 **************************************************/


typedef enum
{
	ETH_FILT_CRC_ERR_ACCEPT		= _ETHRXFC_CRCERREN_MASK,	// frames with wrong CRC are accepted
	ETH_FILT_RUNT_ACCEPT		= _ETHRXFC_RUNTERREN_MASK,	// runt frames accepted
	ETH_FILT_CRC_ERR_REJECT		= _ETHRXFC_CRCOKEN_MASK,	// frames with wrong CRC are rejected
	ETH_FILT_RUNT_REJECT		= _ETHRXFC_RUNTEN_MASK,		// runt frames rejected
	ETH_FILT_ME_UCAST_ACCEPT	= _ETHRXFC_UCEN_MASK,		// me unicast accepted
	ETH_FILT_NOTME_UCAST_ACCEPT	= _ETHRXFC_NOTMEEN_MASK,	// not me unicast accepted
	ETH_FILT_MCAST_ACCEPT		= _ETHRXFC_MCEN_MASK,		// multicast accepted
	ETH_FILT_BCAST_ACCEPT		= _ETHRXFC_BCEN_MASK,		// broadcast accepted
	ETH_FILT_HTBL_ACCEPT		= _ETHRXFC_HTEN_MASK,		// hash table matches destination address accepted
	ETH_FILT_MAGICP_ACCEPT		= _ETHRXFC_MPEN_MASK,		// magic packet accepted
	// ETH_FILT_PMATCH_ACCEPT					// packet match accepted. Use EthRxFiltersPMSet() to set the pattern match filter
	ETH_FILT_ALL_FILTERS		= _ETHRXFC_CRCERREN_MASK|_ETHRXFC_RUNTERREN_MASK|_ETHRXFC_CRCOKEN_MASK|_ETHRXFC_RUNTEN_MASK|_ETHRXFC_UCEN_MASK|
						_ETHRXFC_NOTMEEN_MASK|_ETHRXFC_MCEN_MASK|_ETHRXFC_BCEN_MASK|_ETHRXFC_HTEN_MASK|_ETHRXFC_MPEN_MASK
}eEthRxFilters; // ethernet receive filters flags; multiple values can be OR-red.
// NOTE:  the values in this enum are in the order of priorities the receive filter state machine works, with the highest priority first
// Once a filter accepts or rejects a packet, further filters are not tried.
// if a packet isn't rejected/accepted after all filters are tried, it will be rejected by default!


typedef enum
{
	ETH_FILT_PMATCH_ACCEPT			= 1,	// simple Pattern Match accepted
	ETH_FILT_PMATCH_ME_UCAST_ACCEPT,		// Pattern Match AND destination==me
	ETH_FILT_PMATCH_NOTME_UCAST_ACCEPT,		// Pattern Match AND destination!=me
	ETH_FILT_PMATCH_MCAST_ACCEPT,			// Pattern Match AND destination!=unicast
	ETH_FILT_PMATCH_NOT_MCAST_ACCEPT,		// Pattern Match AND destination==unicast
	ETH_FILT_PMATCH_BCAST_ACCEPT,			// Pattern Match AND destination==broadcast
	ETH_FILT_PMATCH_NOT_BCAST_ACCEPT,		// Pattern Match AND destination!=broadcast
	ETH_FILT_PMATCH_HTBL_ACCEPT,			// Pattern Match AND hash table filter match (irrespective of the ETH_FILT_HTBL_ACCEPT setting)
	ETH_FILT_PMATCH_MAGICP_ACCEPT,			// Pattern Match AND packet ==magic packet


	ETH_FILT_PMATCH_INVERT			= 0x80000000	// if set, the pattern must NOT match for a successful Pattern Match to occur!
}eEthPMatchMode; // Pattern Match modes, not flags. However ETH_FILT_PMATCH_INVERT act as a flag and can be applied to any value


// filter interface functions
// 

/****************************************************************************
 * Function:        EthRxFiltersSet
 *
 * PreCondition:    This function should not be called when Eth rx operation is enabled.
 *
 * Input:           rxFilters - rx filters that are to be set
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function sets the acceptance/rejection filters for the Eth receive mechanism.
 *                  Multiple filters can be OR-red together.
 *                  All filter specified in rxFilters will be set.          
 *
 * Note:            All filters except Pattern Match can be set. Use EthRxFiltersPMSet();
 *                  Before enabling the Hash Table filter, the hash table values should be set using EthRxFiltersHTSet(). 
 *****************************************************************************/
void		EthRxFiltersSet(eEthRxFilters rxFilters);

/****************************************************************************
 * Function:        EthRxFiltersClr
 *
 * PreCondition:    This function should not be called when Eth rx operation is enabled.
 *
 * Input:           rxFilters - rx filters that are to be cleared
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function clears the acceptance/rejection filters for the Eth receive mechanism.
 *                  Multiple filters can be OR-red together.         
 *                  All filters specified in rxFilters will be cleared.          
 *
 * Note:            All filters except Pattern Match can be cleared. Use EthRxFiltersPMClr();
 *****************************************************************************/
void		EthRxFiltersClr(eEthRxFilters rxFilters);


/****************************************************************************
 * Function:        EthRxFiltersWrite
 *
 * PreCondition:    This function should not be called when Eth rx operation is enabled.
 *
 * Input:           rxFilters - RX filters that are to be written
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function updates the acceptance/rejection filters for the Eth receive mechanism to the required value.
 *
 * Note:            All filters except Pattern Match can be updated.
 *                  The Pattern Match filter will be disabled by this call.  Use EthRxFiltersPMSet();
 *                  Before enabling the Hash Table filter, the hash table values should be set using EthRxFiltersHTSet(). 
 *****************************************************************************/
void		EthRxFiltersWrite(eEthRxFilters rxFilters);


/****************************************************************************
 * Function:        EthRxFiltersHTSet
 *
 * PreCondition:    This function should not be called when Eth rx operation is enabled and the hash filter is active.
 *
 * Input:           htable - the hash table itself
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function sets the hash table for the hash table rx filter.       
 *
 * Note:            Properly set the hash table with this function before enabling the Hash Table filter.
 *****************************************************************************/
void		EthRxFiltersHTSet(unsigned long long htable);


/****************************************************************************
 * Function:        EthRxFiltersPMSet
 *
 * PreCondition:    This function should not be called when Eth rx operation is enabled.
 *
 * Input:           mode          - the required Pattern Match Mode
 *                  matchMask     - mask in the 64 byte window  
 *                  matchOffs     - the offset applied to the incoming data (0 to 8128)
 *                  matchChecksum - the 16 bit checksum used for comparison
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function enables the Pattern Match filter with the specified settings for the
 *                  offset and window mask. The calculated checksum of the 64 bytes window
 *                  starting at matchOffs and using the matchMask bytes in the window, is compared against
 *                  matchChecksum.
 *                  If ETH_FILT_PMATCH_INVERT is used in the mode, then the calculated checksum must not
 *                  match the passed matchChecksum in order for the Pattern Match to succeed.         
 *
 * Note:            None            
 *****************************************************************************/
void		EthRxFiltersPMSet(eEthPMatchMode mode, unsigned long long matchMask, unsigned int matchOffs, unsigned int matchChecksum);

/****************************************************************************
 * Function:        EthRxFiltersPMClr
 *
 * PreCondition:    This function should not be called when Eth rx operation is enabled.
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function disables the Pattern Match receive filter.        
 *
 * Note:            None            
 *****************************************************************************/
void		EthRxFiltersPMClr(void);


/**************************************
 *  Ethernet event functions
 **************************************************/

typedef enum
{
	ETH_EV_RXOVFLOW			= _ETHIRQ_RXOVFLW_MASK,		// Rx FIFO overflow
	ETH_EV_RXBUFNA			= _ETHIRQ_RXBUFNA_MASK,		// Rx buffer not available (descriptor overrun)
	ETH_EV_TXABORT			= _ETHIRQ_TXABORT_MASK,		// Tx abort condition
	ETH_EV_TXDONE			= _ETHIRQ_TXDONE_MASK,		// Tx done
	ETH_EV_RXACT			= _ETHIRQ_RXACT_MASK,		// Rx activity
	ETH_EV_PKTPEND			= _ETHIRQ_PKTPEND_MASK,		// Rx packet pending
	ETH_EV_RXDONE			= _ETHIRQ_RXDONE_MASK,		// Rx packet sucessfully received
	ETH_EV_FWMARK			= _ETHIRQ_FWMARK_MASK,		// Full watermark reached
	ETH_EV_EWMARK			= _ETHIRQ_EWMARK_MASK,		// Empty watermark reached
	ETH_EV_RXBUSERR			= _ETHIRQ_RXBUSE_MASK,		// Rx bus error
	ETH_EV_TXBUSERR			= _ETHIRQ_TXBUSE_MASK,		// Tx bus error

	ETH_EV_ALL=			(ETH_EV_RXOVFLOW|ETH_EV_RXBUFNA|ETH_EV_TXABORT|ETH_EV_TXDONE|ETH_EV_RXACT|ETH_EV_PKTPEND|
						ETH_EV_RXDONE|ETH_EV_FWMARK|ETH_EV_EWMARK|ETH_EV_RXBUSERR|ETH_EV_TXBUSERR)	// all events	
}eEthEvents;	// events that can generate interrupts




/*********************************************************************
 * Function:        void EthEventsEnableSet(eEthEvents eEvents)
 *
 * PreCondition:    None
 *
 * Input:           eEvents	- events with the significance described in the eEthEvents definition.
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The function enables the events that will generate interrupts for the Eth controller.
 *                  Multiple events can be orr-ed together.
 *                  Any event that is set in the eEvents will be enabled , the other events won't be touched.
 *
 * Note:            None.
 *
 ********************************************************************/
 void		EthEventsEnableSet(eEthEvents eEvents);

/*********************************************************************
 * Function:        void EthEventsEnableClr(eEthEvents eEvents)
 *
 * PreCondition:    None
 *
 * Input:           eEvents	- events with the significance described in the eEthEvents definition.
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The function disables the events that will generate interrupts for the Eth controller.
 *                  Multiple events can be orr-ed together.
 *                  Any event that is set in the eEvents will be disabled , the other events won't be touched.
 *
 * Note:            None.
 ********************************************************************/
 void		EthEventsEnableClr(eEthEvents eEvents);

/*********************************************************************
 * Function:        void EthEventsEnableWrite(eEthEvents eEvents)
 *
 * PreCondition:    None
 *
 * Input:           eEvents	- events with the significance described in the eEthEvents definition.
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The function enables the events that will generate interrupts for the Eth controller.
 *                  The enabled events are forced to the eEvents value.
 *
 * Note:            None.
 ********************************************************************/
void		EthEventsEnableWrite(eEthEvents eEvents);

/*********************************************************************
 * Function:        eEthEvents EthEventsEnableGet(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          eEvents	- events with the significance described in the eEthEvents definition.
 * 
 * Side Effects:    None
 *
 * Overview:        The function returns the enabled events for the Eth controller.
 *
 * Note:            None.
 ********************************************************************/
 eEthEvents	EthEventsEnableGet(void);

/*********************************************************************
 * Function:        void EthEventsClr(eEthEvents eEvents)
 *
 * PreCondition:    None
 *
 * Input:           eEvents	- events with the significance described in the eEthEvents definition.
 * 
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The function clears the selected events for the Eth controller.
 *                  Multiple events can be orr-ed together. Any event that is set in the eEvents will be
 *                  cleared, the other events won't be touched.
 *
 * Note:            The ETH_EV_FWMARK cannot be cleared directly. It is cleared indirectly by EthRxAckPacket().
 *                  The ETH_EV_EWMARK cannot be cleared directly. It is cleared by hw when receiving a new packet.
 *                  The ETH_EV_PKTPEND cannot be cleared directly. It is cleared indirectly by EthRxAckPacket().
 ********************************************************************/
extern __inline__ void __attribute__((always_inline)) EthEventsClr(eEthEvents eEvents)
{
	ETHIRQCLR=eEvents;	
}


/*********************************************************************
 * Function:        eEthEvents EthEventsGet(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          events with the significance described in the eEthEvents definition.
 *
 * Side Effects:    None
 *
 * Overview:        The function returns the active events for the Eth controller.
 *
 * Note:            None.
 ********************************************************************/
extern __inline__ eEthEvents __attribute__((always_inline)) EthEventsGet(void)
{
	return (eEthEvents)ETHIRQ;
}

/**************************************
 *  Ethernet statistics registers access
 **************************************************/

/*********************************************************************
 * Function:        int EthStatRxOvflCnt(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          the number of dropped receive frames
 *
 * Side Effects:    None
 *
 * Overview:        The function returns the current number of dropped receive frames by the Eth controller.
 *                  These are frames accepted by the RX filter but dropped due to internal receive error (RX FIFO overrun).
 *
 * Note:            This statistics register is cleared by the read operation.
 *                  An RX overflow event is signalled by the ETH_EV_RXOVFLOW event (see eEthEvents definition).
 ********************************************************************/
#define		EthStatRxOvflCnt()	(ETHRXOVFLOW)

/*********************************************************************
 * Function:        int EthStatRxOkCnt(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          the number of frames received ok 
 *
 * Side Effects:    None
 *
 * Overview:        The function returns the current number of sucessfully received frames by the Eth controller.
 *                  These are frames accepted by the RX filter (some of them may still be dropped because of an RX FIFO overrun).
 *
 * Note:            This statistics register is cleared by the read operation.
 *                  Frames with FCS or alignment errors will not increment this count.
 ********************************************************************/
#define		EthStatRxOkCnt()	(ETHFRMRXOK)

/*********************************************************************
 * Function:        int EthStatRxFcsErrCnt(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          the number of received frames having FCS errors 
 *
 * Side Effects:    None
 *
 * Overview:        The function returns the current number of frames with FCS errors received by the Eth controller.
 *                  These are received frames that have an integral number of bytes.
 *
 * Note:            This statistics register is cleared by the read operation.
 *                  Frames with alignment error do not increment this count.
 ********************************************************************/
#define		EthStatRxFcsErrCnt()	(ETHFCSERR)

/*********************************************************************
 * Function:        int EthStatRxAlgnErrCnt(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          the number of received frames having alignment errors 
 *
 * Side Effects:    None
 *
 * Overview:        The function returns the current number of frames with alignment errors received by the Eth controller.
 *                  These are received frames that have FCS error and they do not contain an integral number of bytes (aka dribble-nibble).
 *
 * Note:            This statistics register is cleared by the read operation.
 ********************************************************************/
#define		EthStatRxAlgnErrCnt()	(ETHALGNERR)


/*********************************************************************
 * Function:        int EthStatTxOkCnt(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          the number of transmitted OK frames
 *
 * Side Effects:    None
 *
 * Overview:        The function returns the current number of frames transmitted sucessfully by the Eth controller.
 *
 * Note:            This statistics register is cleared by the read operation.
 ********************************************************************/
#define		EthStatTxOkCnt()	(ETHFRMTXOK)

/*********************************************************************
 * Function:        int EthStatTxSColCnt(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          the number of single collision frames
 *
 * Side Effects:    None
 *
 * Overview:        The function returns the current number of frames transmitted sucessfully by the Eth controller
 *                  on the second try.
 *
 * Note:            This statistics register is cleared by the read operation.
 ********************************************************************/
#define		EthStatTxSColCnt()	(ETHSCOLFRM)

/*********************************************************************
 * Function:        int EthStatTxMColCnt(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          the number of multiple collision frames
 *
 * Side Effects:    None
 *
 * Overview:        The function returns the current number of frames transmitted sucessfully  by the Eth controller
 *                  after there was more than one collision.
 *
 * Note:            This statistics register is cleared by the read operation.
 ********************************************************************/
#define		EthStatTxMColCnt()	(ETHMCOLFRM)


/**************************************
 *  MIIM access functions
 **************************************************/

/****************************************************************************
 * Function:        EthMIIMInit
 *
 * PreCondition:    None
 *
 * Input:           hostClock: the value of the host clock, Hz
 *                  miimClock: the maximum clock frequency that the PHY can use for the MIIM transactions 
 *                  oFlags:    opening flags to select the 10/100 Mbps RMII/MII speed
 *                  isRmii:    boolean to select the RMII/MII operation mode
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function initializes the MIIM interface for access of a PHY.
 *
 * Note:            - Any previous MIIM operation will be aborted!
 *                  - only ETH_OPEN_RMII and ETH_OPEN_100/ETH_OPEN_10 flags are relevant.
 *****************************************************************************/
void		EthMIIMInit(unsigned int hostClock, unsigned int miimClock, eEthOpenFlags oFlags, int isRmii);


/****************************************************************************
 * Function:        EthMIIMReadReg
 *
 * PreCondition:    rIx a valid PHY register, 0-31
 *                  phyAdd a valid PHY address, 0-31
 *
 * Input:           rIx: PHY register to be accessed
 *                  phyAdd: PHY address
 *
 * Output:          PHY register value 
 *
 * Side Effects:    None
 *
 * Overview:        This function performs a read operation from a PHY register using the MIIM interface.
 *
 * Note:            It assumes that no scan operation is in progress!
 *****************************************************************************/
unsigned short	EthMIIMReadReg(unsigned int rIx, unsigned int phyAdd);


/****************************************************************************
 * Function:        EthMIIMWriteReg
 *
 * PreCondition:    rIx a valid PHY register, 0-31
 *                  phyAdd a valid PHY address, 0-31
 *
 * Input:           rIx:    PHY register to be accessed
 *                  phyAdd: PHY address
 * 		    wData:  data to be written
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function performs a write operation to a PHY register using the MIIM interface.
 *                  It doesn't wait for the write to take effect.
 *
 * Note:            It assumes that no scan operation is in progress!
 *****************************************************************************/
void		EthMIIMWriteReg(unsigned int rIx, unsigned int phyAdd, unsigned short wData);

	
/****************************************************************************
 * Function:        EthMIIMWriteRegWait
 *
 * PreCondition:    rIx a valid PHY register, 0-31
 *                  phyAdd a valid PHY address, 0-31
 *
 * Input:           rIx: 	PHY register to be accessed
 *                  phyAdd: PHY address
 * 		    wData:	data to be written
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function performs a write operation to a PHY register using the MIIM interface.
 *                  It waits for the write to take effect.
 *
 * Note:            It assumes that no scan operation is in progress!
 *****************************************************************************/
void		EthMIIMWriteRegWait(unsigned int rIx, unsigned int phyAdd, unsigned short wData);


/****************************************************************************
 * Function:        EthMIIMScanStart
 *
 * PreCondition:    rIx a valid PHY register, 0-31
 *                  phyAdd a valid PHY address, 0-31
 *
 * Input:           rIx: 	PHY register to be accessed
 *                  phyAdd: PHY address
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function initiates a scan operation to a PHY register using the MIIM interface.
 *
 * Note:            It assumes that no scan operation is in progress!
 *****************************************************************************/
void		EthMIIMScanStart(unsigned int rIx, unsigned int phyAdd);

/****************************************************************************
 * Function:        EthMIIMScanRead
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function reads the result of a previously initiated scan operation from a PHY register using the MIIM interface.
 *
 * Note:            It assumes that a scan operation is in progress!
 *****************************************************************************/
unsigned short	EthMIIMScanRead(void);

/****************************************************************************
 * Function:        EthMIIMScanStop
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function stops a previously initiated scan operation from a PHY register using the MIIM interface.
 *                  It doesn't wait for the scan operation to finish.
 *
 * Note:            It assumes that a scan operation is in progress!
 *****************************************************************************/
void		EthMIIMScanStop(void);

/****************************************************************************
 * Function:        EthMIIMScanStopWait
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function stops a previously initiated scan operation from a PHY register using the MIIM interface.
 *                  It waits for the scan operation to finish.
 *
 * Note:            It assumes that a scan operation is in progress!
 *****************************************************************************/
void		EthMIIMScanStopWait(void);


/****************************************************************************
 * Function:        EthMIIMBusy
 *
 * PreCondition:    EthMIIMInit called
 *
 * Input:           None
 *
 * Output:          TRUE if the MIIM is busy with some transaction
 *                  FALSE otherwise
 *
 * Side Effects:    None
 *
 * Overview:        This function returns the current busy state of the MIIM interface.
 *
 * Note:            None
 *****************************************************************************/
extern __inline__ int __attribute__((always_inline)) EthMIIMBusy(void)
{
	return EMACxMINDbits.MIIMBUSY!=0;
}

/****************************************************************************
 * Function:        EthMIIMScanValid
 *
 * PreCondition:    EthMIIMInit called
 *
 * Input:           None
 *
 * Output:          TRUE if the result of a MIIM scan result is valid 
 *                  FALSE otherwise
 *
 * Side Effects:    None
 *
 * Overview:        This function returns the current validity state of the MIIM scan operation.
 *
 * Note:            None
 *****************************************************************************/
extern __inline__ int __attribute__((always_inline)) EthMIIMScanValid(void)
{
	return EMACxMINDbits.NOTVALID==0;
}



/**************************************
 *  Ethernet low level access functions
 **************************************************/

typedef enum
{
	ETH_CONFIG_MANFC	= _ETHCON1_MANFC_MASK,		// configure manual flow control
	ETH_CONFIG_AUTOFC	= _ETHCON1_AUTOFC_MASK,		// configure auto flow control
	ETH_CONFIG_RXEN		= _ETHCON1_RXEN_MASK,		// RX enabled
	ETH_CONFIG_TXRTS	= _ETHCON1_TXRTS_MASK,		// activate the TX logic and send packets
	ETH_CONFIG_SIDL		= _ETHCON1_SIDL_MASK,		// stop in idle
	ETH_CONFIG_ON		= _ETHCON1_ON_MASK,		// turn on/off the Eth controller
}eEthConfigFlags;	// ETH configuration flags



/*********************************************************************
 * Function:        void EthEnable(int enable)
 *
 * PreCondition:    None
 *
 * Input:           enable - boolean to enable/disable the ETHC
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The function gives access to low level Eth controller configuration.
 *                  It can turn on/off the controller.
 *
 * Note:            The Eth controller should be turned off before configured.
 *
 * Example:         EthEnable(0);
 ********************************************************************/
void		EthEnable(int enable);


/*********************************************************************
 * Function:        void EthConfig(eEthConfigFlags cFlags)
 *
 * PreCondition:    cFlags - valid configuration flags
 *
 * Input:           cFlags - configuration flags
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The function gives access to low level Eth controller configuration.
 *                  It configures the ETHC.
 *
 * Note:            The Eth controller should be configured when disabled.
 *
 * Example:         EthEnable(0); EthConfig(ETH_CONFIG_MANFC|ETH_CONFIG_RXEN|ETH_CONFIG_SIDL); EthEnable(1); 
 ********************************************************************/
void		EthConfig(eEthConfigFlags cFlags);
 

/*********************************************************************
 * Function:        void EthConfigSetFlags(eEthConfigFlags cFlags)
 *
 * PreCondition:    cFlags - valid configuration flags
 *
 * Input:           cFlags - configuration flags to be set
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The function gives access to low level Eth controller configuration.
 *                  Any flag set in the cFlags will be set, the other flags won't be touched. 
 *
 * Note:            The Eth controller should be configured when disabled.
 *
 * Example:         EthConfigSetFlags(ETH_CONFIG_MANFC|ETH_CONFIG_RXEN|ETH_CONFIG_SIDL);
 ********************************************************************/
void		EthConfigSetFlags(eEthConfigFlags cFlags);

/*********************************************************************
 * Function:        void EthConfigClrFlags(eEthConfigFlags cFlags)
 *
 * PreCondition:    cFlags - valid configuration flags
 *
 * Input:           cFlags - configuration flags to be cleared
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The function gives access to low level Eth controller configuration.
 *                  Any flag set in the cFlags will be cleared, the other flags won't be touched. 
 *
 * Note:            The Eth controller should be configured when disabled.
 *
 * Example:         EthConfigClrFlags(ETH_CONFIG_MANFC|ETH_CONFIG_RXEN|ETH_CONFIG_SIDL);
 ********************************************************************/
void		EthConfigClrFlags(eEthConfigFlags cFlags);

/*********************************************************************
 * Function:        void EthConfigTxStAddress(void* address)
 *
 * PreCondition:    address - valid physical address
 *
 * Input:           address - starting physical address of the first TX descriptor
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The function gives access to low level Eth controller configuration.
 *                  It sets the address of the TX descriptors in memory.
 *
 * Note:            - The address should be 4-byte aligned.
 *                  - The address should be a physical address.
 *                  - The address should not be written while any transmit or DMA operations are in progress.
 *
 * Example:         EthConfigTxStAddress(VirtToPhys(myTxDcpt));
 ********************************************************************/
void		EthConfigTxStAddress(void* address);

/*********************************************************************
 * Function:        void EthConfigRxStAddress(void* address)
 *
 * PreCondition:    address - valid physical address
 *
 * Input:           address - starting physical address of the first RX descriptor
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The function gives access to low level Eth controller configuration.
 *                  It sets the address of the RX descriptors in memory.
 *
 * Note:            - The address should be 4-byte aligned.
 *                  - The address should be a physical address.
 *                  - The address should not be written while any receive or DMA operations are in progress.
 *
 * Example:         EthConfigRxStAddress(VirtToPhys(myRxDcpt));
 ********************************************************************/
void		EthConfigRxStAddress(void* address);

/*********************************************************************
 * Function:        void EthAckRxDcpt(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The function gives access to low level Eth controller functionality.
 *                  It acknowledges the number of received descriptors.
 *                  When called, the number of un-acknowledged RX descriptors(BUFCNT=EthDescriptorsGetRxUnack())
 *                  will be decremented by one.
 *
 * Note:            - The function should be called after a RX packet is processed and the RX descriptor is available for the hardware.
 *                  - BUFCNT is also decremented by a call to EthRxAckPacket().
 *                  - BUFCNT value also directs the auto flow control behavior.
 *
 * Example:         EthAckRxDcpt();
 ********************************************************************/
extern __inline__ void __attribute__((always_inline))	EthAckRxDcpt(void)
{
	ETHCON1SET=_ETHCON1_BUFCDEC_MASK;
}

/**************************************
 *  Ethernet MAC low level access functions
 **************************************************/

typedef enum
{
	ETH_MAC_CONFIG1_RXEN		= _EMACxCFG1_RXENABLE_MASK,		// enable RX
	ETH_MAC_CONFIG1_PASS_ALL	= _EMACxCFG1_PASSALL_MASK,		// pass all RX frames (normal and control)
	ETH_MAC_CONFIG1_RX_PAUSE	= _EMACxCFG1_RXPAUSE_MASK,		// enable acting on RX-ed PAUSE control frames
	ETH_MAC_CONFIG1_TX_PAUSE	= _EMACxCFG1_TXPAUSE_MASK,		// enable TX of PAUSE control frames
	ETH_MAC_CONFIG1_LOOPBACK	= _EMACxCFG1_LOOPBACK_MASK,		// enable the loopback mode
	ETH_MAC_CONFIG1_RESET_TFUN	= _EMACxCFG1_RESETTFUN_MASK,		// put the TX logic in reset
	ETH_MAC_CONFIG1_RESET_TMCS	= _EMACxCFG1_RESETTMCS_MASK,		// put the MAC Control SUblayer/TX domain logic in reset
	ETH_MAC_CONFIG1_RESET_RFUN	= _EMACxCFG1_RESETRFUN_MASK,		// put the RX logic in reset
	ETH_MAC_CONFIG1_RESET_RMCS	= _EMACxCFG1_RESETRMCS_MASK,		// put the MAC Control SUblayer/RX domain logic in reset
	ETH_MAC_CONFIG1_RESET_SIM	= _EMACxCFG1_SIMRESET_MASK,		// reset the random generator within the TX function 
	ETH_MAC_CONFIG1_RESET_MAC	= _EMACxCFG1_SOFTRESET_MASK,		// put the MAC in reset	
}eEthMACConfig1Flags;	// ETH MAC configuration flags


typedef enum
{
	ETH_MAC_CONFIG2_FDUPLEX		= _EMACxCFG2_FULLDPLX_MASK,		// enable full duplex functionality
	ETH_MAC_CONFIG2_LEN_CHECK	= _EMACxCFG2_LENGTHCK_MASK,		// enable length frame check
	ETH_MAC_CONFIG2_HUGE_FRAME	= _EMACxCFG2_HUGEFRM_MASK,		// enable huge frames RX/TX
	ETH_MAC_CONFIG2_DELAY_CRC	= _EMACxCFG2_DELAYCRC_MASK,		// enable 4 bytes of proprietary header (ignored by the CRC)
	ETH_MAC_CONFIG2_CRC_ENABLE	= _EMACxCFG2_CRCENABLE_MASK,		// enable CRC append to each TX frame
	ETH_MAC_CONFIG2_PAD_ENABLE	= _EMACxCFG2_PADENABLE_MASK,		// PAD/CRC enable used together with ETH_MAC_CONFIG2_VLAN_PAD and ETH_MAC_CONFIG2_AUTO_PAD
	ETH_MAC_CONFIG2_VLAN_PAD	= _EMACxCFG2_VLANPAD_MASK,		// enable padding of al frames to 64 bytes and append valid CRC
	ETH_MAC_CONFIG2_AUTO_PAD	= _EMACxCFG2_AUTOPAD_MASK,		// enable automatic detection by the MAC of the type of frame and pad accordingly
	ETH_MAC_CONFIG2_PURE_PRE	= _EMACxCFG2_PUREPRE_MASK,		// enable preamble verification and discard packets with preamble errors
	ETH_MAC_CONFIG2_LONG_PRE	= _EMACxCFG2_LONGPRE_MASK,		// allow only packets having preamble fields less than 12 bytes in length
	ETH_MAC_CONFIG2_NO_BACKOFF	= _EMACxCFG2_NOBKOFF_MASK,		// do not use Binary Exponential Backoff algorithm following a collision, transmit immediately
	ETH_MAC_CONFIG2_BP_NO_BACKOFF	= _EMACxCFG2_BPNOBKOFF_MASK,		// after incidentally causing a collision during backpressure the MAC will immediately retransmit without backoff
	ETH_MAC_CONFIG2_EXCESS_DEFER	= _EMACxCFG2_EXCESSDFR_MASK,		// if enabled the MAC will defer to carrier indefinitely as per the standard.
	                                                                        // if disabled the MAC will abort ehrn the excessive deferral limit is reached and provide feedback to the host system
}eEthMACConfig2Flags;	// ETH MAC configuration flags

/*********************************************************************
 * Function:        void EthMACConfig(eEthMACConfig1Flags c1Flags, eEthMACConfig2Flags c2Flags)
 *
 * PreCondition:    c1Flags, c2Flags - valid configuration flags
 *
 * Input:           c1Flags, c2Flags - configuration flags
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The function gives access to low level Eth MAC configuration.
 *                  It configures the MAC.
 *
 * Note:            None
 *
 * Example:         EthMACConfig(ETH_MAC_CONFIG1_RXEN|ETH_MAC_CONFIG1_RX_PAUSE|ETH_MAC_CONFIG1_TX_PAUSE, ETH_MAC_CONFIG2_FDUPLEX|ETH_MAC_CONFIG2_HUGE_FRAME); 
 ********************************************************************/
void		EthMACConfig(eEthMACConfig1Flags c1Flags, eEthMACConfig2Flags c2Flags);
 

/*********************************************************************
 * Function:        void EthMACConfigSetFlags(eEthMACConfig1Flags c1Flags, eEthMACConfig2Flags c2Flags)
 *
 * PreCondition:    c1Flags, c2Flags - valid configuration flags
 *
 * Input:           c1Flags, c2Flags - configuration flags to be set
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The function gives access to low level Eth MAC configuration.
 *                  Any flag set in the c1Flags, c2Flags will be set, the other flags won't be touched. 
 *
 * Note:            None
 *
 * Example:         EthMACConfigSetFlags(ETH_MAC_CONFIG1_RXEN|ETH_MAC_CONFIG1_RX_PAUSE|ETH_MAC_CONFIG1_TX_PAUSE, ETH_MAC_CONFIG2_FDUPLEX|ETH_MAC_CONFIG2_HUGE_FRAME);
 ********************************************************************/
void		EthMACConfigSetFlags(eEthMACConfig1Flags c1Flags, eEthMACConfig2Flags c2Flags);

/*********************************************************************
 * Function:        void EthMACConfigClrFlags(eEthMACConfig1Flags c1Flags, eEthMACConfig2Flags c2Flags)
 *
 * PreCondition:    c1Flags, c2Flags - valid configuration flags
 *
 * Input:           c1Flags, c2Flags - configuration flags to be cleared
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The function gives access to low level Eth MAC configuration.
 *                  Any flag set in the c1Flags, c2Flags will be cleared, the other flags won't be touched. 
 *
 * Note:            None
 *
 * Example:         EthMACConfigClrFlags(ETH_MAC_CONFIG1_RXEN|ETH_MAC_CONFIG1_RX_PAUSE|ETH_MAC_CONFIG1_TX_PAUSE, ETH_MAC_CONFIG2_FDUPLEX|ETH_MAC_CONFIG2_HUGE_FRAME);
 ********************************************************************/
void		EthMACConfigClrFlags(eEthMACConfig1Flags c1Flags, eEthMACConfig2Flags c2Flags);


/*********************************************************************
 * Function:        void EthMACConfigB2BIpGap(unsigned int ipGap)
 *
 * PreCondition:    0 < ipGap < 128
 *
 * Input:           ipGap - the back to back inter packet gap
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The function sets the back to back inter packet gap value (the nibble time offset
 *                  of the minimum possible period between the end of any transmitted packet to the beginning of the next).
 *                  
 * Note:            - In full duplex the recommended value is 0x15
 *                  - in half duplex the recommended value is 0x12
 *
 * Example:         EthMACConfigB2BIpGap(0x15);
 ********************************************************************/
void		EthMACConfigB2BIpGap(unsigned int ipGap);

/*********************************************************************
 * Function:        void EthMACConfigNB2BIpGap(unsigned int ipgr1, unsigned int ipgr2)
 *
 * PreCondition:    0 < carrDefer < 128
 *                  0 < nB2BIpGap < 128
 *
 * Input:           carrDefer - the carrier deference
 *                  nB2BIpGap - non back to back inter packet gap
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The function sets the non back to back inter packet gap value part 1 and 2.
 * 
 *                  The ipgr1 represents the optional carrierSense window referenced in IEEE 802.3/4.2.3.2.1 ‘Carrier Deference’.
 *                  If carrier is detected during the timing of IPGR1, the MAC defers to carrier. If, however, carrier becomes after IPGR1,
 *                  the MAC continues timing IPGR2 and transmits, knowingly causing a collision, thus ensuring fair access to medium.
 *                  The ipgr2 represents the non back to back inter packet gap.
 *                  
 * Note:            - the ipgr1 recommended value is 0xc. Its range of values is 0x0 to ipgr2
 *                  - the ipgr2 recommended value is 0x12
 *
 * Example:         EthMACConfigNB2BIpGap(0xc, 0x12);
 ********************************************************************/
void		EthMACConfigNB2BIpGap(unsigned int ipgr1, unsigned int ipgr2);

/*********************************************************************
 * Function:        void EthMACConfigCollWinRetry(unsigned int collWin, unsigned int retMax)
 *
 * PreCondition:    0 < collWin < 64
 *                  0 < retMax < 16
 *
 * Input:           collWin - the collision window
 *                  retMax   - retransmission maximum
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The function sets the collision window and the retransmission max.
 *
 *                  The collision window is a programmable field representing the slot time or collision window
 *                  during which collisions occur in properly configured networks.
 *                  Since the collision window starts at the beginning of transmission, the preamble and SFD is included.
 *                                
 *                  The retransmission maximum is a programmable field specifying the number of retransmission attempts
 *                  following a collision before aborting the packet due to excessive collisions.
 *                  
 * Note:            - the collWin recommended value is 0x37 which corresponds to the count of frame bytes
 *                    at the end of the window. 
 *                  - the retMax recommended value is 0xf
 *
 * Example:         EthMACConfigCollWinRetry(0x37, 0xf);
 ********************************************************************/
void		EthMACConfigCollWinRetry(unsigned int collWin, unsigned int retMax);

#endif	// _ETH


#endif	// _ETH_H_

