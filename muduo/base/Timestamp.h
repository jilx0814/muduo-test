/**
 * Timestamp.h 时间戳
 */

#ifndef MUDUO_BASE_TIMESTAMP_H
#define MUDUO_BASE_TIMESTAMP_H

#include "muduo/base/copyable.h"
#include "muduo/base/Types.h"

#include <boost/operators.hpp>

namespace muduo
{
///
/// 以UTC为单位的时间戳，以微秒为单位。
///
/// 此类是不可变的。
/// 建议通过值传递它，因为它是在x64上的寄存器中传递的。
///
class Timestamp : public muduo::copyable,
				  public boost::equality_comparable<Timestamp>,
				  public boost::less_than_comparable<Timestamp>
{
public:
	/// 
	/// 构造一个无效的时间戳。 
	///
	Timestamp()
		: microSecondsSinceEpoch_(0)
	{}
	
	///
	/// 在特定时间构造时间戳记
	///
	/// @param microSecondsSinceEpoch
	explicit Timestamp(int64_t microSecondsSinceEpochArg)
		: microSecondsSinceEpoch_(microSecondsSinceEpochArg)
	{}
	
	void swap(Timestamp& that)
	{
		std::swap(microSecondsSinceEpoch_, that.microSecondsSinceEpoch_);
	}
	
	// default copy/assignment/dtor are Okay
	
	string toString() const;
	string toFormattedString(bool showMicroseconds = true) const;
	
	bool valid() const { return microSecondsSinceEpoch_ > 0; }
	
	// for internal usage
	int64_t microSecondsSinceEpoch() const { return microSecondsSinceEpoch_; }
	time_t secondsSinceEpoch() const { return static_cast<time_t>(microSecondsSinceEpoch_ / kMicroSecondsPerSecond); }
	
	///
	/// Get time of now
	///
	static Timestamp now();
	static Timestamp invalid()
	{
		return Timestamp();
	}
	
	static Timestamp fromUnixTime(time_t t)
	{
		return fromUnixTime(t, 0);
	}
	
	static Timestamp fromUnixTime(time_t t, int microseconds)
	{
		return Timestamp(static_cast<int64_t>(t) * kMicroSecondsPerSecond + microseconds);
	}
	
	static const int kMicroSecondsPerSecond = 1000*1000;
private:
	int64_t microSecondsSinceEpoch_;
};

inline bool operator<(Timestamp lhs, Timestamp rhs)
{
	return lhs.microSecondsSinceEpoch() < rhs.microSecondsSinceEpoch();
}

inline bool operator==(Timestamp lhs, Timestamp rhs)
{
	return lhs.microSecondsSinceEpoch() == rhs.microSecondsSinceEpoch();
}


///
/// 获得两个时间戳的时差，以秒为单位。
///
/// @param high, low
/// @return (high-low) in seconds
/// @c double具有52位精度，足够用于一微秒
/// 未来100年的分辨率。
inline double timeDifference(Timestamp high, Timestamp low)
{
	int64_t diff = high.microSecondsSinceEpoch() - low.microSecondsSinceEpoch();
	return static_cast<double>(diff) / Timestamp::kMicroSecondsPerSecond;
}

///
/// Add @c 给定时间戳记的秒数。
///
/// @return timestamp+seconds as Timestamp
///
inline Timestamp addTime(Timestamp timestamp, double seconds)
{
	int64_t delta = static_cast<int64_t>(seconds * Timestamp::kMicroSecondsPerSecond);
	return Timestamp(timestamp.microSecondsSinceEpoch() + delta);
}

} // namespace muduo

#endif // MUDUO_BASE_TIMESTAMP_H
