#ifndef CROSS_SVR_MESSAGES_H
#define CROSS_SVR_MESSAGES_H
#include <qglobal.h>
namespace ZP_Cluster{
#pragma  pack (push,1)
	typedef struct tag_cross_svr_message{
		struct tag_header{
			quint16 Mark;    //Always be 0x1234
			quint8 messagetype;
			quint32 data_length;
		}  hearder;
		union uni_payload{
			quint8 data[1];
			struct tag_CSM_heartBeating{
				quint32 nClients;
			} heartBeating;
			struct tag_CSM_BasicInfo{
				quint8 name [64];
				quint8 Address_LAN[64];
				quint16 port_LAN;
				quint8 Address_Pub[64];
				quint16 port_Pub;
			} basicInfo;
			struct tag_CSM_Broadcast{
				quint8 name [64];
				quint8 Address_LAN[64];
				quint16 port_LAN;
				quint8 Address_Pub[64];
				quint16 port_Pub;
			} broadcastMsg[1];
		} payload;
	} CROSS_SVR_MSG;

#pragma pack(pop)
}

#endif // CROSS_SVR_MESSAGES_H
