#include "protocol.h"
PDU *mkPDU(uint uiMsgLen)
{
    uint uiPDULen = sizeof (PDU)+uiMsgLen;//消息长度+结构体长度来算总长度。

    PDU* pdu = (PDU*) malloc(uiPDULen);//根据总长度去申请空间。需要头文件string.h
    if(pdu==NULL){//申请好空间后需要判空。
        exit(1);
    }
    memset(pdu,0,uiPDULen);//申请的空间要清空一下
    pdu->uiMsgLen=uiMsgLen;//初始化两个长度
    pdu->uiPDULen=uiPDULen;
    return pdu;
}
