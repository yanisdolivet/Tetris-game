/*
** EPITECH PROJECT, 2025
** Tetris
** File description:
** Logs.hpp
*/

#ifndef LOGS_HPP_
#define LOGS_HPP_

#include <memory>
#include <optional>
#include <spdlog/async.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
#include <string>

#define LOGGER_NAME "Tetris Logger"
#define QUEUE_SIZE 8192
#define NB_THREAD 1

#define DISPLAY_TEMPLATE "[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v"

class Logs
{
    public:
        Logs() = delete;

        static void init(std::optional<std::string>(log_file_name));
        static void shutdown();

        static std::shared_ptr<spdlog::logger>& get();

    protected:
    private:
        static std::shared_ptr<spdlog::logger> _logger;
};

// Macro for all types of logs
#define LOG_TRACE(...) Logs::get()->trace(__VA_ARGS__)
#define LOG_INFO(...) Logs::get()->info(__VA_ARGS__)
#define LOG_WARN(...) Logs::get()->warn(__VA_ARGS__)
#define LOG_ERROR(...) Logs::get()->error(__VA_ARGS__)
#define LOG_FATAL(...) Logs::get()->critical(__VA_ARGS__)

#endif /* !LOGS_HPP_ */
