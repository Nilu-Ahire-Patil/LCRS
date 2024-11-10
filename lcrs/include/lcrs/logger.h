#ifndef LOGGER_H
#define LOGGER_H

#include <filesystem>

#include <spdlog/spdlog.h>

#define lcrsLog(LogLevel, ...) SPDLOG_##LogLevel(__VA_ARGS__)

#ifdef _WIN32
template class std::shared_ptr<spdlog::logger>;
#endif

class Logger {
public:
	static int init(const std::string &loggerName,
						  const std::string &logFilePath = "",	// on empty it stores log in syatem temperory directory
						  const spdlog::level::level_enum logLevel = spdlog::level::info,
						  const int maxFileSize = 1024 * 1024 * 1024,
						  const int maxFileCount = 3
	);

	static int setLevel(const spdlog::level::level_enum logLevel);

private:
	Logger();

private:
	static std::string mLoggerName;
	static std::string mLogFilePath;

	static spdlog::level::level_enum mLogLevel;

	static int mMaxFileSize;
	static int mMaxFileCount;

	static std::shared_ptr<spdlog::logger> mLogger;
};

#endif