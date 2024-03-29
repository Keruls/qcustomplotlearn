#pragma once
#include "qcustomplot.h"
#include "tools.h"
#include <QObject>
#include <QQueue>
#include <QFile>
#include "mySerial.h"
#include "tools.h"
class MyGraph :public QObject
{
	Q_OBJECT
public:
	MyGraph(QCustomPlot *parent_plot, QString graph_name, QObject *parent = nullptr);
	~MyGraph();
	void showAllPoint(bool b = 0);
	void setBrush(int r = 255, int g = 255, int b = 255, int a = 100);
	void setPen(int r = 255, int g = 255, int b = 255, int a = 100);
	/*"background"，"grid"，"main"，"axes"，"legend"，"overlay"*/
	void setLayer(QString layer);
//public slots:
	//保存曲线点的数据，格式为yyyy-mm-dd hh-mm-ss
	void saveGraphData2File(QString path) const;
	//从文件加载数据到曲线
	void loadFileData2Graph(QString path);
	bool addData(double key, double value);
	quint32 getGraphDataCount();
private:
	const QCustomPlot *parent_plot;
	QString m_name;
	QCPGraph *m_graph;
};

//线程任务类，用于为某一条曲线加载数据
class TaskWoker :public QObject {
	Q_OBJECT
public:
	TaskWoker(QObject *parent = nullptr);
	~TaskWoker();
public slots:
	void run(QQueue<quint8> *data_queue, MyGraph *mg);
};