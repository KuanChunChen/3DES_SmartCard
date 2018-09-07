 

#ifndef DEFINE_H
#define DEFINE_H
#include "OpenSSL/des.h"
#ifdef __cplusplus
extern "C" {
#endif


// data type
//typedef unsigned char BYTE;
//typedef long LONG;
//page
#define d_PAGE_HOME    0x0900  
//ICV
#define d_ICV "\x46\x49\x53\x43\x2D\x53\x56\x43"

#define d_SCARD_S_SUCCESS 0x0000
//error code 
#define d_SCARD_ESTABLISH_FAIL 0x1000
#define d_SCARD_TRANSMIT_FAIL  0x1001	
#define d_SCARD_CONNECT_FAIL   0x1002
#define d_SCARD_OFF_FAIL       0x1003
#define d_SCARD_RESET_EMV_FAIL  0x1004
#define d_SCARD_RESET_ISO_FAIL 0x1005
#define d_RN_INFO_ERROR        0x2000 
#define d_LOADING_COUNT_EXCEED 0x3000	
#define d_LOADING_VAULE_EXCEED 0x3001
#define d_LOADING_VAULE_OVERFLOW 0x3002
#define d_LOADING_VAULE_INVALID 0x3003
#define d_DEDUCT_VAULE_OVERFLOW 0x3004    
#define d_COMMAND_TRANSMIT_FAIL 0x4000
#define d_READER_NOT_IN_GROUP	0x4001
#define d_SMARTCARDLIST_FAIL	0x4002
#define d_PTAC_ERROR            0x6000
#define d_SENT_APDU_FAIL        0x6001
#define d_INPUT_CANCEL          0x6002
//ba buffer size
#define d_MAX_BUFFER_SIZE 128
#define d_KEY_LENGTH 8
#define d_LOADING_COMMAND_LENGTH  22+1
#define d_DEDUCT_COMMAND_LENGTH   22+1
#define d_LOADING_REC_LENGTH   10+1    
#define d_DEDUCT_REC_LENGTH   34+1
#define d_GETRN_REC_LENGTH 59+1    
#define d_MAX_LV_DV_VALUE  99999999    

    

    //bo page in/out
#define d_PAGE_IN			1
#define d_PAGE_OUT			0
//message
#define d_LOADING_INPUT 0x5000
#define d_DEDUCT_INPUT  0x5001
#define d_MAIN_MESG	0x5002
//PID
#define d_PID "\x31\x32\x33\x34\x35\x36\x37\x38"
//command
#define d_RN_COMMAND "\x00\x52\xA2\x01\x39"
#define d_LOADING_COMMAND "\x00\x54\xA2\x01\x10"
#define d_LOADING_LE "\x08"
#define d_DEDUCT_COMMAND  "\x00\x56\xA2\x01\x10"
#define d_DEDUCT_LE  "\x00"

//seed Loading key 
#define d_LOADING_KEY_LK "\x00\x01\x02\x03\x04\x05\x06\x07"
#define d_LOADING_KEY_DK "\x10\x11\x12\x13\x14\x15\x16\x17 "
#define d_LOADING_KEY_PK "\x20\x21\x22\x23\x24\x25\x26\x27"
//seed Deduct key 
#define d_DEDUCT_KEY_LK "\x20\x21\x22\x23\x24\x25\x26\x27"
#define d_DEDUCT_KEY_DK "\x30\x31\x32\x33\x34\x35\x36\x37 "
#define d_DEDUCT_KEY_PK "\x40\x41\x42\x43\x44\x45\x46\x47"
//seed Purchase key 
#define d_Purchase_KEY_LK "\x10\x11\x12\x13\x14\x15\x16\x17"
#define d_Purchase_KEY_DK "\x20\x21\x22\x23\x24\x25\x26\x27 "
#define d_Purchase_KEY_PK "\x30\x31\x32\x33\x34\x35\x36\x37"
//struct

typedef struct  __RN_Info
{
	// n byte context and 1 byte end Ending char
	BYTE  baRN_PID[8 + 1];
	BYTE  baRN_LSN_OR_TSN[8 + 1];
	BYTE  baRN_LA[8 + 1];
	BYTE  baRN_DSN_OR_TSN[8 + 1];
	BYTE  baRN_DA[8 + 1];
	BYTE  baRN_LRn[8 + 1];
	BYTE  baRN_ToF[1 + 1];
	BYTE  baRN_BUL[8 + 1];
	BYTE  baRN_SW12[2 + 1];

	BYTE  baRN_LV[8 + 1 ];
	BYTE  baRN_DV[8 + 1];
      
        LONG  lRN_LA;
        LONG  lRN_DA;
        LONG  lRN_AB;
        
        BYTE  baRN_AB[8 + 1];

}RN_Info;








//存key的結構
typedef struct  __KeyStructure
{
	DES_cblock stLK, stDK, stPK;
	DES_key_schedule stSchedule_LK, stSchedule_DK, stSchedule_PK;
}	KeyStructure;



#ifdef __cplusplus
}
#endif

#endif /* DEFINE_H */

