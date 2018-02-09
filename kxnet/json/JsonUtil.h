#pragma once
#include <kxnet/json/cJSON.h>
#include <kxnet/json/JsonType.h>

namespace kxnet
{
namespace JsonUtil 
{
    string   objectToString(const JsonType &obj);
    JsonType stringToObject(const string &json);
}
}

