 

#ifndef DEDUCT_H
#define DEDUCT_H
#include "openSSL/des.h"
#include <ctosapi.h>
#include "define.h"
#ifdef __cplusplus
extern "C" {
#endif


//(in / in /out)
USHORT deductDTAC(IN BYTE * baDSN,IN BYTE *baDV, OUT BYTE *baTAC);
//(out / out / out)
USHORT getNewDeductKey(OUT DES_cblock *stNewLK,OUT DES_cblock *stNewDK,OUT DES_cblock *stNewPK);
//(in / in / in / out / in)
USHORT CheckAndDeductData(IN LONG lRN_DA,IN BYTE *baInputDV,IN int iInputLen,OUT BYTE *baDV,IN BYTE *baDSn);
//( in / in / out / out)
USHORT DeductCommand(IN BYTE *baDTAC,IN BYTE *baDV , OUT BYTE *baRecvBuffer,OUT USHORT *usRecvLength);
//(out / out / out)
USHORT getNewPurchaseKey(OUT DES_cblock *stNewLK,OUT DES_cblock *stNewDK,OUT DES_cblock *stNewPK);
//(in / in /out)
USHORT getPTAC(IN BYTE * baDSN,IN BYTE *baDV,OUT BYTE *baTAC);


#ifdef __cplusplus
}
#endif

#endif /* DEDUCT_H */

