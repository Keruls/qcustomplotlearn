#include "mySerial.h"

MySerial::MySerial(QQueue<quint8> *buf, QObject *parent) :buf(buf), QObject(parent)
{
	m_port = new QSerialPort(this);
	recive_timer = new QTimer(this);
	//修改为参数化-----------------------------------------------
	m_port->setBaudRate(QSerialPort::Baud9600);//波特率_9600
	m_port->setParity(QSerialPort::NoParity);//校验位_n
	m_port->setDataBits(QSerialPort::Data8);//数据位_8
	m_port->setStopBits(QSerialPort::OneStop);//停止位_1
	m_port->setPortName("COM2");
	//------------------------------------------------------
	if (m_port->open(QIODevice::ReadWrite)) {
		qDebug() << "port opened.";
		connect(recive_timer, &QTimer::timeout, this, &MySerial::recive);
		recive_timer->start(RECIVE_TIMER_GAP);
		//connect(m_port, &QSerialPort::readyRead, this, &MySerial::recive);
	}
	else {
		qDebug() << "port opened fail.";
	}
}

MySerial::~MySerial()
{
	m_port->close();
	delete m_port;
	m_port = nullptr;
	delete recive_timer, buf;
}

//起始位、数据位、结束位
bool MySerial::recive()
{
	recive_timer->stop();//防止频繁触发槽函数导致堆积影响性能
	//tools::SharedResourceLocker::mutex.lock();
	data = m_port->read(BUFF_MAXSIZE - buf->size());//每次读取的字节数不会超过BUFF_MAXSIZE
	//data = m_port->readAll();
	for (quint8 i : data) {
		buf->enqueue(i);
	}
	//tools::SharedResourceLocker::mutex.unlock();
	recive_timer->start(RECIVE_TIMER_GAP);
	return 1;
}