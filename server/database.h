#pragma once
#include <vector>
#include <string>
#include <memory>
#include <sqlite3.h>

struct Column {
	std::string name;
	std::string dataType;
	std::string constraint = "";
};

class Database {
public:
	Database(std::string const& path);
	~Database();

	void execute(std::string const& sql, sqlite3_callback callback = nullptr, void* argument = nullptr);
	void createTableIfNotExists(std::string const& tableName, std::vector<Column> const& columns);
	void insertInto(std::string const& tableName, std::vector<std::string> const& columns, std::vector<std::string> const& values);
	void update(std::string const& tableName, std::vector<std::string> const& columns,
		std::vector<std::string> const& values, std::string const& id);
	void deleteObject(std::string const& tableName, std::string const& id);
	std::string selectId(std::string const& tableName, std::string const& id);
	std::string select(std::string const& tableName, std::string const& condition);
	std::vector<std::string> selectAll(std::string const& tableName);

private:
	sqlite3* _database;
};