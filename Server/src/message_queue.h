#ifndef _MESSAGE_QUEUE_H_
#define _MESSAGE_QUEUE_H_

#include <mutex>
#include <queue>
#include <iostream>
#include <condition_variable>

template <typename T>
class MessageQueue
{
public:
    void push(const T &__message);
    T pop();

private:
    std::queue<T> queue;
    std::mutex mtx;
    std::condition_variable cond_var;
};

// ====================


template <typename T>
void MessageQueue<T>::push(const T &__message)
{
    std::lock_guard<std::mutex> lock(mtx);
    queue.push(__message);
    cond_var.notify_one();
}

template <typename T>
T MessageQueue<T>::pop()
{
    std::unique_lock<std::mutex> lock(mtx);
    cond_var.wait(lock, [this]
                  { return !queue.empty(); });
    T message = queue.front();
    queue.pop();
    return message;
}


#endif