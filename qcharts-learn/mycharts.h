#pragma once
#include <QTimer>
#include <QtWidgets/QWidget>
#include <QtGlobal>
#include <QDebug>
#include <QThread>
#include <QFileDialog>
#include "tools.h"
#include "ui_mycharts.h"
#include "myTracer.h"
#include "myGraph.h"
#include "mySerial.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MyChartsClass; };
QT_END_NAMESPACE

const quint32 REPLOT_TIMER_GAP = 100;
const quint32 HANDLE_TIMER_GAP = 300;
//坐标轴属性
struct AxisStyle {
    QString style;
    QString lable_name;
    bool sub_tick_visible = false;
    bool grid_visible = false;
    int tick_count = 0;
    int sub_tick_count = 0;
    int tick_rotate = 0;
    double min_tick = 0;
    double max_tick = 0;
};
class MyCharts : public QWidget
{
    Q_OBJECT 

public:
    MyCharts(QWidget* parent = nullptr);
    ~MyCharts();
    void initAxis();
    void axisStyle(AxisStyle style, QCPAxis* axis);
signals:
    void tryHandle(QQueue<quint8> *data_queue, MyGraph *mg);
    void choosedDir(QString dir_path);
public:
private:
    Ui::MyChartsClass* ui;
    QCustomPlot* m_plot;
    QVector<MyGraph*> graph;
    QTimer* handle_timer;
    QTimer* replot_timer;
    MyTracer* m_tracer;
    MySerial *m_serial;
    QThread *thread_handle;
    TaskWoker *handle;
    QQueue<quint8> buf;
};