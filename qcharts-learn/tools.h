#pragma once
#include <QMutex>
#include <QFileInfo>
#include <QDir>
namespace tools {
	//在[min，max]获取随机数
	double getRandomNum(double min = 0, double max = 1);
	//return second, or msecond(getCurrentTime()/1000)
	double getCurrentTime();
	// 格式化返回当前时间
	// yyyy-MM-dd HH-mm-ss
	QString getCurrentDate(QString format);
	//创建多级目录
	bool mutilDirPath(QString path);
	//共享锁
	class SharedResourceLocker {
	public:
		static QMutex mutex;
	};

}
