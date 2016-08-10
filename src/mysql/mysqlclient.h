#ifndef MYSQLCLIENT_H
#define MYSQLCLIENT_H
#include <string>
#include <map>
#include <vector>
#include "mysql.h"
#include "../util/common_define.h"

namespace cgserver{
    // Notice: we use key-value to indicate field-field_value in mysql.
    typedef std::map<std::string, std::string> RowValues;
    typedef std::vector<std::string> MysqlRow;
    typedef std::vector<MysqlRow> MysqlRows;
    typedef std::string MysqlStr;
    
    class MysqlClient{
    public:
	static const std::string S_ItemSep;
	static const std::string S_LeftBracket;
	static const std::string S_RightBracket;
	static const std::string S_Values;
	static const std::string S_DQuote;

	static MysqlClient &getInstance();	
        ~MysqlClient();

	bool initClient(const std::string &uri, const std::string &uname,
			const std::string &pwd,const std::string &db, int port = 3306);
	bool addRow(const std::string &tableName, const RowValues &rv);
	bool addRow(const std::string &tableName, const std::string &field,
		    const std::string &value, bool quoteValue = true);
	
	bool query(const std::string &q);
	bool simpleSelect(const std::string &tb, const std::string &fd,
			  const std::string &value, uint32_t offset,
			  uint32_t size, MysqlRows &out);

	bool queryWithResult(const std::string &q, MysqlRow &out);
	bool queryWithResult(const std::string &q, MysqlRows &out);
	
	bool insertWithReturn(MysqlStr &insertQuery, MysqlStr &selectQuery, MysqlRow &out);

	/*startTransaction will try to get lock, but won't release lock*/
	bool startTransaction();
	/*end transaction release lock, but won't get lock*/
	bool endTransaction(bool commit);
	
	uint64_t affectRows();	
	bool exeQuery(const std::string &q);
	
    private:
        MysqlClient(){}
	MysqlClient(const MysqlClient&);
	MysqlClient & operator = (const MysqlClient &);
	
	void appendValue(const std::string &sVal, std::string sDest);
	bool getSingleResult(MysqlRow &out);
	bool getAllResult(MysqlRows &out);	
	
	MYSQL _client;
	std::recursive_mutex _lock;
    };
}
#endif
