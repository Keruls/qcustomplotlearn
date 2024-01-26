#include "myTracer.h"

MyTracer::MyTracer(QCustomPlot *plot, MyTracer::TextPositionStyle style) :
	parent_plot(plot)
{
	parent_graph = parent_plot->graph(0);
	m_tracer = new QCPItemTracer(parent_plot);
	m_tracer_text = new QCPItemText(parent_plot);
	initTracer();
	initTracerText(style);
}

MyTracer::~MyTracer()
{
}

bool MyTracer::updatePosition(QMouseEvent *e)
{
	//获取鼠标当前位置映射在图表上的坐标
	double x = parent_plot->xAxis->pixelToCoord(e->position().x());
	double y = parent_plot->yAxis->pixelToCoord(e->position().y());
	limitVisibleArea(x, y);
	searchNearly(parent_graph->data(), x, y);
	return 1;
}

//init
void MyTracer::initTracer()
{
	m_tracer->setVisible(0);
	m_tracer->setStyle(QCPItemTracer::TracerStyle::tsCrosshair);
	m_tracer->setBrush(QColor(255, 0, 0));
	m_tracer->setPen(QPen(QColor(255, 0, 0)));
}
//init
void MyTracer::initTracerText(MyTracer::TextPositionStyle style)
{
	m_tracer_text->setVisible(0);
	m_tracer_text->setLayer("overlay");
	setPositionStyle(style);
	m_tracer_text->setText("x=0\ny=0");
}
//set tracer text is follow tracer or not
void MyTracer::setPositionStyle(MyTracer::TextPositionStyle position_style)
{
	m_tracer_text->setPadding(QMargins(10, 2, 10, 5));//l,t,r,b
	m_tracer_text->setPen(QPen(QColor(50, 50, 50)));
	m_tracer_text->setBrush(QBrush(QColor(255, 255, 255)));
	m_tracer_text->setPositionAlignment(Qt::AlignLeft | Qt::AlignTop);
	m_tracer_text->setTextAlignment(Qt::AlignLeft | Qt::AlignTop);
	switch (position_style)
	{
	case follow:
		m_tracer_text->position->setParentAnchor(m_tracer->position);//tracer text move with tracer
		break;
	case fixed:
		m_tracer_text->position->setType(QCPItemPosition::ptAxisRectRatio);//tracer text fixed at Axis-Rect
		m_tracer_text->position->setCoords(0.01, 0.01);//start point at Axis left-top, param value from 0 to 1)
		break;
	}
}

//when mouse out of chart ,hidden tracer
void MyTracer::limitVisibleArea(double mouse_position_x, double mouse_position_y)
{
	if (mouse_position_x > parent_plot->xAxis->range().upper ||
		mouse_position_x < parent_plot->xAxis->range().lower ||
		mouse_position_y > parent_plot->yAxis->range().upper ||
		mouse_position_y < parent_plot->yAxis->range().lower)
	{
		m_tracer->setVisible(0);
		m_tracer_text->setVisible(0);
	}
	else
	{
		m_tracer->setVisible(1);
		m_tracer_text->setVisible(1);
	}
}

void MyTracer::setParentGraph(QCPGraph *g)
{
	parent_graph = g;
}

void MyTracer::searchNearly(QSharedPointer<QCPGraphDataContainer> parent_graph_data, double x, double y)
{
	if (parent_graph_data->size() != 0)
	{
		double point_x, point_y, point_next_x, point_next_y, point_mid;
		auto point = parent_graph_data->findBegin(x);//返回离鼠标当前位置最近的一个点，第二个参数true为左边最近，false为右边最近
		//auto point = searchNearer(x);
		auto point_next = point + 1;
		if (point == parent_graph_data->constEnd() - 1) //avoid out of index
		{
			point_next = point;
		}
		point_next_x = point_next->key;
		point_next_y = point_next->value;
		point_x = point->key;
		point_y = point->value;
		point_mid = (point_next_x + point_x) / 2;
		if (x <= point_mid)
		{
			m_tracer->position->setCoords(point_x, point_y);//reset tracer's position
			m_tracer_text->setText(QString("x=%1\ny=%2").arg(point_x).arg(point_y));
			//do something...
		}
		else if (x > point_mid)
		{
			m_tracer->position->setCoords(point_next_x, point_next_y);
			m_tracer_text->setText(QString("x=%1\ny=%2").arg(point_next_x).arg(point_next_y));
			//do something...
		}
	}
}

//double MyTracer::searchNearly(double x)
//{
//	auto high = parent_graph_data->end()->key;
//	auto low = parent_graph_data->begin()->key;
//	while (low <= high) {
//		auto mid = (low + high) / 2;
//		if (x < mid) {
//			high = mid;
//		}
//		else if (x > mid) {
//			low = mid;
//		}
//		return mid;
//	}
//}


