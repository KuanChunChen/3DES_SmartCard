
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctosapi.h>
#include "define.h"
#include <openssl/des.h>
#include "deduct.h"
/** 
** The main entry of the terminal application 
**/
int main(int argc,char *argv[])
{
	
    BYTE bKey;
    USHORT usReturn;
    //
    int i=d_PAGE_IN;
    //用在getRn info
    BYTE baRecvBuffer[d_GETRN_REC_LENGTH];
    USHORT usRecvLength;
    //從卡片取得的回傳值轉成 結構用
    RN_Info stRN_Info;

    //算出各種TAC存在這
    BYTE baLTAC[8+1];
    BYTE baPTAC[8+1];
    //存送完command的回傳
    BYTE baLVRec[d_LOADING_REC_LENGTH];
    BYTE baDVRec[d_DEDUCT_REC_LENGTH];
    // 存輸入金額
    BYTE baInput[8+1];
    int iInputLen;
    USHORT usLDLength;
   
    while(i)
    {  
        //首頁
        page_home(d_PAGE_HOME);
        CTOS_KBDGet(&bKey);
        
        switch(bKey)
        {
            case d_KBD_1:
                CTOS_LCDTClearDisplay();
                memset(baRecvBuffer, 0x00, sizeof(baRecvBuffer));
                memset(&stRN_Info, 0x00, sizeof(RN_Info));
                usReturn = getInfoWithRN(baRecvBuffer,&usRecvLength);
                if (usReturn != d_OK)
                {
                    showMesg(usReturn);
                    break;
                }
                
                usReturn = RNinfoProcessing(baRecvBuffer, &stRN_Info);
                if (usReturn != d_OK)
                {
                    showMesg(usReturn);
                    break;
                }
                page_RN_info(stRN_Info);
                
                usReturn = ICCPowerOff();
                if(usReturn !=d_OK)
                {
                    showMesg(usReturn);
                }
                CTOS_KBDGet(&bKey);
                break;
            case d_KBD_2:
               
                CTOS_LCDTClearDisplay();
                memset(baRecvBuffer, 0x00, sizeof(baRecvBuffer));
                memset(&stRN_Info, 0x00, sizeof(RN_Info));
                usReturn = getInfoWithRN(baRecvBuffer,&usRecvLength);
                if (usReturn != d_OK)
                {
                    showMesg(d_SCARD_ESTABLISH_FAIL);
                    usReturn = ICCPowerOff();
                    
                    if(usReturn !=d_OK)
                    {
                        showMesg(usReturn);
                    }
                    break;
                }else
                {
                
                    usReturn = RNinfoProcessing(baRecvBuffer, &stRN_Info);
                    
                        if (usReturn != d_OK)
                        {
                            showMesg(usReturn);
                            usReturn = ICCPowerOff();
                            
                            if(usReturn !=d_OK)
                            {
                                showMesg(usReturn);
                            }
                            
                            CTOS_KBDGet(&bKey);
                            break;
                        }
                    
           
                    memset(baInput, 0x00, sizeof(baInput));
                    usReturn = showInput(baInput ,&iInputLen);
                    if(usReturn ==d_INPUT_CANCEL)
                    {
                        break;
                    }
                    usReturn = CheckAndLoadingData(stRN_Info.lRN_LA,baInput,iInputLen,stRN_Info.baRN_LV,stRN_Info.baRN_LSN_OR_TSN);
                
                }
                if (usReturn == d_OK)
                {
                    loadingLTAC(stRN_Info.baRN_LSN_OR_TSN, stRN_Info.baRN_LRn ,stRN_Info.baRN_LV,baLTAC);
                    usReturn = LoadingCommand(baLTAC,stRN_Info.baRN_LV , baLVRec, usLDLength);
                    if (usReturn != d_OK)
                    {
			//指令傳送失敗	
                        showMesg(d_SCARD_TRANSMIT_FAIL);
                      
                    }else
                    {
                        //指令傳送成功
                        page_Loading(baLVRec, usLDLength);
                    }
                
                }else
                {
                    //根據狀況顯示加值失敗的ErroCode
                    showMesg(usReturn);
                   
                }
                
                usReturn = ICCPowerOff();
                if(usReturn !=d_OK)
                {
                    showMesg(usReturn);
                }
                CTOS_KBDGet(&bKey);
                break;
            case d_KBD_3:
                
                CTOS_LCDTClearDisplay();
                memset(baRecvBuffer, 0x00, sizeof(baRecvBuffer));
                memset(&stRN_Info, 0x00, sizeof(RN_Info));
                usReturn = getInfoWithRN(baRecvBuffer,&usRecvLength);
                if (usReturn != d_OK)
                {
                    showMesg(d_SCARD_ESTABLISH_FAIL);
                    ICCPowerOff();
                    CTOS_KBDGet(&bKey);
                    break;
                }else
                {
                
                    usReturn = RNinfoProcessing(baRecvBuffer, &stRN_Info);
                   
                    if (usReturn != d_OK)
                    {
                        showMesg(d_RN_INFO_ERROR);
                        ICCPowerOff();
                        CTOS_KBDGet(&bKey);
                        break;
                    }
                    
           
                    memset(baInput, 0x00, sizeof(baInput));
                    usReturn = showInput(baInput ,&iInputLen);
                    if(usReturn ==d_INPUT_CANCEL)
                    {
                        break;
                    }
                    usReturn = CheckAndDeductData(stRN_Info.lRN_DA,baInput,iInputLen,stRN_Info.baRN_DV,stRN_Info.baRN_DSN_OR_TSN);
                
                }
                if (usReturn == d_OK)
                {
                    deductDTAC(stRN_Info.baRN_DSN_OR_TSN,stRN_Info.baRN_DV,baLTAC);
                    usReturn = DeductCommand(baLTAC,stRN_Info.baRN_DV , baDVRec, usLDLength);
                    if (usReturn != d_OK)
                    {
			//指令傳送失敗	
                        showMesg(d_SCARD_TRANSMIT_FAIL);
                      
                    }else
                    {
                        getPTAC(stRN_Info.baRN_DSN_OR_TSN, stRN_Info.baRN_DV, baPTAC);
                        //結果相同 之後繼續修改show在螢幕上

                        page_Deduct(baDVRec, baPTAC);
                    }
                
                }else
                {
                    //根據狀況顯示加值失敗的ErroCode
                    showMesg(usReturn);
                   
                }
                
                usReturn = ICCPowerOff();
                if(usReturn !=d_OK)
                {
                    showMesg(usReturn);
                }
                CTOS_KBDGet(&bKey);
                break;
            case d_KBD_CANCEL:
                
                i=d_PAGE_OUT; 
                break;    
         
        }
       
    }
	 
}




