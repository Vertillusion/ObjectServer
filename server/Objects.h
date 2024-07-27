#pragma once
#include <drogon/HttpController.h>
#include "database.h"
using namespace drogon;

class Objects :public drogon::HttpController<Objects> {
public:
	Objects();

	METHOD_LIST_BEGIN
		METHOD_ADD(Objects::getObjects, "/", Get);
		METHOD_ADD(Objects::getObjectDescription, "/{1}", Get);
		METHOD_ADD(Objects::getObjectContent, "/{1}/{2}", Get);
		METHOD_ADD(Objects::createObject, "/{1}", Post);
		METHOD_ADD(Objects::changeObject, "/{1}/{2}", Put);
		METHOD_ADD(Objects::deleteObject, "/{1}/{2}", Delete);
	METHOD_LIST_END

	void getObjects(HttpRequestPtr const& req,
		std::function<void(HttpResponsePtr const&)>&& callback);
	void getObjectDescription(HttpRequestPtr const& req,
		std::function<void(HttpResponsePtr const&)>&& callback,
		std::string const& type);
	void getObjectContent(HttpRequestPtr const& req,
		std::function<void(HttpResponsePtr const&)>&& callback,
		std::string const& type, std::string const& id);
	void createObject(HttpRequestPtr const& req,
		std::function<void(HttpResponsePtr const&)>&& callback,
		std::string const& type);
	void changeObject(HttpRequestPtr const& req,
		std::function<void(HttpResponsePtr const&)>&& callback,
		std::string const& type, std::string const& id);
	void deleteObject(HttpRequestPtr const& req,
		std::function<void(HttpResponsePtr const&)>&& callback,
		std::string const& type, std::string const& id);

private:
	Database _database = Database("objects.db");
};