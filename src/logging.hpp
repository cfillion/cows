#ifndef LOGGING_HPP
#define LOGGING_HPP

#include "logger.hpp"

#define LOG_MODULE(name) static const char *_LOG_MODULE_NAME = name;

#define LOG(level, message) \
  Logger::instance()->log(level, _LOG_MODULE_NAME, message);

#define LOG_DEBUG(msg) LOG(Logger::DEBUG, msg)
#define LOG_INFO(msg) LOG(Logger::INFO, msg)
#define LOG_WARNING(msg) LOG(Logger::WARNING, msg)
#define LOG_ERROR(msg) LOG(Logger::ERROR, msg)
#define LOG_FATAL(msg) LOG(Logger::FATAL, msg)

#endif
