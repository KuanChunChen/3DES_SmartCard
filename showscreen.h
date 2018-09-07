 

#ifndef SHOWSCREEN_H
#define SHOWSCREEN_H
#include <ctosapi.h>
#ifdef __cplusplus
extern "C" {
#endif

 
    USHORT page_home(LONG lPageID);
    USHORT page_RN_info(RN_Info stRN);
     
    void showMesg(USHORT usMeg);
    USHORT showInput(OUT BYTE *baInput ,OUT int *iInputLen);
    USHORT page_Loading(BYTE *baRecvBuffer, USHORT usRecvLength);
    USHORT page_Deduct(BYTE *baDeduct, BYTE *baPTAC);
#ifdef __cplusplus
}
#endif

#endif /* SHOWSCREEN_H */

