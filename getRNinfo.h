 

#ifndef GETRNINFO_H
#define GETRNINFO_H
#include <ctosapi.h>
#include "define.h"
#ifdef __cplusplus
extern "C" {
#endif


USHORT getInfoWithRN(OUT BYTE *baRecvBuffer,OUT USHORT *usRecvLength);
USHORT RNinfoProcessing(OUT BYTE *baProcessRN,IN RN_Info *stRN);
USHORT ICCPowerOff(void);

#ifdef __cplusplus
}
#endif

#endif /* GETRNINFO_H */

