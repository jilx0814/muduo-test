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
	
	void remove_prefix(int n) // 删除字首
	{
		ptr_ += n;
		length_ -= n;
	}
	
	void remove_suffix(int n) // 删除后缀
	{
		length_ -= n;
	}
	
	bool operator==(const StringPiece& x) const
	{
		return ((length_ == x.length_) &&
				(memcmp(ptr_, x.ptr_, length_) == 0));
	}
	bool operator!=(const StringPiece& x) const
	{
		return !(*this == x);
	}
	
#define STRINGPIECE_BINARY_PREDICATE(cmp,auxcmp)                             \
  bool operator cmp (const StringPiece& x) const {                           \
    int r = memcmp(ptr_, x.ptr_, length_ < x.length_ ? length_ : x.length_); \
    return ((r auxcmp 0) || ((r == 0) && (length_ cmp x.length_)));          \
  }
  STRINGPIECE_BINARY_PREDICATE(<,  <);
  STRINGPIECE_BINARY_PREDICATE(<=, <);
  STRINGPIECE_BINARY_PREDICATE(>=, >);
  STRINGPIECE_BINARY_PREDICATE(>,  >);
#undef STRINGPIECE_BINARY_PREDICATE

	int compare(const StringPiece& x) const
	{
		int r = memcmp(ptr_, x.ptr_, length_ < x.length_ ? length_ : x.length_);
		if (r == 0)
		{
			if (length_ < x.length_) r = -1;
			else if (length_ > x.length_) r = +1;
		}
		return r;
	}
	
	string as_string() const
	{
		return string(data(), size());
	}
	
	void CopyToString(string* target) const
	{
		target->assign(ptr_, length_);
	}
	
	//“ this”是否以“ x”开头
	bool starts_with(const StringPiece& x) const
	{
		return ((length_ >= x.length_) && (memcmp(ptr_, x.ptr_, x.length_) == 0));
	}
};

} // namespace muduo


// ------------------------------------------------ ------------------
//用于创建使用StringPiece的STL容器的函数
//请记住，StringPiece的生命周期最好小于
//基础字符串或char *的字符串。 如果不是，那你
//无法将StringPiece安全地存储到STL容器中
// ------------------------------------------------ ------------------

#ifdef HAVE_TYPE_TRAITS
// This makes vector<StringPiece> really fast for some STL implementations
template<> struct __type_traits<muduo::StringPiece> {
  typedef __true_type    has_trivial_default_constructor;
  typedef __true_type    has_trivial_copy_constructor;
  typedef __true_type    has_trivial_assignment_operator;
  typedef __true_type    has_trivial_destructor;
  typedef __true_type    is_POD_type;
};
#endif

// allow StringPiece to be logged
std::ostream& operator<<(std::ostream& o, const muduo::StringPiece& piece);

#endif // MUDUO_BASE_STRINGPIECE_H