#ifndef ST_CROSS_SVR_MSG_H
#define ST_CROSS_SVR_MSG_H

namespace ExampleServer{

#pragma  pack (push,1)

#if defined(__GNUC__)
#include <stdint.h>
	typedef struct tag_example_crosssvr_msg{
		struct tag_msgHearder{
			__UINT16_TYPE__ Mark;    //Always be "0x4567"
			__UINT16_TYPE__ version; //Structure Version
			__UINT8_TYPE__ mesageType;
			__UINT32_TYPE__ messageLen;
		} header;
		union union_payload{
			__UINT8_TYPE__ data[1];
			__UINT32_TYPE__ uuids[1];
		} payload;
	} EXAMPLE_CROSSSVR_MSG;
#endif

#if defined(_MSC_VER)
	typedef struct tag_example_crosssvr_msg{
		struct tag_msgHearder{
			unsigned __int16 Mark;    //Always be 0x4567
			unsigned __int16 version; //Structure Version
			unsigned __int8 mesageType;
			unsigned __int32 messageLen;
		} header;
		union union_payload{
			unsigned __int8 data[1];
			unsigned __int32 uuids[1];
		} payload;
	} EXAMPLE_CROSSSVR_MSG;

#endif



#pragma pack(pop)
}
#endif
