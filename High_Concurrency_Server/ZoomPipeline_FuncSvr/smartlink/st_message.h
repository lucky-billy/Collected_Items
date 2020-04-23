#ifndef ST_MESSAGE_H
#define ST_MESSAGE_H
#include <qglobal.h>
namespace ExampleServer{

#pragma  pack (push,1)

	typedef struct tag_example_message{
		quint16 Mark;    //Always be 0xAA55
		quint32 source_id ; //0xffffffff means from server
		quint32 destin_id ; //0xffffffff means to server
		quint16 data_length; //Data at least have 2 byte, for heart-beating counter.
		quint8 data[1];
	} EXAMPLE_TRANS_MSG;
	typedef struct tag_example_heartBeating
	{
		quint16 Mark;  //Always be 0xBeBe
		quint32 source_id ; //0xffffffff means from server
		quint16 tmStamp;
	} EXAMPLE_HEARTBEATING;

#pragma pack(pop)
}
#endif // ST_MESSAGE_H
