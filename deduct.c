
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctosapi.h>
#include "define.h"

USHORT getNewDeductKey(OUT DES_cblock *stNewLK,OUT DES_cblock *stNewDK,OUT DES_cblock *stNewPK)
{
	//用給定的seeding key 初始化
	DES_cblock stSLK;
	DES_cblock stSDK;
	DES_cblock stSPK;
	//解密資料 PID
	const_DES_cblock stCleartext;
	// cblock 轉成 schedule 後拿去加解密
	DES_key_schedule stSchedule_LK, stSchedule_DK, stSchedule_PK;

	//初始化 key值
	memcpy(stSLK, d_DEDUCT_KEY_LK, 8);
	memcpy(stSDK, d_DEDUCT_KEY_DK, 8);
	memcpy(stSPK, d_DEDUCT_KEY_PK, 8);
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


USHORT deductDTAC(IN BYTE * baDSN,IN BYTE *baDV, OUT BYTE *baTAC)
{

	//--*Local Variable*--//	
	//存放 3des cbc 時的in 與out
	BYTE baCleantext[16 + 1];
	BYTE baCiphertext[16 + 1];
	//存初始向量
	DES_cblock  stIvec;
	//存新算出來的key 3把
	KeyStructure stNewKey;
	//組合2組資料成一組
        memset(baCleantext, 0x00, sizeof(baCleantext));
	memcpy(baCleantext, baDSN, 8);
	memcpy((baCleantext + 8), baDV, 8);
	
	//此時baCleantext已建置成功

	//初始化stIvec
	memcpy(stIvec, d_ICV, 8);
	//初始化 stNewKey
	memset(&stNewKey, 0x00, sizeof(KeyStructure));
	//經過des解密後取得新的 Loading Key
	getNewDeductKey(&stNewKey.stLK, &stNewKey.stDK, &stNewKey.stPK);

	//這邊輸出 stNewKey.stLK stNewKey.stDK stNewKey.stPK
	//即可看到算出的 LK DK PK


	//新取得的new key 轉成schedule
	DES_set_key_unchecked(&stNewKey.stLK, &stNewKey.stSchedule_LK);
	DES_set_key_unchecked(&stNewKey.stDK, &stNewKey.stSchedule_DK);
	DES_set_key_unchecked(&stNewKey.stPK, &stNewKey.stSchedule_PK);


	//分段做加解密
	//根據陣列長度 ,每八byte做一次加密
        int i;
	for (i = 0; i < sizeof(baCleantext); i += 8)
	{
		//此function 參數 (in 明文, out 結果 ,in 欲加密長度,in key1, in key 2 ,in key 3 ,in [DES_ENCRYPT or DES_DNCRYPT] )
		DES_ede3_cbc_encrypt(
			baCleantext + i,
			baCiphertext + i,
			8,
			&stNewKey.stSchedule_LK, &stNewKey.stSchedule_DK, &stNewKey.stSchedule_PK,
			&stIvec,
			DES_ENCRYPT);
	}

	//將結果放入baTAC
	for (i = 0; i < 8; i++)
		baTAC [i]= baCiphertext[i + 8];

	//最終baTAC建置完成
	//這邊printf 可以看到完整baTAC


	return d_OK;

}

USHORT CheckAndDeductData(IN LONG lRN_DA,IN BYTE *baInputDV,IN int iInputLen,OUT BYTE *baDV,IN OUT BYTE *baDSn)
{

	//輸入加值金額錯誤
	BYTE baDVBuff[8+1];
	 
       
	BYTE baLSnbuff[8+1];
        int w;
        int index=0;
        int iZeroTest=0;
        LONG lInput=0;
        for(w=0;w<iInputLen;w++)
        {
            if(lInput == 0)
            {
                lInput = baInputDV[w]-48;
            }else
            {
                lInput = lInput*10 ;
                lInput +=  baInputDV[w]-48;
            }
        }
     
        if(lInput + lRN_DA>d_MAX_LV_DV_VALUE)
        { 
            return d_DEDUCT_VAULE_OVERFLOW;
        }
        //建置DV array
        memset(baDVBuff, 0x30, 8);
        
        for(w=iInputLen;w>0;w--)
        {
            baDVBuff[8-w]=baInputDV[index];
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
        //OUT baDV
        memcpy(baDV, baDVBuff, 8);
        //
	int i;
	memset(baLSnbuff, 0x00, 8);
        baLSnbuff[8]="\0";
	for ( i = 0; i < d_KEY_LENGTH; i++)
		baLSnbuff[i] = baDSn[i] - 48;
 
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
		baDSn[i] = baLSnbuff[i];
        
       

	return d_OK;

}

USHORT DeductCommand(IN BYTE *baDTAC,IN BYTE *baDV , OUT BYTE *baRecvBuffer,OUT USHORT *usRecvLength)
{	
	USHORT usReturn;
	BYTE babuff[22];
        BYTE bKey;
	BYTE baDeductCommand[d_DEDUCT_COMMAND_LENGTH];
        BYTE baRec[d_DEDUCT_REC_LENGTH];
	USHORT usTotalCommand_Length = 22;
        USHORT usRLEN;
	memset(baDeductCommand, 0x00, sizeof(baDeductCommand));
	memcpy(baDeductCommand, d_DEDUCT_COMMAND, 5);
	memcpy((baDeductCommand + 5), baDV, 8);
	memcpy((baDeductCommand + 13), baDTAC, 8);
	memcpy((baDeductCommand + 21), d_DEDUCT_LE, 1);
	//baLoadingCommand = 加值指令
	//這邊指令已經建置完成
        CTOS_LCDTClearDisplay();

    
	usReturn = CTOS_SCSendAPDU(d_SC_USER, baDeductCommand, usTotalCommand_Length,baRec, &usRLEN);
       
        if (usReturn != d_OK)
        {
            
            memset(babuff, 0x00, sizeof (babuff));
            sprintf(babuff, "error:%X",usReturn );
            CTOS_LCDTPrintXY(1, 8, babuff);
            CTOS_KBDGet(&bKey);
            return;
        }
        memcpy(baRecvBuffer, baRec, sizeof(baRec));
        //*usRecvLength=usRLEN;
	return d_OK;
}

USHORT getNewPurchaseKey(OUT DES_cblock *stNewLK,OUT DES_cblock *stNewDK,OUT DES_cblock *stNewPK)
{
    //用給定的seeding key 初始化
    DES_cblock stSLK;
    DES_cblock stSDK;
    DES_cblock stSPK;
    //解密資料 PID
    const_DES_cblock stCleartext;
    // cblock 轉成 schedule 後拿去加解密
    DES_key_schedule stSchedule_LK, stSchedule_DK, stSchedule_PK;

    //初始化 key值
    memcpy(stSLK, d_Purchase_KEY_LK, 8);
    memcpy(stSDK, d_Purchase_KEY_DK, 8);
    memcpy(stSPK, d_Purchase_KEY_PK, 8);
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


USHORT getPTAC(IN BYTE * baDSN,IN BYTE *baDV,OUT BYTE *baTAC)
{

	//--*Local Variable*--//	
	//存放 3des cbc 時的in 與out
	BYTE baCleantext[16];
	BYTE baCiphertext[16];
	//存初始向量
	DES_cblock  stIvec;
	//存新算出來的key 3把
	KeyStructure stNewKey;
	//組合2組資料成一組
	memcpy(baCleantext, baDSN, 8);
	memcpy(baCleantext + 8, baDV, 8);

	//此時baCleantext已建置成功

	//初始化stIvec
	memcpy(stIvec, d_ICV, 8);
	//初始化 stNewKey
	memset(&stNewKey, 0x00, sizeof(KeyStructure));
	//經過des解密後取得新的 Loading Key
	getNewPurchaseKey(&stNewKey.stLK, &stNewKey.stDK, &stNewKey.stPK);

	//這邊輸出 stNewKey.stLK stNewKey.stDK stNewKey.stPK
	//即可看到算出的 LK DK PK


	//新取得的new key 轉成schedule
	DES_set_key_unchecked(&stNewKey.stLK, &stNewKey.stSchedule_LK);
	DES_set_key_unchecked(&stNewKey.stDK, &stNewKey.stSchedule_DK);
	DES_set_key_unchecked(&stNewKey.stPK, &stNewKey.stSchedule_PK);


	//分段做加解密
	//根據陣列長度 ,每八byte做一次加密
        int i;
	for (i = 0; i < sizeof(baCleantext); i += 8)
	{
		//此function 參數 (in 明文, out 結果 ,in 欲加密長度,in key1, in key 2 ,in key 3 ,in [DES_ENCRYPT or DES_DNCRYPT] )
		DES_ede3_cbc_encrypt(
			baCleantext + i,
			baCiphertext + i,
			8,
			&stNewKey.stSchedule_LK, &stNewKey.stSchedule_DK, &stNewKey.stSchedule_PK,
			&stIvec,
			DES_ENCRYPT);
	}

	//將結果放入baTAC
	for (i = 0; i < 8; i++)
            baTAC[i] = baCiphertext[i + 8];

	//最終baTAC建置完成
	//這邊printf 可以看到完整baTAC


	return d_OK;

}



