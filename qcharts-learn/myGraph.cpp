#include "myGraph.h"


MyGraph::MyGraph(QCustomPlot *parent_plot, QString graph_name, QObject *parent) :
	parent_plot(parent_plot), m_name(graph_name),QObject(parent)
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


TaskWoker::TaskWoker(QObject *parent):QObject(parent)
{
}

TaskWoker::~TaskWoker()
{
}
void TaskWoker::run(QQueue<quint8> *buf , MyGraph *mg) {
	qDebug() << "buff_size:" << buf->size();
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
			}
		}
		else
		{
			qDebug() << "head_byte != 0x66";
		}
	}
	//tools::SharedResourceLocker::mutex.unlock();
}

