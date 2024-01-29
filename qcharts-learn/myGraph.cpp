#include "myGraph.h"


MyGraph::MyGraph(QCustomPlot *parent_plot, QString graph_name, QObject *parent) :
	parent_plot(parent_plot), m_name(graph_name), QObject(parent)
{
	m_graph = parent_plot->addGraph();
	m_graph->setName(graph_name);
	m_graph->setPen(QPen(QColor(255, 0, 0)));
	m_graph->setLineStyle(QCPGraph::lsLine);
	//set data point style
	QCPScatterStyle m_scatter;
	m_scatter.setShape(QCPScatterStyle::ssCircle);
	m_scatter.setPen(QPen(QColor(255, 0, 0)));
	m_scatter.setBrush(QColor(255, 255, 0));
	m_scatter.setSize(5);//size of shape
	//m_graph->setScatterStyle(m_scatter);//set to anyone graph to use 
}

MyGraph::~MyGraph()
{
}

void MyGraph::showAllPoint(bool b)
{
	m_graph->rescaleAxes(b);
}

void MyGraph::setBrush(int r, int g, int b, int a)
{
	m_graph->setBrush(QBrush(QColor(r, g, b, a)));
}
void MyGraph::setPen(int r, int g, int b, int a)
{
	m_graph->setPen(QPen(QColor(r, g, b, a)));
}

void MyGraph::setLayer(QString layer)
{
	m_graph->setLayer(layer);
}

void MyGraph::saveGraphData2File(QString path) const
{
	QSharedPointer<QCPGraphDataContainer> graph_data = m_graph->data();
	QString file_name = tools::getCurrentDate(QString("yyyy-mm-dd_hh-mm-ss"));
	//保存为CSV
	if (1) {
		path += (file_name + ".csv");
		qDebug() << path;
		QFile f(path);
		if (f.open(QIODevice::WriteOnly | QIODevice::Text)) {
			QTextStream w(&f);
			w << "X,Y\n";
			for (int i = 0; i < m_graph->dataCount(); ++i) {
				w << graph_data->at(i)->key << "," << graph_data->at(i)->value << "\n";
			}
			f.close();
			qDebug() << "graph data save complete.";
		}
		else {
			qDebug() << "file open fail.";
			qDebug() << f.errorString();
		}
	}
	//保存为JSON
	if (0) {
		path += (file_name + ".json");
		qDebug() << path;
		QFile f(path);
		if (f.open(QIODevice::WriteOnly | QIODevice::Text)) {
			QTextStream w(&f);
			//w.AlignLeft;w.setFieldWidth(15);
			w << "{\n";
			w << "\"X\" : \"Y\",\n";
			for (int i = 0; i < m_graph->dataCount(); ++i) {
				w << "    \"" << graph_data->at(i)->key << "\" : " << graph_data->at(i)->value << ",\n";
			}
			w << "\"key\" : \"value\"\n";
			w << "}";
			f.close();
			qDebug() << "graph data save complete.";
		}
		else {
			qDebug() << "file open fail.";
			qDebug() << f.errorString();
		}

	}
}

void MyGraph::loadFileData2Graph(QString path) {
	QFile f(path);
	QString line;
	QStringList line_data;
	if (f.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QTextStream w(&f);
		int num = 0;
		w.readLine();//将第一行(列名)排除
		while (!w.atEnd())
		{
			line = w.readLine();
			line_data = line.split(',');
			m_graph->addData(line_data.at(0).toDouble(), line_data.at(1).toDouble());
			++num;
		}
		f.close();
		qDebug() << "读取完成，数据数量:" << num;
	}
	else {
		qDebug() << "file open fail.";
		qDebug() << f.errorString();
	}
}


TaskWoker::TaskWoker(QObject *parent) :QObject(parent)
{
}

TaskWoker::~TaskWoker()
{
}
void TaskWoker::run(QQueue<quint8> *buf, MyGraph *mg) {
	//qDebug() << "buff_size:" << buf->size();
	//tools::SharedResourceLocker::mutex.lock();
	while (buf->size() >= 3)
	{
		quint8 head_byte = buf->dequeue();
		quint8 data_byte = buf->dequeue();
		quint8 tail_byte = buf->dequeue();
		if (head_byte == 0x66) {
			if (tail_byte == 0xee) {
				mg->m_graph->addData(mg->m_graph->dataCount(), static_cast<double>(data_byte));
			}
			else
			{
				qDebug() << "tail_byte != 0xee";
				break;
			}
		}
		else
		{
			qDebug() << "head_byte != 0x66";
		}
	}
	//tools::SharedResourceLocker::mutex.unlock();
}

