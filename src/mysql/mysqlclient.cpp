#include "mysqlclient.h"
#include "errmsg.h"
#include "sqlselect.h"
#include "../util/stringutil.h"

namespace cgserver{

    const std::string MysqlClient::S_ItemSep = ",";
    const std::string MysqlClient::S_LeftBracket = "(";
    const std::string MysqlClient::S_RightBracket= ")";
    const std::string MysqlClient::S_Values = " values ";
    const std::string MysqlClient::S_DQuote = "\"";
    
    MysqlClient::~MysqlClient(){
	mysql_close(&_client);
    }

    MysqlClient &MysqlClient::getInstance(){
	static MysqlClient instance;
	return instance;
    }

    bool MysqlClient::initClient(const std::string &uri, const std::string &uname,
				 const std::string &pwd, const std::string &db, int port)
    {
	if (mysql_init(&_client) == NULL) {
	    return false;
	}
	//reconnect to server after lose connection.
	char value = 1;
	mysql_options(&_client, MYSQL_OPT_RECONNECT, &value);
	return (mysql_real_connect(&_client, uri.c_str(),uname.c_str(), pwd.c_str(),
				   db.c_str(), port, NULL, 0) != NULL);
    }

    bool MysqlClient::addRow(const std::string &tableName, const RowValues &rv) {
	std::string q;
	q.assign("insert into ");
	q.append(tableName);
	if (rv.size() == 0){
	    q.append("() values ()");
	}else {
	    std::string fields;
	    std::string values;
	    RowValues::const_iterator it = rv.begin();
	    fields.append(S_LeftBracket);
	    fields.append(it->first);
	    values.append(S_LeftBracket);
	    appendValue(it->second, values);
	    while(it != rv.end()) {
		fields.append(S_ItemSep);		
		fields.append(it->first);
		values.append(S_ItemSep);
		appendValue(it->second, values);		
		++it;
	    }
	    fields.append(S_RightBracket);
	    values.append(S_RightBracket);
	    q.append(fields);
	    q.append(S_Values);
	    q.append(values);
	}
	return exeQuery(q);
    }

    bool MysqlClient::addRow(
	const std::string &tableName, const std::string &field,
	const std::string &value, bool quoteValue)
    {
	std::string q = "insert into ";
	q += tableName;
	q += " (";
	q += field;
	q += ") values (";
	q += (quoteValue ? "\"":"");
	q += value;
	q += (quoteValue ? "\"":"");
	q += ")";
	return query(q);
    }

    bool MysqlClient::simpleSelect(const std::string &tb, const std::string &fd,
				   const std::string &value, uint32_t offset,
				   uint32_t size, MysqlRows &out)
    {
	std::string q;
	SqlSelect ss(q);
	ss.addTable(tb);
	ss.hasCondition();
	ss.addEqualCondition(fd, value);
	ss.setLimit(StringUtil::toString(offset), StringUtil::toString(size));
	return queryWithResult(q, out);
    }
    

    uint64_t MysqlClient::affectRows(){
	return mysql_affected_rows(&_client);
    }

    bool MysqlClient::query(const std::string &q) {
	return exeQuery(q);
    }

    /*Get single result from mysql db*/
    bool MysqlClient::queryWithResult(const std::string &q, MysqlRow &out) {
	out.clear();
	_lock.lock();
	bool ret = false;
	do {
	    if (!exeQuery(q)) {
		break;
	    }
	    if (!getSingleResult(out)) {
		break;
	    }
	    ret = true;
	}while(0);
	_lock.unlock();
	return ret;
    }

    /*Get lots result from mysql db*/
    bool MysqlClient::queryWithResult(const std::string &q, MysqlRows &out) {
	out.clear();
	_lock.lock();
	bool ret = false;
	do {
	    if (!exeQuery(q)) {
		break;
	    }
	    if (!getAllResult(out)) {
		break;
	    }
	    ret = true;
	}while(0);
	_lock.unlock();
	return ret;
    }

    bool MysqlClient::insertWithReturn(
	MysqlStr &insertQuery, MysqlStr &selectQuery, MysqlRow &out)
    {
	bool ret = false;
	do {
	    if (!query(insertQuery)) {
		break;
	    }
	    if (!queryWithResult(selectQuery, out)) {
		break;
	    }
	    ret = true;
	}while(0);
	return ret;
    }

    bool MysqlClient::startTransaction() {
	_lock.lock();
	return exeQuery("START TRANSACTION");
    }
    
    bool MysqlClient::endTransaction(bool commit) {
	bool ret;
	if (commit) {
	    ret = exeQuery("COMMIT");
	} else {
	    ret = exeQuery("ROLLBACK");
	}
	_lock.unlock();
	return ret;
    }

    bool MysqlClient::exeQuery(const std::string &q) {
	CLOG(INFO) << "run query:"<< q ;
	_lock.lock();
	int ret = mysql_query(&_client, q.c_str());
	_lock.unlock();
	if (ret != 0) {
	    switch(ret) {
	    case CR_COMMANDS_OUT_OF_SYNC:
		CLOG(ERROR) << "ERROR:CR_COMMANDS_OUT_OF_SYNC" ;
		break;
	    case CR_SERVER_GONE_ERROR:
		CLOG(ERROR) << "ERROR:CR_SERVER_GONE_ERROR" ;
		break;
	    case CR_SERVER_LOST:
		CLOG(ERROR) << "ERROR:CR_SERVER_LOST" ;
		break;
	    default:
		CLOG(ERROR) << "Unknow error." ;
	    }
	    return false;
	}
	return true;
    }
    
/***Private method***/
    void MysqlClient::appendValue(const std::string &sVal, std::string sDest) {
	sDest.append(S_DQuote);
	sDest.append(sVal);
	sDest.append(S_DQuote);	
    }

    /*Get first result from mysql db.*/
    bool MysqlClient::getSingleResult(MysqlRow &out){
	MYSQL_RES *res = mysql_store_result(&_client);
	// no result.
	if (res == NULL ) {
	    CLOG(ERROR) << "Store result from mysql failed.\n";
	    return false;
	}
	// char **
	MYSQL_ROW row = mysql_fetch_row(res);
	while(row != NULL) {
	    unsigned long *lens = mysql_fetch_lengths(res);
	    int fieldNum = mysql_num_fields(res);
	    for (size_t i = 0; i < fieldNum; ++i) {
		if (row[i] == NULL) {
		    out.push_back("");
		}else {
		    out.push_back(std::string(row[i], lens[i]));
		}
	    }
	    // just one result
	    break;
	}
	// release result.
	mysql_free_result(res);
	return true;
    }

    bool MysqlClient::getAllResult(MysqlRows &out){
	MYSQL_RES *res = mysql_store_result(&_client);
	// no result.
	if (res == NULL ) {
	    CLOG(ERROR) << "Store result from mysql failed.\n";
	    return false;
	}
	// char **
	MYSQL_ROW row;
	unsigned long *lens;
	int fieldNum = mysql_num_fields(res);
	while((row = mysql_fetch_row(res))) {
	    lens = mysql_fetch_lengths(res);
	    MysqlRow singleRow;
	    for (size_t i = 0; i < fieldNum; ++i) {
		if (row[i] == NULL) {
		    singleRow.push_back("");
		}else {
		    singleRow.push_back(std::string(row[i], lens[i]));
		}
	    }
	    out.push_back(singleRow);
	}
	// release result.
	mysql_free_result(res);
	return true;
    }
    
}
