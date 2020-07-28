// 类似于对象的字符串，指向另一个内存。
// 有助于提供一个界面，让客户轻松
// 传递“ const char *”或“ string”。
//
// 我希望C ++文字自动为“字符串”类型。

#ifndef MUDUO_BASE_STRINGPIECE_H
#define MUDUO_BASE_STRINGPIECE_H

#include <string.h>
#include <iosfwd>  // 用于ostream前向声明

#include "muduo/base/Types.h"

namespace muduo
{
// 用于将C样式的字符串参数传递给函数。
class StringArg // copyable
{
public:
	StringArg(const char* str)
		: str_(str)
	{}
	
	StringArg(const string& str)
		: str_(str.c_str())
	{}
	
	const char* c_str() const { return str_; }
	
private:
	const char* str_;
};

class StringPiece 
{
private:
	const char* ptr_;
	int length_;
	
public:
	//我们提供非明确的单例构造函数，以便用户可以通过
	//在“ const char *”或“ string”中，无论“ StringPiece”在哪里
	//预期。
	StringPiece()
		: ptr_(NULL), length_(0) {}
	StringPiece(const char* str)
		: ptr_(str), length_(static_cast<int>(strlen(ptr_))) {}
	StringPiece(const unsigned char* str)
		: ptr_(reinterpret_cast<const char*>(str)),
		  length_(static_cast<int>(strlen(ptr_))) {}
	StringPiece(const string& str)
		: ptr_(str.data()), length_(static_cast<int>(str.size())) {}
	StringPiece(const char* offset, int len)
		: ptr_(offset), length_(len) {}
		
	// data（）可能会返回一个指向带有嵌入式NUL的缓冲区的指针，并且
	// 返回的缓冲区可以终止也可以不为null。 因此它是
	// 通常是将data（）传递给需要NUL的例程的错误
	// 终止的字符串。 如果确实需要，请使用“ as_string（）。c_str（）”
	// 这个。 或者更好的是，更改您的例程，使其不依赖NUL
	// 终止。
	const char* data() const { return ptr_; }
	int size() const { return length_; }
	bool empty() const { return length_ == 0; }
	const char* begin() const { return ptr_; }
	const char* end() const { return ptr_ + length_; }
	
	void clear() { ptr_ = NULL; length_ = 0; }
	void set(const char* buffer, int len) { ptr_ = buffer; length_ = len; }
	void set(const char* str) 
	{
		ptr_ = str;
		length_ = static_cast<int>(strlen(str));
	}
	
	void set(const void* buffer, int len)
	{
		ptr_ = reinterpret_cast<const char*>(buffer);
		length_ = len;
	}
	
	char operator[](int i) const { return ptr_[i]; }
	
	void remove_prefix(int n) 
	{
		ptr_ += n;
		length_ -= n;
	}
};

} // muduo

#endif // MUDUO_BASE_STRINGPIECE_H