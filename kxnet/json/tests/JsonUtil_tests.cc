#include <kxnet/json/JsonType.h>
#include <kxnet/json/JsonUtil.h>

#include <iostream>
#include <string>

using namespace std;
using namespace kxnet;

string typeString(JsonType::Type type)
{
    switch (type)
    {
        case JsonType::VoidType:
            return "VoidType";
        case JsonType::NumberType:
            return "NumberType";
        case JsonType::BooleanType:
            return "BooleanType";
        case JsonType::StringType:
            return "StringType";
        case JsonType::ListType:
            return "ListType";
        case JsonType::MapType:
            return "MapType";
        default:
            return "";
    }
}

void NullString_test()
{
    string nullString = "";
    JsonType json = JsonUtil::stringToObject(nullString).toMap();
    cout << "Json_type: " << typeString(json.type()) << endl;

    nullString = "{}";
    json = JsonUtil::stringToObject(nullString);
    JsonMap jsonMap = json.toMap();
    cout << "Json_type: " << typeString(json.type()) << endl;
    cout << "JsonMap size = " << jsonMap.size() << endl;
}

void list_test()
{
    string lst = "{\"product_list\":[\"apple\", \"orange\", \"mango\"]}";
    JsonMap jsonMap = JsonUtil::stringToObject(lst).toMap();
    JsonList jsonList = jsonMap["product_list"].toList();
    cout << "JsonList size() = " << jsonList.size() << endl;
    for (auto &it : jsonList)
    {
        cout << it.toString() << endl;
    }
}

void incomplete_test()
{
    // incomplete json string, less the right braces(})
    string errString = "{\"time\":\"122345\"";
    JsonType json = JsonUtil::stringToObject(errString);
    JsonMap  jsonMap = json.toMap();
    cout << "Json_type: " << typeString(json.type()) << endl;
    cout << "JsonMap size = " << jsonMap.size() << endl;
}

int main()
{
    NullString_test();
    list_test();
    incomplete_test();
}

