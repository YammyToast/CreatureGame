#ifndef _MESSAGES_H_
#define _MESSAGES_H_

#include <nlohmann/json.hpp>
using json = nlohmann::json;

enum InMessageTypes
{
    GETUPDATE,
    ACTION

};

struct InMessageWrapper
{
    InMessageTypes type;
    json *body;
};

bool verify_data_is_json(const std::string& __json_string);
InMessageWrapper parse_incoming_json(const std::string& __json_string);

#endif