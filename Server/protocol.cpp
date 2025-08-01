#include "protocol.h"
PDU *mkPDU(uint uiMsgLen)
{
    uint uiPDULen = sizeof (PDU)+uiMsgLen;
    PDU* pdu = (PDU*) malloc(uiPDULen);
    if(pdu==NULL){
        exit(1);
    }
    memset(pdu,0,uiPDULen);
    pdu->uiMsgLen=uiMsgLen;
    pdu->uiPDULen=uiPDULen;
    return pdu;
}
