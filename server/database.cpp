#include "database.h"
#include <exception>
#include <chrono>

std::string formatString(std::string str) {
	std::string::size_type pos = 0;
	while ((pos = str.find('\'', pos)) != std::string::npos) {
		str.replace(pos, 1, "\\'");
		pos += 2;
	}
	std::string result = "'" + str + "'";
	return result;
}


Database::Database(std::string const& path) {
	if (int rc = sqlite3_open(path.c_str(), &_database); rc != 0) {
		throw std::exception{ sqlite3_errmsg(_database) };
	}
}

Database::~Database() {
	sqlite3_close(_database);
}

void Database::execute(std::string const& sql, sqlite3_callback callback, void* argument) {
	char* errmsg = 0;
	if (int rc = sqlite3_exec(_database, sql.c_str(), callback, argument, &errmsg); rc != 0) {
		std::string error = errmsg;
		sqlite3_free(errmsg);
		throw std::exception(error.c_str());
	}
}

void Database::createTableIfNotExists(std::string const& tableName, std::vector<Column> const& columns) {
	std::string sql = "CREATE TABLE IF NOT EXISTS ";
	sql = sql + tableName + "(" + columns[0].name + " " + columns[0].dataType + " " + columns[0].constraint;
	for (auto ix = 1; ix < columns.size(); ix++) {
		sql = sql + "," + columns[ix].name + " " + columns[ix].dataType + " " + columns[ix].constraint;
	}
	sql += ")";
	execute(sql);
}

void Database::insertInto(std::string const& tableName,
	std::vector<std::string> const& columns, std::vector<std::string> const& values) {
	std::string sql = "INSERT INTO " + tableName + "(" + columns[0];
	for (auto ix = 1; ix < columns.size(); ix++) {
		sql += ", " + columns[ix];
	}
	sql += ") VALUES (" + formatString(values[0]);
	for (auto ix = 1; ix < values.size(); ix++) {
		sql += ", " + formatString(values[ix]);
	}
	sql += ")";
	execute(sql);
}

void Database::update(std::string const& tableName, std::vector<std::string> const& columns,
	std::vector<std::string> const& values, std::string const& id) {
	std::string sql = "UPDATE " + tableName + " SET " + columns[0] + " = " + formatString(values[0]);
	for (auto ix = 1; ix < columns.size(); ix++) {
		sql += ", " + columns[ix] + " = " + formatString(values[ix]);
	}
	sql += " WHERE id=" + id;
	execute(sql);
}

void Database::deleteObject(std::string const& tableName, std::string const& id) {
	std::string sql = "DELETE FROM " + tableName + " WHERE id=" + id;
	execute(sql);
}

std::string Database::selectId(std::string const& tableName, std::string const& id) {
	return select(tableName, "id=" + id);
}

std::string Database::select(std::string const& tableName, std::string const& condition) {
	std::string sql = "SELECT * FROM " + tableName + " WHERE " + condition;
	std::unique_ptr<std::string> object = std::make_unique<std::string>();
	execute(sql, [](void* data, int, char** arg, char**) {
		*((std::string*)data) = arg[1];
		return 0;
		}, object.get());
	auto startTime = std::chrono::steady_clock::now();
	while ((*object) == "")
		if (std::chrono::duration<double>(std::chrono::steady_clock::now() - startTime)
			.count() >= 1.)
			throw std::exception("Timeout.");
	return *object;

}

std::vector<std::string> Database::selectAll(std::string const& tableName) {
	sqlite3_stmt* stmt = nullptr;
	char const* tail = nullptr;
	std::vector<std::string> results;
	std::string sql = "SELECT * FROM " + tableName;
	if (sqlite3_prepare_v2(_database, sql.c_str(), -1, &stmt, &tail) == 0) {
		while (sqlite3_step(stmt) == SQLITE_ROW) {
			unsigned char const* value = sqlite3_column_text(stmt, 1);
			results.push_back(reinterpret_cast<char const*>(value));
		}
	}
	sqlite3_finalize(stmt);
	return results;
}