
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctosapi.h>
#include "define.h"


#include <openssl/des.h>


USHORT getNewLoadingKey (OUT DES_cblock *stNewLK,OUT  DES_cblock *stNewDK,OUT DES_cblock *stNewPK)
{	
	//用給定的seeding key 初始化
	DES_cblock stSLK ;
	DES_cblock stSDK ;
	DES_cblock stSPK ;
	//解密資料 PID
	const_DES_cblock stCleartext;
	// cblock 轉成 schedule 後拿去加解密
	DES_key_schedule stSchedule_LK, stSchedule_DK, stSchedule_PK;
 
	//初始化 key值
	memcpy(stSLK, d_LOADING_KEY_LK, 8);
	memcpy(stSDK, d_LOADING_KEY_DK, 8);
	memcpy(stSPK, d_LOADING_KEY_PK, 8);
	memcpy(stCleartext, d_PID, 8);
	//cblock 轉換成schedule
	DES_set_key_unchecked(&stSLK, &stSchedule_LK);
	DES_set_key_unchecked(&stSDK, &stSchedule_DK);
	DES_set_key_unchecked(&stSPK, &stSchedule_PK);

	//解密 並out 三把key
	
	DES_ecb_encrypt(&stCleartext, stNewLK, &stSchedule_LK, DES_DECRYPT);
	DES_ecb_encrypt(&stCleartext, stNewDK, &stSchedule_DK, DES_DECRYPT);
	DES_ecb_encrypt(&stCleartext, stNewPK, &stSchedule_PK, DES_DECRYPT);
	




	return d_OK;
}


USHORT loadingLTAC(IN BYTE * baLSN, IN BYTE * baLRn ,IN BYTE *baLV,OUT BYTE *baTAC)
{

    
	//--*Local Variable*--//	
	//存放 3des cbc 時的in 與out
	BYTE baCleantext[24 + 1];
	BYTE baCiphertext[24 + 1];
	//存初始向量
	DES_cblock  stIvec;
	//存新算出來的key 3把
	KeyStructure stNewKey;
        
        
        //BYTE babuff[8];
       
        
	//組合三組資料成一組
        memset(baCleantext, 0x00, sizeof(baCleantext));
	memcpy(baCleantext, baLSN, 8);
	memcpy((baCleantext + 8), baLV, 8);
	memcpy((baCleantext + 16), baLRn, 8);
	//此時baCleantext已建置成功

	//初始化stIvec
	memcpy(stIvec, d_ICV, 8);
	//初始化 stNewKey
	memset(&stNewKey, 0x00, sizeof(KeyStructure));
	//經過des解密後取得新的 Loading Key
	getNewLoadingKey(&stNewKey.stLK, &stNewKey.stDK, &stNewKey.stPK);
	
	//這邊輸出 stNewKey.stLK stNewKey.stDK stNewKey.stPK
	//即可看到算出的 LK DK PK
	
	
	//新取得的new key 轉成schedule
	DES_set_key_unchecked(&stNewKey.stLK, &stNewKey.stSchedule_LK);
	DES_set_key_unchecked(&stNewKey.stDK, &stNewKey.stSchedule_DK);
	DES_set_key_unchecked(&stNewKey.stPK, &stNewKey.stSchedule_PK);


	//分段做加解密
	//根據陣列長度 ,每八byte做一次加密
        int w;
        //memset(baCiphertext, 0x00, sizeof(baCiphertext));
	for ( w = 0; w < sizeof(baCleantext); w+= 8)
	{
		//此function 參數 (in 明文, out 結果 ,in 欲加密長度,in key1, in key 2 ,in key 3 ,in [DES_ENCRYPT or DES_DNCRYPT] )
		DES_ede3_cbc_encrypt(baCleantext + w,
                        baCiphertext + w,
                        8,
                        &stNewKey.stSchedule_LK, &stNewKey.stSchedule_DK, &stNewKey.stSchedule_PK,
                        &stIvec,
                        DES_ENCRYPT);
	}
	
	//將結果放入baTAC
        int i;
	for (i = 0; i < 8; i++)
            baTAC [i] = baCiphertext[i + 16];
        
	//最終baTAC建置完成
	//這邊printf 可以看到完整baTAC
	
	 
	 return d_OK;

}



