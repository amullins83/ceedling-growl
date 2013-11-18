// *****************************************************************************
// *****************************************************************************
// NDA with FTDI - DO NOT DISCLOSE UNTIL 07/01/2015
// *****************************************************************************
// *****************************************************************************

// PROPRIETARY INFORMATION FTDI STRUCTURES

// *****************************************************************************
// *****************************************************************************
// FTDI Constants - CONFIDENTIAL!!!
// *****************************************************************************
// *****************************************************************************

// Functions.
#define FTDI_REQUEST_RESET              0x00    // bmRequestType - Reset the communication port.
#define FTDI_REQUEST_MODEMCTRL          0x01    // bmRequestType - Set the modem control register.
#define FTDI_REQUEST_SETFLOWCTRL        0x02    // bmRequestType - Set flow control options.
#define FTDI_REQUEST_SETBAUDRATE        0x03    // bmRequestType - Set the baud rate.
#define FTDI_REQUEST_SETDATA            0x04    // bmRequestType - Set the data characteristics of the port.
#define FTDI_REQUEST_GETMODEMSTAT       0x05    // bmRequestType - Retrieve the current value of the modem status register.
#define FTDI_REQUEST_SETEVENTCHAR       0x06    // bmRequestType - Set the event character.
#define FTDI_REQUEST_SETERRORCHAR       0x07    // bmRequestType - Set the error character.
#define FTDI_REQUEST_SETLATTIMER        0x09    // bmRequestType - Set the latency timer.
#define FTDI_REQUEST_GETLATTIMER        0x0A    // bmRequestType - Return the latency timer.
#define FTDI_REQUEST_SETBITMODE         0x0B    // bmRequestType - Set a special bit mode or turn on a special function.
#define FTDI_REQUEST_GETBITMODE         0x0C    // bmRequestType - Return the current values on the data bus pins.
#define FTDI_REQUEST_EEPROM_READE2      0x90    // bmRequestType - Read the external EEPROM (if fitted).
#define FTDI_REQUEST_EEPROM_WRITEE2     0x91    // bmRequestType - Write to the external EEPROM (if fitted).
#define FTDI_REQUEST_EEPROM_ERASEE2     0x92    // bmRequestType - Erase the external EEPROM (if fitted).

#define FTDI_SET_REQUEST                0x40    // Set Request Type.
#define FTDI_GET_REQUEST                0xC0    // Get Request Type.

#define FTDI_PORTA                      0x01    // Specify port A.
#define FTDI_PORTB                      0x02    // Specify port B.
#define FTDI_PORTC                      0x03    // Specify port C.
#define FTDI_PORTD                      0x04    // Specify port D.

// RESET
#define FTDI_RESET_PURGE_RXTX           0x00    // Purge RX and TX.
#define FTDI_RESET_PURGE_RX             0x01    // Purge RX.
#define FTDI_RESET_PURGE_TX             0x02    // Purge TX.

// ModemCtrl
#define FTDI_DTR_ACTIVE                 0x01    // DTR Active.
#define FTDI_RTS_ACTIVE                 0x02    // RTS Active.
#define FTDI_DTR_WRITING                0x01    // Enable DTR for writing.
#define FTDI_RTS_WRITING                0x02    // Enable RTS for writing.

// SetFlowCtrl
#define FTDI_RTSCTS_FLOWCTRL            0x01    // Enable RTS/CTS flow control.
#define FTDI_DTRDSR_FLOWCTRL            0x02    // Enable DTR/DSR flow control.
#define FTDI_XONXOFF_FLOWCTRL           0x04    // Enable XON/OFF flow control.

// SetBaudRate
// To calculate baud rate, take 48Mhz and divide by 16.  Then divide by baud
// rate needed.
#define FTDI_BAUD_115200                0x001A  // Actual baud rate is 115385.
#define FTID_BAUD_HIGH                  0x00    // Keep max frequency at 48Mhz.

// SetData
#define FTDI_SEVEN_BITS                 0x07    // 7 data bits.
#define FTDI_EIGHT_BITS                 0x08    // 8 data bits.

#define FTDI_PARITY_NONE                0x00    // No parity.
#define FTDI_PARITY_ODD                 0x01    // Odd parity.
#define FTDI_PARITY_EVEN                0x02    // Even parity.
#define FTDI_PARITY_MARK                0x03    // Mark parity.
#define FTDI_PARITY_SPACE               0x04    // Space parity.

#define FTDI_STOP_ONE                   0x00    // One stop bit.
#define FTDI_STOP_TWO                   0x10    // Two stop bits.

#define FTDI_BREAK_NO                   0x00    // No break.
#define FTDI_BREAK_SET                  0x40    // Set break.

// SetEventChar
#define FTDI_TRIGGER_NO                 0x00    // No trigger.
#define FTDI_TRIGGER_EVENT              0x01    // Trigger IN on event char.

// SetErrorChar
#define FTDI_ERROR_OFF                  0x00    // No Error replacement.
#define FTDI_ERROR_ON                   0x01    // Error replacement ON.

// SetBitMode
#define FTDI_MODE_OFF                   0x00    // Bit mode off.
#define FTDI_ASYNC_BITBANG              0x01    // Asynchronus Bit Bang.
#define FTDI_MPSSE_ENABLE               0x02    // MPSSE enable.
#define FTDI_SYNC_BITBANG               0x04    // Synchronous Bit Bang.
#define FTID_MPSSE_CPU_EMUL             0x08    // MPSSE CPU emulation.
#define FTDI_RESET_FAST_SERIAL          0x10    // Reset FAST SERIAL mode.
#define FTDI_ENABLE_FIFO_A              0x40    // Enable Single Channel Synchronous FIFO on A (only A).
