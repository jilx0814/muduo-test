/**
 * Logging.cc 日志类的实现
 */
 
#include "muduo/base/Logging.h"

#include "muduo/base/CurrentThread.h"
#include "muduo/base/Timestamp.h"
#include "muduo/base/TimeZone.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>

#include <sstream>

namespace muduo
{
/*
class LoggerImpl
{
 public:
  typedef Logger::LogLevel LogLevel;
  LoggerImpl(LogLevel level, int old_errno, const char* file, int line);
  void finish();

  Timestamp time_;
  LogStream stream_;
  LogLevel level_;
  int line_;
  const char* fullname_;
  const char* basename_;
};
*/
}