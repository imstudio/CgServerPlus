#ifndef USERACHIEVEMENTCONFIG_H
#define USERACHIEVEMENTCONFIG_H

#include <util/common_define.h>
#include <util/stringutil.h>

BEGIN_NAMESPACE(slots)

struct UserAchievementConfig{
    int32_t id;
    int32_t reward_type;
    int32_t type;
    int32_t value;
    int64_t target;
    int64_t reward;
    bool deserialize(const std::vector<std::string> &row) {
        	if (row.size() < 6) {
            CLOG(WARNING) << "Invalid colum size.";
            return false;
        }
        bool ret = cgserver::StringUtil::StrToInt32(row[0].c_str(), id);
        ret = ret && cgserver::StringUtil::StrToInt64(row[1].c_str(), target);
        ret = ret && cgserver::StringUtil::StrToInt64(row[2].c_str(), reward);
        ret = ret && cgserver::StringUtil::StrToInt32(row[3].c_str(), reward_type);
        ret = ret && cgserver::StringUtil::StrToInt32(row[4].c_str(), type);
        ret = ret && cgserver::StringUtil::StrToInt32(row[5].c_str(), value);
        if (!ret) {
            CLOG(WARNING) << "Invalid number in colum";
        }
        return ret;
    }
};

#define CjSetting UserAchievementConfig
DF_SHARED_PTR(CjSetting);
typedef std::vector<CjSetting> CjSettings;
typedef std::map<int32_t, CjSettings> CjSettingMap;

END_NAMESPACE
#endif
