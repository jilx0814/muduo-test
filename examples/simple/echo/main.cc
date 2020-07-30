#include "muduo/base/Logging.h"

#include <unistd.h>


int main(void)
{
	LOG_INFO << "pid = " << getpid();
	return 0;
}