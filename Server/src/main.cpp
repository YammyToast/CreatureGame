

#include <unistd.h>
#include <stdio.h>
#include <thread>
#include <cstdio>
#include <signal.h>
#include <atomic>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

#include "net.cpp"
#include "messages.cpp"
#include "shared.h"

std::atomic<bool> interrupt_flag(false);

void sig_interrupt(int __signum)
{
    interrupt_flag = true;
    shared_struct.logger->info("Exiting Gracefully");
    shared_struct.logger->flush();
    exit(0);
}

void worker_thread()
{
    while (true)
    {
        std::string message = shared_struct.message_queue.pop();
        shared_struct.logger->debug("Processing message: {}", message.data());
        if (verify_data_is_json(message) == false)
        {
            shared_struct.logger->warn("Non JSON message received: {}", message.data());
            return;
        }
        InMessageWrapper qualified_message = parse_incoming_json(message);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main(int argc, char **argv)
{
    signal(SIGINT, sig_interrupt);

    // Getopt Vars
    int index;
    int c;
    // Arg Vars
    bool debug_flag = false;
    std::string savefile = "player";
    int num_worker_threads = 1;
    int port = 8080;

    opterr = 0;

    while ((c = getopt(argc, argv, "ds:c:p:")) != -1)
    {
        switch (c)
        {
        case 'd':
            debug_flag = true;
            break;
        case 's':
            savefile.assign(optarg);
            break;
        case 'c':
            num_worker_threads = std::stoi(optarg);
            break;
        case 'p':
            port = std::stoi(optarg);
            break;
        }
    }

    // Logger
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/session-log.log", false);

    spdlog::logger logger("session_logger", {console_sink, file_sink});
    shared_struct.logger = std::make_shared<spdlog::logger>(logger);

    if (debug_flag == true)
    {
        shared_struct.logger->set_level(spdlog::level::debug);
    }
    shared_struct.logger->set_pattern("[%H:%M:%S] [%^%l%$] %v");
    shared_struct.logger->debug("Debug logging on");

    // Write unread args
    for (index = optind; index < argc; index++)
        shared_struct.logger->warn("Non-option argument: {}", argv[index]);

    // Initialize worker thread(s)
    std::vector<std::thread> workers;
    for (int i = 0; i < num_worker_threads; i++)
    {
        workers.emplace_back(worker_thread);
    }

    // Initialize server socket
    int server_fd = create_socket(port);

    // Lifetime Loop
    while (!interrupt_flag)
    {
        int client_socket = accept_client(server_fd);
        // if client is valid
        if (client_socket >= 0)
        {
            std::thread client_thread(handle_client, client_socket);
            client_thread.detach(); // Lose Handler? Dispatches thread to keep running separately from the current thread.
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    close(server_fd);
    return 0;
}
