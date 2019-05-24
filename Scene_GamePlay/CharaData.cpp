#include "CharaData.hpp"
#include <fstream>
#include <sstream>
#include <sqlite3.h>

bool CharaData::initialize = false;
std::unordered_map<std::string, std::unordered_map<std::string, float>> CharaData:: value;

CharaData::CharaData(std::string charaKey) :
	charaKey(charaKey)
{
	if (!initialize)
	{
		sqlite3* sql3;
		int result = sqlite3_open("Data/StatusData.db", &sql3);

		if (result == SQLITE_OK)
		{
			sqlite3_stmt* statement;
			const char* command = "";

			//テーブルの名前を取り出す
			std::vector<std::string> tables;
			command = "select name from sqlite_master where type = 'table';";
			sqlite3_prepare_v2(sql3, command, -1, &statement, &command);
			while (sqlite3_step(statement) == SQLITE_ROW)
			{
				tables.emplace_back(reinterpret_cast<const char*>(sqlite3_column_text(statement, 0)));
			}

			//値をセットする
			for (const auto& tableName : tables)
			{
				std::string commandStr = "select * from " + tableName + ";";
				command = commandStr.c_str();
				sqlite3_prepare_v2(sql3, command, -1, &statement, &command);

				//フィールドの値を格納する
				while (sqlite3_step(statement) == SQLITE_ROW)
				{
					int columnCount = sqlite3_column_count(statement);
					for (int i = 0; i < columnCount; ++i)
					{
						value[tableName][sqlite3_column_name(statement, i)] = sqlite3_column_double(statement, i);
					}
				}
			}

			sqlite3_finalize(statement);
			sqlite3_close(sql3);

			initialize = true;
		}
	}
}
