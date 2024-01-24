#pragma once
#include <QObject>
#include <QDebug>
#include"qcustomplot.h"
class MyTracer : public QObject {
	Q_OBJECT
public:
	enum TextPositionStyle { fixed, follow };
	MyTracer(QCustomPlot* plot, MyTracer::TextPositionStyle style = MyTracer::TextPositionStyle::follow);
	~MyTracer();
	void initTracer();
	void initTracerText(MyTracer::TextPositionStyle style);
	bool updatePosition(QMouseEvent* e);
	void setPositionStyle(MyTracer::TextPositionStyle position_style);
	void limitVisibleArea(double mouse_position_x, double mouse_position_y);
	void setParentGraph(QCPGraph* g);
	void searchNearly(QSharedPointer<QCPGraphDataContainer> parent_graph_data, double x, double y);
	//double searchNearly(double x);
public:
	QCPItemTracer* m_tracer = nullptr;
	QCPItemText* m_tracer_text = nullptr;
private:
	QCustomPlot* parent_plot = nullptr;
	const QCPGraph* parent_graph = nullptr;
};