USHORT LoadingCommand(IN BYTE *baLTAC,IN BYTE *baLV , OUT BYTE *baRecvBuffer,OUT USHORT *usRecvLength)
{	
	USHORT usReturn;
	 
	BYTE baLoadingCommand[d_LOADING_COMMAND_LENGTH];
        BYTE baRec[d_LOADING_REC_LENGTH];
	USHORT usTotalCommand_Length = 22;
        USHORT usRLEN;
	memset(baLoadingCommand, 0x00, sizeof(baLoadingCommand));
	memcpy(baLoadingCommand, d_LOADING_COMMAND, 5);
	memcpy((baLoadingCommand + 5), baLV, 8);
	memcpy((baLoadingCommand + 13), baLTAC, 8);
	memcpy((baLoadingCommand + 21), d_LOADING_LE, 1);
	//baLoadingCommand = 加值指令
	//這邊指令已經建置完成
        CTOS_LCDTClearDisplay();

        memset(baRec, 0x00, sizeof(baRec));
	usReturn = CTOS_SCSendAPDU(d_SC_USER, baLoadingCommand, usTotalCommand_Length,baRec, &usRLEN);
       
        if (usReturn != d_OK)
        {
            return d_SENT_APDU_FAIL;
        }
        memcpy(baRecvBuffer, baRec, sizeof(baRec));
        //*usRecvLength=usRLEN;
	return d_OK;
}

USHORT CheckAndLoadingData(IN LONG lRN_LA,IN BYTE *baInputLV,IN int iInputLen,OUT BYTE *baLV,IN BYTE *baLSn)
{

	//輸入加值金額錯誤
	BYTE baDVBuff[8+1];
	
	BYTE baLSnbuff[8 + 1];
        int w;
        int index=0;
        int iZeroTest=0;
        LONG lInput=0;
        //stRN.lRN_Input = 0;
        for(w=0;w<iInputLen;w++)
        {
            if(lInput == 0)
            {
                lInput = baInputLV[w]-48;
            }else
            {
                lInput = lInput*10 ;
                lInput +=  baInputLV[w]-48;
            }
        }  
        
        if(lInput + lRN_LA>d_MAX_LV_DV_VALUE)
        { 
            return d_LOADING_VAULE_OVERFLOW;
        }
        
        //建置LV array
        memset(baDVBuff, 0x30, 8);
        
        for(w=iInputLen;w>0;w--)
        {
            baDVBuff[8-w]=baInputLV[index];
            index++;
        }
       for(w=0;w<8;w++)
       {
           if(baDVBuff[w]=='\x30')
           {
               iZeroTest++;
           }
           if(iZeroTest==8 )
           {
               return d_LOADING_VAULE_INVALID;
           }
       }
        //OUT baLV
        memcpy(baLV, baDVBuff, 8);
        //
	int i;
	memset(baLSnbuff, 0x00, 8);
        baLSnbuff[8]='\0';
	for ( i = 0; i < d_KEY_LENGTH; i++)
		baLSnbuff[i] = baLSn[i] - 48;
 
	for (i = d_KEY_LENGTH - 1; i >=0; i--)
	{
		//從陣列最後面加一 
		//如果不是則往前
		if (baLSnbuff[i] != '\x09')
		{
			baLSnbuff[i] += '\x01';
			break;
		}
		else
		{
			baLSnbuff[i] = '\x00';

		}

		if (i == 0 && baLSnbuff[i]=='\x00')
		{
			//已經超越加值次數上限
			return d_LOADING_COUNT_EXCEED;
		}
	}

	//轉回ascii
        
	for ( i= 0; i < d_KEY_LENGTH; i++)
		baLSnbuff[i] += 48;
	//重新放入baLSn
	//此為+1後的LSN
	for (i = 0; i < 8; i++)
		baLSn[i] = baLSnbuff[i];
        
       

	return d_OK;

}


