#ifndef _MESSAGE_CPP_
#define _MESSAGE_CPP_

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include "messages.h"


// ==================================================
// INCOMING MESSAGES
// ==================================================

bool verify_data_is_json(const std::string& __json_string)
{
    try {
        auto _ = json::parse(__json_string);
        return true;
    } catch(json::parse_error& e) {
        return false;
    }
}

InMessageWrapper parse_incoming_json(const std::string& __json_string)
{
    json parsed = json::parse(__json_string);
    std::cout << parsed << std::endl;
    InMessageWrapper out = {
        .type = InMessageTypes::GETUPDATE,
        .body = &parsed
    };
    return out;
}

// ==================================================
// OUTGOING MESSAGES
// ==================================================

#endif