#ifndef SLOTSUSERDATA_H
#define SLOTSUSERDATA_H

#include <slots/sql/slotsdb.h>
#include <cache/redisclientfactory.h>

BEGIN_NAMESPACE(slots)

namespace SlotCacheStr{
  const std::string sDailyKeyPrefix = "L";
  const std::string sLRunnerIdKey = "rID";
  const std::string sLRunnerRewardKey = "rRwd";
  const std::string sLRecvKey = "recv";
  const std::string sLVipKey = "vip";
  const std::string sLDayKey = "dRwd";
  const std::string sLRecvTrue = "1";
  const std::string sLRecvFalse = "0";
};

class SlotsUserData: public PersistenceBase{
 public:
    SlotsUserData();
    virtual ~SlotsUserData();
    virtual void save2MySQL(uint64_t factor);
    virtual bool needSave(uint64_t factor);

    bool getByUid(const std::string &uid, SlotsUserPtr &out);
    bool getByMid(const std::string &mid, SlotsUserPtr &out);
    void set(const std::string &uid, SlotsUserPtr &in);

    void setDailyReward(const std::string &userID, const LoginReward &in);
    void updateDailyReward(const std::string &userID, bool recved);
    bool getDailyReward(const std::string &userID, LoginReward &out);

private:
    std::map<std::string, SlotsUserPtr> _data;
    std::mutex _lock;

    CRedisClient &_redisClient;
};

DF_SHARED_PTR(SlotsUserData);

END_NAMESPACE
#endif
