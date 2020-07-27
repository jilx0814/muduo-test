/**
 * Logging.h 日志类的定义，日志等级
 */

#ifndef MUDUO_BASE_LOGGING_H
#define MUDUO_BASE_LOGGING_H

#include "muduo/base/LogStream.h"
#include "muduo/base/Timestamp.h"

namespace nuduo
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
	}

	// 源文件基本名称的编译时间计算
	class SourceFile
	{
	public:
		template<int N>
		SourceFile(const char (&arr)[N])
			: data_(arr),
			  size_(N-1)
		{
			const char* slash = strrchr(data_, '/') // 内置函数
			if (slash)
			{
				data_ = slash + 1;
				size -= static_cast<int>(data_ - arr);
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
			size = static_cast<int>(strlen(data_));
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
	
	

};

} // namespace muduo

#endif / MUDUO_BASE_LOGGING_H