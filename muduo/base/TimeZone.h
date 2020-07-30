/**
 * TimeZone.h 时间域
 */
 
#ifndef MUDUO_BASE_TIMEZONE_H
#define MUDUO_BASE_TIMEZONE_H

#include "muduo/base/copyable.h"
#include <memory>
#include <time.h>
 
namespace muduo
{

// TimeZone for 1970-2030
class TimeZone : public muduo::copyable
{
public:
	explicit TimeZone(const char* zonefile);
	TimeZone(int eastOfUtc, const char* tzname); // 固定时区
	TimeZone() = default; // 无效的时区
	

	// 默认复制ctor / assignment / dtor可以。
	
	bool valid() const 
	{
		// 'explicit operator bool() const' in C++11
		return static_cast<bool>(data_);
	}
	
	struct tm toLocalTime(time_t secondsSinceEpoch) const;
	time_t fromLocalTime(const struct tm&) const;
	
	// gmtime(3)
	static struct tm toUtcTime(time_t secondsSinceEpoch, bool yday = false);
	// timegm(3)
	static time_t fromUtcTime(const struct tm&);
	// year in [1900..2500], month in [1..12], day in [1..31]
	static time_t fromUtcTime(int year, int month, int day,
								int hour, int minute, int seconds);
								
	struct Data;
	
	std::shared_ptr<Data> data_;
};

} // namespace muduo

#endif // MUDUO_BASE_TIMEZONE_H