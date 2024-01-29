#pragma once
#include <qobject.h>
#include <QObject>
#include <QDebug>
#include <QSerialPort>
#include <QThread>
#include <QMutex>
#include <QQueue>
#include <QTimer>
#include "myGraph.h"
#include "tools.h"

const quint32 BUFF_MAXSIZE = 1024;//若该值较小而下位机发送数据量大将造成数据堆积，导致即使下位机停止发送，程序仍需一定时间来处理完系统缓冲区的数据
const quint32 RECIVE_TIMER_GAP = 100;//ms
class MySerial :public QObject
{
	Q_OBJECT
public:
	MySerial(QQueue<quint8> *buf, QObject *parent = nullptr);
	~MySerial();
signals:
public slots:
	//接收数据放入队列
	bool recive();
public:
	QByteArray data;
	QTimer *recive_timer;
	QSerialPort *m_port;
	QQueue<quint8> *buf;
};

