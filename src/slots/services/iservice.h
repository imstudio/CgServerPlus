#ifndef ISERVICE_H
#define ISERVICE_H

#include <handlers/ihandler.h>
#include <util/luatoolfactory.h>
#include <util/common_define.h>
#include <util/stringutil.h>

#include <slots/resultformatter.h>
#include <slots/sql/slotsdb.h>
#include <slots/data/slotsdatacenter.h>

namespace slots{
    
#define GET_PARAM(key, dest, required)					\
    if (!packet.getParamValue(key, dest)) {				\
	if (required) {							\
	    CLOG(ERROR) << "Get [" << key << "] from url failed.";	\
	    return false;						\
	}else {								\
	    dest.clear();						\
	}								\
    }

#define GET_PARAM_WITH_BREAK(key, dest, required)					\
    if (!packet.getParamValue(key, dest)) {				\
	if (required) {							\
	    CLOG(ERROR) << "Get [" << key << "] from url failed.";	\
	    break;						\
	}else {								\
	    dest.clear();						\
	}								\
    }
    
    typedef cgserver::HTTPPacket CPacket;
    typedef cgserver::HttpResponsePacket CResponse;

#define MAX_PAGE_SIZE 30    
    
    class IService{
   public:
      static const std::string sRoomID;
      static const std::string sHallID;
      static const std::string sUserID;
      static const std::string sType;
      virtual ~IService() {}
      virtual bool doJob(CPacket &packet,CResponse &resp) =0;

      bool getIntVal(CPacket &packet, const std::string &key, uint32_t &val) {
        std::string strVal;
        GET_PARAM(key, strVal, true);
        return cgserver::StringUtil::StrToUInt32(strVal.c_str(), val);
      }

      bool getIntVal(CPacket &packet, const std::string &key, uint64_t &val) {
        std::string strVal;
        GET_PARAM(key, strVal, true);
        return cgserver::StringUtil::StrToUInt64(strVal.c_str(), val);
      }

      bool getIntVal(CPacket &packet, const std::string &key, int32_t &val) {
        std::string strVal;
        GET_PARAM(key, strVal, true);
        return cgserver::StringUtil::StrToInt32(strVal.c_str(), val);
      }

      bool getIntVal(CPacket &packet, const std::string &key, int64_t &val) {
        std::string strVal;
        GET_PARAM(key, strVal, true);
        return cgserver::StringUtil::StrToInt64(strVal.c_str(), val);
      }

    };
    const std::string IService::sRoomID = "r";
    const std::string IService::sHallID = "h";
    const std::string IService::sUserID = "u";
    const std::string IService::sType = "t";
    DF_SHARED_PTR(IService);
}
#endif
