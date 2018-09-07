#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
 #include <ctosapi.h>
#include "define.h"

USHORT page_home(LONG lPageID)
{
     switch(lPageID)
        {
            case d_PAGE_HOME:
                
                CTOS_LCDTClearDisplay();
                CTOS_LCDTSetReverse(TRUE);
                CTOS_LCDTPrintXY(1, 1, " V5s_CTMS_TEST    "); 
                CTOS_LCDTSetReverse(FALSE); 
         
                CTOS_LCDTPrintXY(1, 2, "1.Get RN Info"); 
     
                CTOS_LCDTPrintXY(1, 3, "2.Loading"); 
    
                CTOS_LCDTPrintXY(1, 4, "3.Deduct"); 
                CTOS_LCDTPrintXY(1, 8, "         ver.001"); 
                break;
             
         }
     
     return d_OK;
    
}


USHORT page_RN_info (RN_Info stRN)
{     
    int w;
    BYTE babuff[9];
    BYTE bKey;
    CTOS_LCDTClearDisplay();
    
    
    CTOS_LCDTClearDisplay();
    CTOS_LCDTSetReverse(TRUE);
    CTOS_LCDTPrintXY(1, 1, " SMART CARD INFO        "); 
    CTOS_LCDTSetReverse(FALSE); 
    
    CTOS_LCDTPrintXY(1, 2, "Loading Times:       " );
 
    memset(babuff, 0x00, sizeof(babuff));
    memcpy(babuff, stRN.baRN_LSN_OR_TSN, 8);
    CTOS_LCDTPrintXY(1, 3, babuff);
    
    CTOS_LCDTPrintXY(1, 4, "Loading Amount:      " );
    memset(babuff, 0x00, sizeof(babuff));
    memcpy(babuff, stRN.baRN_LA, 8);
    CTOS_LCDTPrintXY(1, 5, babuff);
    
    CTOS_KBDGet(&bKey);
    
    //CTOS_LCDTClearDisplay();
    CTOS_LCDTPrintXY(1, 2, "Deduct Times:         " );
    memset(babuff, 0x00, sizeof(babuff));
    memcpy(babuff, stRN.baRN_DSN_OR_TSN, 8);
    CTOS_LCDTPrintXY(1, 3, babuff);
    
    CTOS_LCDTPrintXY(1, 4, "Deduct Amount:       " );
    memset(babuff, 0x00, sizeof(babuff));
    memcpy(babuff, stRN.baRN_DA, 8);
    CTOS_LCDTPrintXY(1, 5, babuff);
     //CTOS_LCDTPrintXY(1, 3, baRecvBuffer);
	
    CTOS_LCDTPrintXY(1, 6, "Acount Balance:       " );
    memset(babuff, 0x00, sizeof(babuff));
    memcpy(babuff, stRN.baRN_AB, 8);
    CTOS_LCDTPrintXY(1, 7, babuff);
	 

                
    return d_OK;
}


 


USHORT showMesg(USHORT usMeg)
{
    CTOS_LCDTClearDisplay();
    switch (usMeg)
    {
	case d_LOADING_INPUT:
            CTOS_LCDTPrintXY(1,2, "Enter load amount: " );
            break;
	case d_DEDUCT_INPUT:
            CTOS_LCDTPrintXY(1,2, "Enter deduct amount: " );
            break;
        case d_SCARD_ESTABLISH_FAIL:
            CTOS_LCDTPrintXY(1,8,"ESTABLISH_FAIL!");
            break;
	case d_SCARD_TRANSMIT_FAIL:
            CTOS_LCDTPrintXY(1,8,"TRANSMIT_FAIL!");
            break; 
	case d_RN_INFO_ERROR:
            CTOS_LCDTPrintXY(1,8,"RN_INFO_ERROR!");
            break;
	case d_LOADING_COUNT_EXCEED:
            CTOS_LCDTPrintXY(1,8,"LOADING_COUNT_EXCEED!");
            break;
	
	case d_LOADING_VAULE_INVALID:
            CTOS_LCDTPrintXY(1,8,"LV /DV INVALID!");
            break;
        case d_SCARD_OFF_FAIL:
            CTOS_LCDTPrintXY(1,8,"SC OFF FAIL");
            break;
        case d_SCARD_RESET_EMV_FAIL:
            CTOS_LCDTPrintXY(1,8," ResetEMV Fail");
            break;            
        case d_SCARD_RESET_ISO_FAIL:
            CTOS_LCDTPrintXY(1,8,"ResetISO Fail");
            break;
        case d_SENT_APDU_FAIL:
            CTOS_LCDTPrintXY(1,8,"Sent APDU Fail");
            break;      
        case d_LOADING_VAULE_OVERFLOW:
            CTOS_LCDTPrintXY(1,8,"Loading Overflow!");
            break;               
            
         case d_DEDUCT_VAULE_OVERFLOW:
            CTOS_LCDTPrintXY(1,8,"Deduct Overflow!");
            break; 
    }
    return d_OK;
}

