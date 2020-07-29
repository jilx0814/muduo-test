/**
 * Logging.h 日志类的定义，日志等级
 */

#ifndef MUDUO_BASE_LOGGING_H
#define MUDUO_BASE_LOGGING_H

#include "muduo/base/LogStream.h"
#include "muduo/base/Timestamp.h"

namespace muduo
{
class TimeZone;

class Logger
{
public:
	enum LogLevel
	{
		TRACE, // 级别最低
		DEBUG, // 程序调式级别
		INFO, // 程序运行输出必要的信息
		WARN, // 可能会存在的错误警告
		ERROR, // 运行发送错误
		FATAL, // 程序运行发生严重错误导致程序退出
		NUM_LOG_LEVELS, // 日志级别总数
	};

	// 源文件基本名称的编译时间计算
	class SourceFile
	{
	public:
		template<int N>
		SourceFile(const char (&arr)[N])
			: data_(arr),
			  size_(N-1)
		{
			const char* slash = strrchr(data_, '/'); // 内置函数
			if (slash)
			{
				data_ = slash + 1;
				size_ -= static_cast<int>(data_ - arr);
			} // "https//www.baidu.com" 变成 "www.baidu.com"
		}
		
		explicit SourceFile(const char* filename)
			: data_(filename)
		{
			const char* slash = strrchr(filename, '/');
			if (slash)
			{
				data_ = slash + 1;
			}
			size_ = static_cast<int>(strlen(data_));
		}
		
		const char* data_;
		int size_;
	};
	
	// 构造函数
	Logger(SourceFile file, int line);
	Logger(SourceFile file, int line, LogLevel level);
	Logger(SourceFile file, int line, LogLevel level, const char* func);
	Logger(SourceFile file, int line, bool toAbort);
	~Logger();
	
	LogStream& stream() { return impl_.stream_; }
	
	static LogLevel logLevel();
	static void setLogLevel(LogLevel level);
	
	typedef void (*OutputFunc)(const char* msg, int len);
	typedef void (*FlushFunc)();
	static void setOutput(OutputFunc);
	static void setFlush(FlushFunc);
	static void setTimeZone(const TimeZone& tz);
	
private:
	
	class Impl
	{
	public:
		typedef Logger::LogLevel LogLevel;
		Impl(LogLevel level, int old_errno, const SourceFile& file, int line);
		void formatTime();
		void finish();
		
		Timestamp time_;
		LogStream stream_;
		LogLevel level_;
		int line_;
		SourceFile basename_;
	};
	
	Impl impl_;
};

extern Logger::LogLevel g_logLevel;

inline Logger::LogLevel Logger::logLevel()
{
	return g_logLevel;
}

//
// 注意：请勿写：
//
// if (good)
//   LOG_INFO << "Good news";
// else
//   LOG_WARN << "Bad news";
//
// this expends to
//
// if (good)
//   if (logging_INFO)
//     logInfoStream << "Good news";
//   else
//     logWarnStream << "Bad news";
//

#define LOG_TRACE if (muduo::Logger::logLevel() <= muduo::Logger::TRACE) \
	muduo::Logger(__FILE__, __LINE__, muduo::Logger::TRACE, __func__).stream()
#define LOG_DEBUG if (muduo::Logger::logLevel() <= muduo::Logger::DEBUG) \
	muduo::Logger(__FILE__, __LINE__, muduo::Logger::DEBUG, __func__).stream()
#define LOG_INFO if (muduo::Logger::logLevel() <= muduo::Logger::INFO) \
	muduo::Logger(__FILE__, __LINE__).stream()
#define LOG_WARN muduo::Logger(__FILE__, __LINE__, muduo::Logger::WARN).stream()
#define LOG_ERROR muduo::Logger(__FILE__, __LINE__, muduo::Logger::ERROR).stream()
#define LOG_FATAL muduo::Logger(__FILE__, __LINE__, muduo::Logger::FATAL).stream()
#define LOG_SYSERR muduo::Logger(__FILE__, __LINE__, false).stream()
#define LOG_SYSFATAL muduo::Logger(__FILE__, __LINE__, true).stream()

const char* strerror_tl(int savedErrno);

// Taken from glog/logging.h
//
// 检查输入是否为非NULL。 这在构造函数中非常有用
// 初始化程序列表。

#define CHECK_NOTNULL(val) \
	::muduo::CheckNotNull(__FILE__, __LINE__, "'" #val "' Must be non NULL", (val))

// A small helper for CHECK_NOTNULL().
template <typename T>
T* CheckNotNull(Logger::SourceFile file, int line, const char *names, T* ptr)
{
	if (ptr == NULL)
	{
		Logger(file, line, Logger::FATAL).stream() << names;
	}
	return ptr;
}

} // namespace muduo

#endif // MUDUO_BASE_LOGGING_H