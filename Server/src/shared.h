#ifndef _SHARED_H_
#define _SHARED_H_

#include <mutex>
#include <vector>

#include "message_queue.h"

std::mutex shared_lock;

struct
{   
    std::vector<int> client_fds;
    MessageQueue<std::string> message_queue;
    std::shared_ptr<spdlog::logger> logger;
} shared_struct;

#endif