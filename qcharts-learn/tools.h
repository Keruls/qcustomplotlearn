#pragma once
#include <QMutex>
namespace tools {
	//在[min，max]获取随机数
	double getRandomNum(double min = 0, double max = 1);
	//return second, or msecond(getCurrentTime()/1000)
	double getCurrentTime();

	//共享锁
	class SharedResourceLocker {
	public:
		static QMutex mutex;
	};

}
