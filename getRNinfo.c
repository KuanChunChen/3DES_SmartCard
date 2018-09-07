 
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctosapi.h>
#include "define.h"


USHORT getInfoWithRN(BYTE *baRecvBuffer, USHORT *usRecvLength)
{
    BYTE bKey;
    BYTE bSC_Status;
    USHORT usReturn;
    //OUT / IN//out
    BYTE baATR[ d_MAX_BUFFER_SIZE], bATRLen, CardType;
    //sent //receive
    BYTE baSAPDU[d_MAX_BUFFER_SIZE] = d_RN_COMMAND;
    BYTE baRAPDU[d_MAX_BUFFER_SIZE];
    BYTE babuff[d_MAX_BUFFER_SIZE];
    USHORT bSLen,bRLen;

    do
    {
        CTOS_KBDHit(&bKey);
        if (bKey == d_KBD_CANCEL) 
            return;
        CTOS_SCStatus(d_SC_USER,&bSC_Status);
    }while(!(bSC_Status & d_MK_SC_PRESENT)); 
    //直到使用者插入smart card 才結束
    
    bATRLen = sizeof(baATR);
 
    //Power on the ICC and retrun the ATR contents metting the EMV2000 specification //
    usReturn = CTOS_SCResetEMV(d_SC_USER, d_SC_5V, baATR, &bATRLen, &CardType);
    if (usReturn != d_OK)
    {
        
        return d_SCARD_RESET_EMV_FAIL;
    }
    
    CTOS_Delay(1000);
    bATRLen = sizeof(baATR);    
    CTOS_LCDTPrintXY(1,4,"Reset ISO..");
    //Power on the ICC and retrun the ATR content metting the ISO-7816 specification //
    //取得卡片狀態
    usReturn = CTOS_SCResetISO(d_SC_USER, d_SC_5V, baATR, &bATRLen, &CardType);
    if (usReturn != d_OK)
    {   
       
        return d_SCARD_RESET_ISO_FAIL;
    }
     
        
    
    CTOS_LCDTPrintXY(1,5,"Send APDU..");
    //APDU Data

    bSLen = 5;
    bRLen = sizeof(baRAPDU);
    usReturn = CTOS_SCSendAPDU(d_SC_USER, baSAPDU, bSLen, baRAPDU, &bRLen);
    //Send out an APDU command and get the response from ICC //
    if (usReturn != d_OK)
    {
        return d_SENT_APDU_FAIL;
    }
 


    CTOS_KBDGet(&bKey);
    *usRecvLength = bRLen ;
    memcpy(baRecvBuffer, baRAPDU, bRLen);
    return d_OK;
}



USHORT RNinfoProcessing(OUT BYTE *baProcessRN,IN RN_Info *stRN)
{ 

    //將收到的整串回傳值存進不同變數
    RN_Info stProcessRN_info;
    int i;
    LONG lDigitBuff;
    LONG lDigitCount;
    BYTE  bReverseBuff;
    if (baProcessRN[57] != '\x90' && baProcessRN[58] != '\x00')
    {
        return d_RN_INFO_ERROR;
    }
	//PID 0~7
	//LSN_TSN 7~15
	//LA 16~23
	//DSN_TSN 24~31
	//DA 32~39
	//LRn 40~47
	//BUL 49~56
	
    memset(&stProcessRN_info, 0x00, sizeof(RN_Info));
    for (i = 0; i < 8; i++)
    {
        stProcessRN_info.baRN_PID[i] = baProcessRN[i];
        stProcessRN_info.baRN_LSN_OR_TSN[i] = baProcessRN[i+8];
        stProcessRN_info.baRN_LA[i] = baProcessRN[i + 16];
        stProcessRN_info.baRN_DSN_OR_TSN[i] = baProcessRN[i + 24];
        stProcessRN_info.baRN_DA[i] = baProcessRN[i+ 32];
        stProcessRN_info.baRN_LRn[i] = baProcessRN[i + 40];
        stProcessRN_info.baRN_BUL[i] = baProcessRN[i + 49];
    }
    //ToF 48
    stProcessRN_info.baRN_ToF[0] = baProcessRN[48];
    //SW12 57~58
    stProcessRN_info.baRN_SW12[0] = baProcessRN[57];
    stProcessRN_info.baRN_SW12[1] = baProcessRN[58];
    
    //stProcessRN_info.baRN_DA- stProcessRN_info.baRN_LA;
    stProcessRN_info.lRN_LA = 0;
    stProcessRN_info.lRN_DA = 0;
    stProcessRN_info.lRN_AB = 0;
    for(i=0;i<8;i++)
    {
        if(stProcessRN_info.lRN_LA == 0)
        {
            stProcessRN_info.lRN_LA = stProcessRN_info.baRN_LA[i]-48;
        }else
        {
            stProcessRN_info.lRN_LA = stProcessRN_info.lRN_LA*10 ;
            stProcessRN_info.lRN_LA += (stProcessRN_info.baRN_LA[i]-48);
        }
        
         if(stProcessRN_info.lRN_DA == 0)
        {
            stProcessRN_info.lRN_DA = (stProcessRN_info.baRN_DA[i]-48);
        }else
        {
            stProcessRN_info.lRN_DA = stProcessRN_info.lRN_DA*10 ;
            stProcessRN_info.lRN_DA += (stProcessRN_info.baRN_DA[i]-48);
    
        }
    }  
    stProcessRN_info.lRN_AB = stProcessRN_info.lRN_LA - stProcessRN_info.lRN_DA;
    
    while (stProcessRN_info.lRN_AB > 0)
    {
        //當迴圈第8次時
        //lAmount還大於0代表輸入的值超過9位數則回傳error
        if (lDigitCount == 8 && stProcessRN_info.lRN_AB > 0)
        {
            return d_LOADING_VAULE_OVERFLOW;
        }
          //算每個位數的數字
        lDigitBuff = stProcessRN_info.lRN_AB % 10;
    	//放入ba
        stProcessRN_info.baRN_AB[lDigitCount] = lDigitBuff ;
        //實現while
        stProcessRN_info.lRN_AB = stProcessRN_info.lRN_AB / 10;
        //每算到一位則加一
        lDigitCount++;


    }
    //反轉陣列
    for (i = 0; i < lDigitCount / 2; i++)
    {
        bReverseBuff = stProcessRN_info.baRN_AB[i];
        stProcessRN_info.baRN_AB[i] = stProcessRN_info.baRN_AB[lDigitCount - i - 1];
        stProcessRN_info.baRN_AB[lDigitCount - i - 1] = bReverseBuff;
    }
    for (i = 0; i < lDigitCount; i++)
        stProcessRN_info.baRN_AB[i] += 48;
     
    //OUT stRN
    *stRN = stProcessRN_info;
    return d_OK;

}

USHORT ICCPowerOff(void)
{
    if (CTOS_SCPowerOff(d_SC_USER) == d_OK)
        return d_OK;
    else
        return d_SCARD_OFF_FAIL;
}
