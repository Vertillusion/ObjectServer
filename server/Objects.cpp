#include "Objects.h"
#include "Object.h"
#include <random>

Objects::Objects() {
	_database.createTableIfNotExists("objects", {
		{ "name", "STRING" },
		{ "value", "STRING" },
		{ "id", "INT" },
	});
}

void Objects::getObjects(HttpRequestPtr const& req,
	std::function<void(HttpResponsePtr const&)>&& callback) {
	Json::Value ret;

	try {
		ret["result"] = "ok";
		std::vector<std::string> result = _database.selectAll("objects");
		for (auto it = result.begin(); it != result.end(); ++it) {
			Object obj = Object::parse(*it);
			std::string name = obj.objectName;
			ret[name] = Json::arrayValue;
			for (auto const& member : obj.members) {
				ret[name].append(member.first + " " + member.second.getType());
			}
		}
	} catch (std::exception e) {
		ret["result"] = e.what();
	}

	auto resp = HttpResponse::newHttpJsonResponse(ret);
	callback(resp);
}

void Objects::getObjectDescription(HttpRequestPtr const& req,
	std::function<void(HttpResponsePtr const&)>&& callback,
	std::string const& type) {
	Json::Value ret;

	try {
		ret["result"] = "ok";
		Object object = Object::parse(_database.select("objects", "name='" + type + "'"));
		ret["value"] = Json::arrayValue;
		for (auto const& member : object.members) {
			ret["value"].append(member.first + " " + member.second.getType());
		}
	} catch (std::exception e) {
		ret["result"] = e.what();
	}

	auto resp = HttpResponse::newHttpJsonResponse(ret);
	callback(resp);
}

void Objects::getObjectContent(HttpRequestPtr const& req,
	std::function<void(HttpResponsePtr const&)>&& callback,
	std::string const& type, std::string const& id) {
	Json::Value ret;

	try {
		ret["result"] = "ok";
		ret["value"] = _database.selectId("objects", id);
	} catch (std::exception e) {
		ret["result"] = e.what();
	}

	auto resp = HttpResponse::newHttpJsonResponse(ret);
	callback(resp);
}

void Objects::createObject(HttpRequestPtr const& req,
	std::function<void(HttpResponsePtr const&)>&& callback,
	std::string const& type) {
	Json::Value ret;

	Object obj;
	try {
		obj = Object::parse(req->getBody().data());
		_database.insertInto("objects", { "name", "value", "id" }, { obj.objectName, obj.serialize(),
			 obj.members["id"].value });
		ret["result"] = "ok";
		ret["id"] = obj.members["id"].value;
	} catch (std::exception e) {
		ret["result"] = e.what();
	}

	auto resp = HttpResponse::newHttpJsonResponse(ret);
	callback(resp);
}

void Objects::changeObject(HttpRequestPtr const& req,
	std::function<void(HttpResponsePtr const&)>&& callback,
	std::string const& type, std::string const& id) {
	Json::Value ret;

	try {
		Object obj = Object::parse(req->getBody().data());
		_database.update("objects", { "name", "value" }, {obj.objectName, obj.serialize()}, id);
		ret["result"] = "ok";
	} catch (std::exception e) {
		ret["result"] = e.what();
	}

	auto resp = HttpResponse::newHttpJsonResponse(ret);
	callback(resp);
}

void Objects::deleteObject(HttpRequestPtr const& req,
	std::function<void(HttpResponsePtr const&)>&& callback,
	std::string const& type, std::string const& id) {
	Json::Value ret;

	try {
		_database.deleteObject("objects", id);
		ret["result"] = "ok";
	} catch (std::exception e) {
		ret["result"] = e.what();
	}

	auto resp = HttpResponse::newHttpJsonResponse(ret);
	callback(resp);
}