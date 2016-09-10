#include "achievementprocessor.h"
#include <util/timeutil.h>
#include <slots/data/slotsdatacenter.h>

BEGIN_NAMESPACE(slots)

AchievementProcessor::AchievementProcessor():_config(SlotsConfig::getInstance().cjConfig){
}

AchievementProcessor::~AchievementProcessor(){
}

#define FIND_CJ_LIST(event, dest)		\
    auto itr = _config.find(event.e);		\
    if (itr == _config.end())			\
	return;					\
    auto &dest = itr->second;			\
    if (dest.size() == 0) return;		\

#define TAKE_ACHIEVEMENT(event, cjList)					\
    for (auto &itrItem: cjList) {					\
	if (event.curData == itrItem->target) {				\
	    UserCJ cj;							\
	    cj.uid = context.user->uInfo.uid;				\
	    cj.aid = itrItem->id;					\
	    cj.progress = event.curData;				\
	    cj.isGain = true;						\
	    cj.isRecvReward = false;					\
	    cj.time = cgserver::CTimeUtil::getCurrentTimeInSeconds();	\
	    cj.changed = true;						\
	    context.userCj.push_back(cj);				\
	    break;							\
	}								\
    }

bool AchievementProcessor::process(GameContext &context) const {
    std::vector<EventInfo> events(context.events.begin(), context.events.end());
    for (auto &item: events) {
	switch(item.e){
	case EGE_LEVEL_UP:
	case EGE_NEW_FRIEND:
	case EGE_SEND_GIFT:
	case EGE_RECV_GIFT:
	case EGE_TINY_GAME:
	case EGE_FREE_GAME:
	case EGE_MEGA_WIN:
	case EGE_BIG_WIN:
	case EGE_JACKPOT:
	case EGE_GAME_COUNT:
	case EGE_LOGIN:
	{
	    auto itr = _config.find(item.e);
	    if (itr != _config.end()){
		auto &setting = itr->second;
		TAKE_ACHIEVEMENT(item, setting);
		//take_cj(context, item, setting);
	    }
	    break;
	}
	case EGE_USE_BET:
	case EGE_EARNED_INCR:
	case EGE_LINE:
	    processRangeCj(context, item);
	    break;
	case EGE_PLAYED_GAME:
	    // do nothing
	    break;
	default:
	    CLOG(WARNING) << "Invalid event type." ;
	    break;
	}
    }
    // save achievement
    auto &cjQueue = SlotsDataCenter::instance().cjQueue;
    cjQueue->addCj(context.userCj);
    return true;
}

void AchievementProcessor::processRangeCj(GameContext &context, const EventInfo &e) const {
    FIND_CJ_LIST(e, setting);
    CjSettingPtr curCj(NULL);
    for (auto &item: setting) {
	if (e.preData < item->target
	    && e.curData >= item->target)
	{
	    UserCJ cj;
	    cj.uid = context.user->uInfo.uid;
	    cj.aid = item->id;
	    cj.progress = item->target;
	    cj.isGain = true;
	    cj.isRecvReward = false;
	    cj.time = cgserver::CTimeUtil::getCurrentTimeInSeconds();
	    cj.changed = true;
	    context.userCj.push_back(cj);
	}
	// get next goal to achive
	if ((curCj.get() == NULL || item->target < curCj->target)
	    && item->target > e.curData)
	{
	    curCj = item;
	}
    }
    // just update its progress
    if (curCj.get() != NULL) {
	UserCJ cj;
	cj.uid = context.user->uInfo.uid;
	cj.aid = curCj->id;
	cj.progress = e.curData;
	cj.isGain = false;
	cj.isRecvReward = false;
	cj.time = 0;
	cj.changed = true;
	context.userCj.push_back(cj);
    }

}

#undef TAKE_ACHIEVEMENT
#undef FIND_CJ_LIST

END_NAMESPACE