void showInput(OUT BYTE *baInput ,OUT int *iInputLen)
{
    int w = d_PAGE_IN; 
    BYTE bKey=0;
    BYTE baInputBuff[8 + 1];
    int iInputIndex=0;
    CTOS_LCDTClearDisplay();
    CTOS_LCDTSetReverse(TRUE);
    CTOS_LCDTPrintXY(1, 1, " Load/Deduct Mode        "); 
    CTOS_LCDTSetReverse(FALSE); 
    memset(baInputBuff, 0, sizeof(baInputBuff));
    CTOS_LCDTPrintXY(1, 2, "Enter LA /DV:");
    while(w)
    {   //memset(&bKey, 0, 1);   
        CTOS_KBDGet(&bKey);
        switch(bKey)
        {
         
            case d_KBD_ENTER:
                //memcpy(baInput, baInputBuff, 8);
                //memset(baInput, 0, sizeof(baInput));
                memcpy(baInput,baInputBuff,strlen(baInputBuff));
                
                *iInputLen=iInputIndex;
               
                //CTOS_LCDTPrintXY(1, 7, baInput);
            
               
                w=d_PAGE_OUT;
                break;
            case d_KBD_CANCEL:
                return d_INPUT_CANCEL;
            case d_KBD_CLEAR:
                baInputBuff[strlen(baInputBuff)-1]='\0';
                CTOS_LCDTPrintXY(1, 3, "                ");
                CTOS_LCDTPrintXY(1, 3, baInputBuff);
                iInputIndex--;
                break;
            case d_KBD_F1 :
            case d_KBD_F2:
            case d_KBD_F3:
            case d_KBD_F4:
            case d_KBD_UP:
            case d_KBD_DOWN:
            case d_KBD_DOT:
            case d_KBD_00 :
                //鎖定按鈕
                break;   
            default:
                //按下數字鍵
                //strcat (baInputBuff,&bKey);
                
                if(iInputIndex<=7)
                {
                    baInputBuff[iInputIndex]=bKey;
                    iInputIndex++;
                }
                CTOS_LCDTPrintXY(1, 3, baInputBuff);
                
                break;
        }
    }
    
}

USHORT page_Loading(BYTE *baRecvBuffer, USHORT usRecvLength)
{
    CTOS_LCDTSetReverse(TRUE);
    CTOS_LCDTPrintXY(1, 1, " SMART CARD INFO        "); 
    CTOS_LCDTSetReverse(FALSE); 
    if (baRecvBuffer[0] == '\x66')
    {
    	CTOS_LCDTPrintXY(1, 8,"Loading Failed!");
    }
    else 
    {
        CTOS_LCDTPrintXY(1, 8,"Loading Success!");
        CTOS_LCDTPrintXY(1, 2,"Total Loading Value:");
        CTOS_LCDTPrintXY(1, 3,baRecvBuffer);
    }

	
    return d_OK;
}


USHORT page_Deduct(BYTE *baDeduct, BYTE *baPTAC)
{
	
    int i;
    BYTE babuff[8+1];
    CTOS_LCDTSetReverse(TRUE);
    CTOS_LCDTPrintXY(1, 1, " Deduct Sequence      "); 
    CTOS_LCDTSetReverse(FALSE); 
    for ( i = 0; i < 8; i++)
    {
        if (baDeduct[i + 24] != baPTAC[i])
        {
            CTOS_LCDTPrintXY(1, 8,"PTAC ERROR!");
            return d_PTAC_ERROR;
        }
    }
    
    
    CTOS_LCDTPrintXY(1, 2,"TotalDeductCount:");
	 
    memset(babuff, 0x00, sizeof(babuff));
    memcpy(babuff, baDeduct+8, 8);
    CTOS_LCDTPrintXY(1, 3, babuff);

	

    CTOS_LCDTPrintXY(1, 4,"TotalDeductAmount:");
    memset(babuff, 0x00, sizeof(babuff));
    memcpy(babuff, baDeduct+16, 8);
    CTOS_LCDTPrintXY(1, 5, babuff);
    

    return d_OK;
	 
}
