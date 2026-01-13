/*
** EPITECH PROJECT, 2025
** Tetris
** File description:
** Logs.cpp
*/

#include "Logs.hpp"

#include <memory>
#include <optional>
#include <spdlog/logger.h>
#include <spdlog/spdlog.h>

// Define static member
std::shared_ptr<spdlog::logger> Logs::_logger;

/**
 * @brief Initialize the logging system.
 *
 * @param log_file_name The name of the log file.
 */
void Logs::init(std::optional<std::string>(log_file_name))
{
    // Check whether logger have already been initialised
    if (spdlog::get(LOGGER_NAME) != nullptr) {
        return;
    }

    // Init thread pool for asynchrone
    spdlog::init_thread_pool(QUEUE_SIZE, NB_THREAD);

    // Create sink for display in console
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_level(spdlog::level::trace);

    // Stock sink into a vector console_sink
    std::vector<spdlog::sink_ptr> sinks{console_sink};

    // Create sink for saving logs into a file if set
    if (log_file_name.has_value()) {
        auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(log_file_name.value(), true);
        file_sink->set_level(spdlog::level::trace);
        sinks.push_back(file_sink);
    }

    // Create asynchrone logger
    _logger = std::make_shared<spdlog::async_logger>(LOGGER_NAME, sinks.begin(), sinks.end(), spdlog::thread_pool(),
                                                     spdlog::async_overflow_policy::block);

    // Create a global configuration
    spdlog::register_logger(_logger);
    spdlog::set_pattern(DISPLAY_TEMPLATE);
    spdlog::set_level(spdlog::level::trace);

    // Automatic flush in case of errors
    _logger->flush_on(spdlog::level::err);
}

/**
 * @brief Shutdown the logging system.
 *
 */
void Logs::shutdown()
{
    spdlog::shutdown();
    _logger.reset();
}

/**
 * @brief Get the logger instance.
 *
 * @return std::shared_ptr<spdlog::logger>& The logger instance.
 */
std::shared_ptr<spdlog::logger>& Logs::get()
{
    return _logger;
}
