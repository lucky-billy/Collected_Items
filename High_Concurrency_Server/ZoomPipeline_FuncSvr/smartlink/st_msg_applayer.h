#ifndef ST_MSG_APPLAYER_H
#define ST_MSG_APPLAYER_H
#include <qglobal.h>
namespace ExampleServer{

#pragma  pack (push,1)

	//User Login request
	//SMARTLINK_MSG_APP::MsgType =  0x0001
	typedef struct tag_stMsg_ClientLoginReq{
		quint32 user_id;
		char Passwd[1];
	}stMsg_ClientLoginReq;

	//User Log response
	//SMARTLINK_MSG_APP::MsgType =  0x7FFE
	typedef struct tag_stMsg_ClientLoginRsp{
		quint8 DoneCode;            //0- successful, 1-redirect, 3-failed.
		quint32 UserID;
		quint8 Address_Redirect[64];// for server-cluster balance, may be this login should be re-direct to another address
		quint16 port_Redirect;      // and a port num.
	} stMsg_ClientLoginRsp;

	//0x1002
	typedef struct tag_stMsg_ClientLogoutReq{
		char UserName[1];//max 32
	}stMsg_ClientLogoutReq;

	//0x7FFD
	typedef struct tag_stMsg_ClientLogoutRsp{
		quint8 DoneCode;
	}stMsg_ClientLogoutRsp;

	//UploadUserListReq,0x1003
	typedef struct tag_stMsg_UploadUserListReq{
		quint16 UserNum;
		quint32 pUserIDList[1];
	}stMsg_UploadUserListReq;

	//User Log response
	//SMARTLINK_MSG_APP::MsgType =  0x7FFC
	typedef struct tag_stMsg_UploadUserListRsp{
		quint8 DoneCode;
		//char TextInfo[64];
	} stMsg_UploadUserListRsp;


	//SMARTLINK_MSG_APP::MsgType =  0x1004
	typedef struct tag_stMsg_DownloadUserListReq{

	} stMsg_DownloadUserListReq;

	//SMARTLINK_MSG_APP::MsgType =  0x7FFB
	typedef struct tag_stMsg_DownloadUserListRsp{
		quint8 DoneCode;
		quint16 UserNum;
		quint32 pUserIDList[1];
	} stMsg_DownloadUserListRsp;



	typedef struct tag_example_app_layer{
		struct tag_app_layer_header{
			quint16 MsgFmtVersion;
			quint8 AskID;
			quint16 MsgType;
		} header;
		union  union_MsgUnion
		{
			stMsg_ClientLoginReq msg_ClientLoginReq;
			stMsg_ClientLoginRsp msg_ClientLoginRsp;
			stMsg_UploadUserListReq  msg_UploadUserListReq;
			stMsg_UploadUserListRsp  msg_UploadUserListRsp;
			stMsg_DownloadUserListReq msg_DownloadUserListReq;
			stMsg_DownloadUserListRsp msg_DownloadUserListRsp;
			stMsg_ClientLogoutReq msg_ClientLogoutReq;
			stMsg_ClientLogoutRsp msg_ClientLogoutRsp;

		}MsgUnion;

	} EXAMPLE_MSG_APP;



#pragma pack(pop)

}
#endif // ST_MSG_APPLAYER_H
