#include "cjqueue.h"
#include <slots/sql/batchsql.h>

using namespace cgserver;
BEGIN_NAMESPACE(slots)

CjQueue::CjQueue():_curWrite(0), _curRead(1){
}

CjQueue::~CjQueue(){
}

void CjQueue::save2MySQL(uint64_t factor) {
  swapQueue();
  CLOG(INFO) << "Dump achievement info to db begin.";
  std::vector<std::string> sqls;
  getSqls(sqls);
  BatchSql bs(sqls);
  auto &pool = MysqlConnPool::getInstance();
  if(!pool.doMysqlOperation((MysqlOperationBase *) &bs)) {
    // if there has error, we should get reason from log
    CLOG(WARNING) << "Save achievement to mysql failed.";
  }
  CLOG(INFO) << "Dump achievement info to db end.";
}

void CjQueue::addCj(std::vector<UserCJ> &newCJ) {
  MUTEX_GUARD(_lock);
  for (auto &item: newCJ) {
    _queue[_curWrite].push(item);
  }
}

void CjQueue::addCj(UserCJ &newCJ) {
  MUTEX_GUARD(_lock);
  _queue[_curWrite].push(newCJ);
}

void CjQueue::getSqls(std::vector<std::string> &sqls) {
  auto &q = _queue[_curRead];
  while(!q.empty()) {
    auto &i = q.front();
    if (i.status == CJS_NEW){
        // this achievement has not been saved to db
        MysqlSimpleInsert msi;
        msi.setTable(UserCJStr::sTableName);
        msi.setField(UserCJStr::sUid);
        msi.addField(UserCJStr::sAid);
        msi.addField(UserCJStr::sRecv);
        msi.addField(UserCJStr::sTime);
        msi.setValue(StringUtil::toString(i.uid));
        msi.addValue(StringUtil::toString(i.aid));
        msi.addValue(i.isRecvReward ? sMysqlTrue : sMysqlFalse);
        msi.addValue(StringUtil::toString(i.time));
        sqls.push_back(msi.getQuery());
    }else if (i.status == CJS_CHANGED) {
        // we should update this achievement, just record it recved
        MysqlSimpleUpdate msu;
        msu.setTable(UserCJStr::sTableName);
        msu.setUpdateValue(UserCJStr::sRecv, i.isRecvReward ? sMysqlTrue : sMysqlFalse);
        msu.setCondition(UserCJStr::sAid, StringUtil::toString(i.aid), false);
        msu.addCondition(UserCJStr::sUid, StringUtil::toString(i.uid), true, false);
        sqls.push_back(msu.getQuery());
    }
    q.pop();
  }
}

void CjQueue::swapQueue() {
  MUTEX_GUARD(_lock);
  _curWrite ^= _curRead;
  _curRead ^= _curWrite;
  _curWrite ^= _curRead;
}

END_NAMESPACE
