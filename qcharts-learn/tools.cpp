#include"tools.h"
#include <random>
#include <QDateTime>
namespace tools {
	double getRandomNum(double min, double max) {
		if(0)
		{
			srand(QTime::currentTime().msec() + QTime::currentTime().second() * 10000);//get random seed
			return rand() % 10;
		}
		if(1)
		{
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_real_distribution distrib(min, max);
			return distrib(gen);
		}
	}

	double getCurrentTime() {
		//return QTime(0, 0).msecsTo(QTime::currentTime());//当日凌晨开始到现在现在的秒数
		return static_cast<double>(QDateTime::currentMSecsSinceEpoch());//纪元开始到现在的秒数
	}

	QString getCurrentDate(QString format) {
		return QDateTime::currentDateTime().toString(format);
	}

	bool mutilDirPath(QString path)
	{
		QFileInfo info(path);
		QString abs_path = info.absolutePath();
		if (!QDir(abs_path).exists()) {
			if (!mutilDirPath(abs_path)) {
				return false;
			}
		}
		return QDir().mkdir(path);
	}

	QMutex SharedResourceLocker::mutex;
}