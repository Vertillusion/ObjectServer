# Server

## 关于服务器
命令行第一个参数指定 IP 地址（默认 `127.0.0.1`），第二个参数指定端口（默认 `80`）。

## 关于 API

- 所有 API 返回都有一个 `result`，值为 `"ok"` 或相应错误信息（如 `"Timeout."`）。
- GET `/objects/`：获取所有对象类型信息
    - 返回例：
    ```json
    {
        "result": "ok",
        "type1": [
            "field1 type1",
            "field2 type2"
        ],
        "type2": [
            "field1 type1",
            "field2 type2"
        ]
    }
    ```
- GET `/objects/{type}`：获取相应类型信息
    - 返回例：
    ```json
    {
        "result": "ok",
        "value": [
            "field1 type1",
            "field2 type2"
        ]
    }
    ```
- GET `/objects/{type}/{id}`：获取对象信息
    - 返回例：
    ```json
    {
        "result": "ok",
        "value": "object{field1:value1,field2:value2,}"
    }
    ```
- POST `/objects/{type}`：创建相应类型
    - 返回例：
    ```json
    {
        "result": "ok",
        "id": 123456789
    }
    ```
- PUT `/objects/{type}/{id}`：修改相应对象
    - 返回例：
    ```json
    {
        "result": "ok"
    }
    ```
- DELETE `/objects/{type}/{id}`：删除相应对象
    - 返回例：
    ```json
    {
        "result": "ok"
    }
    ```

## 关于 Object

语法：
```
object = identifier '{' members '}'
members = member ',' members | member ',' | member
member = identifier ':' value
value = number | string
```

## 关于源代码
用的框架是 [drogon](https://github.com/drogonframework/drogon)，数据库是 sqlite（`database.cpp`）。

parser（`Object.cpp`）相关的内容看的是 [这篇文章](https://thzt.github.io/2021/01/06/parser/)，源代码 [在这](https://github.com/thzt/web.frontend.component/blob/master/library/list-parser/parser.js)。

在 `Objects.h` 头文件中添加 `METHOD_ADD` 项可以添加更多方法。