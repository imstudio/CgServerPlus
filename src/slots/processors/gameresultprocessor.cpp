#include "gameresultprocessor.h"
#include <slots/config/slotsconfig.h>
#include <slots/data/slotsdatacenter.h>

BEGIN_NAMESPACE(slots)

GameResultProcessor::GameResultProcessor(){
}

GameResultProcessor::~GameResultProcessor(){
}

bool GameResultProcessor::process(GameContext &context) const {
    auto &gInfo = context.gameInfo;
    context.events.push_back(EventInfo(EGE_PLAYED_GAME));
    processHall(context, gInfo);
    processGameDetail(context, gInfo);
    processLines(context, gInfo);
    // is free to play
    if (!gInfo.bFreeGame) {
	context.events.push_back(EventInfo(EGE_USE_BET, gInfo.bet));
    } else {
        // reduce freeGameTimes
        --gInfo.freeGameTimes;
    }
    // update user fortune
    processMoney(context, gInfo);
    // update user exp&level
    processExp(context, gInfo);
    // save this result to history
    context.user->gSt.getGameResult(context.hallID, context.roomID) = gInfo;
    return true;
}

void GameResultProcessor::processHall(GameContext &context, GameResult &data) const {
    auto &hall = SlotsDataCenter::instance().getHall(context.hallID);
    auto &cfg = SlotsConfig::getInstance().themeConfig.tsConfig;
    hall.incrPrize(cfg.getTax4Hall(data.bet));
    hall.incrPrize(context.roomID, cfg.getTax4Hall(data.bet));
    if (!data.bJackpot2 && cfg.isForceWinHallPrize(hall.getGameCount())) {
        data.bJackpot2 = true;
        data.earned.hallPrize = hall.takeHallPrize();
    }
    if (!data.bJackpot1 && cfg.isForceWinRoomPrize(hall.getGameCount(context.roomID))) {
        data.bJackpot1 = true;
        data.earned.roomPrize = hall.takeRoomPrize(context.roomID);
    }
    hall.incrGameCount();
    hall.incrGameCount(context.roomID);
}

#define INCR_TAG_VALUE(mThemeTag, mEvent, value)                        \
    tHis.incrTagValue(gType, mThemeTag, 1);                              \
    context.events.push_back(                                           \
        EventInfo(mEvent, TO_GAME_CJ_VALUE(                             \
                      gType, tHis.getTagValue(gType, mThemeTag))));


void GameResultProcessor::processGameDetail(
    GameContext &context, GameResult &data) const
{
    auto &tHis = context.user->uHis.themeHistory;
    auto gType = data.gType;
    // incr game times
    INCR_TAG_VALUE(NORMAL_GAME_TAG, EGE_GAME_COUNT, 1);
    if(data.bBigwin) {
        INCR_TAG_VALUE(BIG_WIN_TAG, EGE_BIG_WIN, 1);
    }
    if(data.bMegawin) {
        INCR_TAG_VALUE(MEGA_WIN_TAG, EGE_MEGA_WIN, 1);
    }
    if(data.bSuperwin) {
        INCR_TAG_VALUE(SUPER_WIN_TAG, EGE_SUPER_WIN, 1);
    }
    // bug: if both jackpot trigger, it will jump one number.
    if(data.bJackpot1) {
        INCR_TAG_VALUE(JACKPOT_TAG, EGE_JACKPOT, 1);
        ++context.user->uHis.jackpot;
    }
    if(data.bJackpot2) {
        INCR_TAG_VALUE(JACKPOT_TAG, EGE_JACKPOT, 1);
        ++context.user->uHis.jackpot;
    }
    if (data.tinyGame.enable) {
        INCR_TAG_VALUE(TINY_GAME_TAG, EGE_TINY_GAME, 1);
    }
    if (data.bFreeGame) {
        tHis.incrTagValue(gType, FREE_GAME_TAG, 1);
    }
    context.user->uHis.changed = true;
}

void GameResultProcessor::processLines(GameContext &context, GameResult &data) const {
    auto &tHis = context.user->uHis.themeHistory;
    auto gType = data.gType;
    auto pre = tHis.getTagValue(gType, SIX_LINK_TAG);
    for (auto &line: data.lines){
        // six link
        if (line.count == 6) {
            tHis.incrTagValue(gType, SIX_LINK_TAG, 1);
        }
    }
    context.events.push_back(
        EventInfo(EGE_LINE, TO_GAME_CJ_VALUE(gType, pre),
                  TO_GAME_CJ_VALUE(gType, tHis.getTagValue(gType, SIX_LINK_TAG))));
}

void GameResultProcessor::processExp(GameContext &context, GameResult &data) const {
    auto &uRes = context.user->uRes;
    // if zero bet then exp will not change
    if (data.bFreeGame) {
	return;
    }
    auto &slotsConfig = SlotsConfig::getInstance();
    auto expGot = slotsConfig.expGain(context, data.bet);
    uRes.incrExp(expGot);
    int64_t expNeed = 0;
    while(true) {
        expNeed = slotsConfig.levelConfig.expNeedForNextLevel(uRes.level);
        if (expNeed < 0 || expNeed > uRes.exp ) {
            return;
        }
        uRes.levelUp();
        context.events.push_back(EventInfo(EGE_LEVEL_UP, uRes.level));
    }
}

void GameResultProcessor::processMoney(GameContext &context, GameResult &data) const {
    auto &uHis = context.user->uHis;
    uHis.incrBet(data.bet);
    int64_t actualEarned = data.earned.sum() - (data.bFreeGame ? 0: data.bet);
    if (actualEarned == 0) {
	return;
    }
    UserResource &uRes = context.user->uRes;
    uRes.incrFortune(actualEarned);
    // update max fortune
    uHis.newFortune(uRes.fortune);
    // update max earned
    uHis.newEarned(data.earned.normal);
    // update earned (include this week and total)
    auto pre = uHis.totalEarned;
    uHis.incrEarned(data.earned.sum());
    if (pre != uHis.totalEarned) {
	// if total earned changed, create event.
	context.events.push_back(EventInfo(EGE_EARNED_INCR, pre, uHis.totalEarned));
    }
}

END_NAMESPACE
