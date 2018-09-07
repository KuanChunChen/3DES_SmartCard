
#ifndef LOADING_H
#define LOADING_H
#include <ctosapi.h>
#include "define.h"
#ifdef __cplusplus
extern "C" {
#endif

USHORT getNewLoadingKey (OUT DES_cblock *stNewLK,OUT  DES_cblock *stNewDK,OUT DES_cblock *stNewPK);
USHORT loadingLTAC(IN BYTE * baLSN, IN BYTE * baLRn ,IN BYTE *baLV,OUT BYTE *baTAC);
USHORT LoadingCommand(IN BYTE *baLTAC,IN BYTE *baLV , OUT BYTE *baRecvBuffer,OUT USHORT *usRecvLength);
USHORT CheckAndLoadingData(IN LONG lRN_LA,IN BYTE *baInputDV,IN int iInputLen,OUT BYTE *baLV,IN BYTE *baLSn);

#ifdef __cplusplus
}
#endif

#endif /* LOADING_H */

