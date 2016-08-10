#ifndef RESULTFORMATTER_H
#define RESULTFORMATTER_H

#include "slotsdb.h"
#include "giftsdata.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <string>
#include <sstream>

namespace slots{
    typedef rapidjson::StringBuffer SBuf;
    typedef rapidjson::Writer<SBuf> JsonWriter;
    class ResultFormatter{
    public:
        explicit ResultFormatter(SBuf &buffer);
        ~ResultFormatter();

	void formatResult(const UserMails &uMails);
	void formatResult(const SlotsUser &su);
	void formatSimpleResult(bool ok, const std::string &err);
	void formatGameResult(const UserResource &sr, int64_t earned, const std::string &detail);
	void formatFriendsInfo(const FriendsList &friends);
	void formatGiftsInfo(const Gifts &gifts, int64_t giftsVal);
	
	void formatStatus(bool bOk);
	void formatMailsInfo(const UserMails &uMails);
	void formatMail(const UserMail &uMail);
	void formatUserInfo(const UserInfo &uInfo);
	void formatUserResource(const UserResource &uRes);
    private:
	JsonWriter _writer;
    };
}
#endif
