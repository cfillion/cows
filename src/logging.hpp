#ifndef LOGGING_H
#define LOGGING_H

#include "logger.hpp"

#define LOG_MODULE(name) \
  static const std::string _LOG_MODULE_NAME = name;

#define LOG_HERE(level, message) \
  Logger::instance()->log(level, _LOG_MODULE_NAME, message);

#define LOG_DEBUG(msg) \
  LOG_HERE(Logger::DEBUG, msg);

#define LOG_INFO(msg) \
  LOG_HERE(Logger::INFO, msg);

#define LOG_WARNING(msg) \
  LOG_HERE(Logger::WARNING, msg);

#define LOG_ERROR(msg) \
  LOG_HERE(Logger::ERROR, msg);

#define LOG_FATAL(msg) \
  LOG_HERE(Logger::FATAL, msg);

#endif
