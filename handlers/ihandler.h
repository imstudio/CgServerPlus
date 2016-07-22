#ifndef I_HANDLER_H
#define I_HANDLER_H

#include "../http/httppacket.h"
#include "../http/httpresponsepacket.h"

namespace cgserver{
    class IHandler{
    public:
	IHandler(){}
	virtual ~IHandler(){}
	virtual bool process(HTTPPacket &packet, HttpResponsePacket &resp) = 0; // we should add const later
	virtual void init() {}
    };
}
#endif
