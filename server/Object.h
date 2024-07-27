#pragma once
#include <variant>
#include <string>
#include <vector>
#include <map>

struct ObjectMember {
public:
	std::string value;

	ObjectMember();
	ObjectMember(std::string value);

	/// <summary>
	/// 获取成员对象的类型。
	/// </summary>
	/// <returns>value 类型</returns>
	std::string getType() const;
};

struct Object {
	std::string objectName;
	std::map<std::string, ObjectMember> members;

	// https://thzt.github.io/2021/01/06/parser/
	static Object parse(std::string source);

	std::string serialize();

	std::string description() const;
};