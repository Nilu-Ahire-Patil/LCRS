#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "lcrs/logger.h"

std::string Logger::mLoggerName;
std::string Logger::mLogFilePath;

spdlog::level::level_enum Logger::mLogLevel;

int Logger::mMaxFileSize;
int Logger::mMaxFileCount;

std::shared_ptr<spdlog::logger> Logger::mLogger;

int Logger::init(const std::string &loggerName,
                             const std::string &logFilePath,
                             const spdlog::level::level_enum logLevel,
                             const int maxFileSize,
                             const int maxFileCount)
{
    mLoggerName = loggerName;
    mLogFilePath = logFilePath.empty() ? std::filesystem::temp_directory_path().string() + "/lcrs/Log/" + mLoggerName + ".log" : logFilePath;
    mLogLevel = logLevel;
    mMaxFileSize = maxFileSize;
    mMaxFileCount = maxFileCount;

    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_level(mLogLevel);
    console_sink->set_pattern("%^[%L]%$ %v");

    auto rotating_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(mLogFilePath, mMaxFileSize, mMaxFileCount);
    rotating_sink->set_level(spdlog::level::trace);

    std::vector<spdlog::sink_ptr> sinks{console_sink, rotating_sink};
    mLogger = std::make_shared<spdlog::logger>(mLoggerName, sinks.begin(), sinks.end());

    mLogger->set_level(mLogLevel);

    spdlog::set_default_logger(mLogger);

    SPDLOG_INFO("Writing logs in {}", mLogFilePath);
    SPDLOG_INFO("{} Application started", mLoggerName);

    return EXIT_SUCCESS;
}

int Logger::setLevel(const spdlog::level::level_enum logLevel)
{
	mLogLevel = logLevel;
	mLogger->set_level(mLogLevel);
    return EXIT_FAILURE;
}