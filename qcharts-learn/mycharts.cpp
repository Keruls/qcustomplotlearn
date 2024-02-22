#include "mycharts.h"

MyCharts::MyCharts(QWidget *parent) : QWidget(parent), ui(new Ui::MyChartsClass())
{
	//初始化-----------------------------------------------------------------------------------------
	qDebug() << "main thread:" << QThread::currentThreadId();
	ui->setupUi(this);
	m_plot = ui->m_qcp;
	m_plot->setOpenGl(true);
	qDebug() << "opengl:" << m_plot->openGl();
	initAxis();
	handle_timer = new QTimer(this);
	replot_timer = new QTimer(this);
	graph.push_back(new MyGraph(m_plot, "Line_111",this));
	graph.push_back(new MyGraph(m_plot, "Line_222",this));
	//graph[ 1 ]->setPen(0, 255, 0);
	m_tracer = new MyTracer(m_plot, MyTracer::TextPositionStyle::follow);
	m_serial = new MySerial(&buf, this);
	//多线程处理-----------------------------------------------------------------------------------------
	thread_handle = new QThread;
	handle = new TaskWoker();
	handle->moveToThread(thread_handle);
	connect(handle_timer, &QTimer::timeout, this, [ & ] () {
		emit tryHandle(&buf, graph[ 0 ]);
		//emit tryHandle(&buf, graph[ 1 ]);
	});
	connect(this, &MyCharts::tryHandle, handle, &TaskWoker::run);
	thread_handle->start();
	connect(this, &MyCharts::destroy, this, [ & ] {
		thread_handle->wait();
		thread_handle->quit();
		});
	//游标及一般信号槽连接-----------------------------------------------------------------------------------------
	//图表刷新频率
	connect(replot_timer, &QTimer::timeout, this, [ & ] () {
		m_plot->replot(QCustomPlot::rpQueuedReplot);
		});
	//鼠标移动更新游标位置
	connect(m_plot, &QCustomPlot::mouseMove, m_tracer, &MyTracer::updatePosition);
	//双击曲线使游标吸附在该曲线上
	connect(m_plot, &QCustomPlot::plottableDoubleClick, this, [ & ] (QCPAbstractPlottable *plottable, int index, QMouseEvent *e) {
		QCPGraph *target_graph = qobject_cast<QCPGraph *>(plottable);
		if (target_graph) {//If the plottable is converted to qcpgraph, it means that a graph was clicked with the mouse
			qDebug() << "double clicked graph:" << target_graph->name();
			m_tracer->setParentGraph(target_graph);
		}
		});
	//数据保存-----------------------------------------------------------------------------------------
	//选择目录
	ui->path->setReadOnly(true);
	connect(ui->choose_path, &QPushButton::pressed, this, [ & ] () {
		QFileDialog fd;
		fd.setFileMode(QFileDialog::AnyFile);
		ui->path->setText(fd.getExistingDirectory(this, "Choose Target Directory", "./") + "/");
		});
	//保存曲线数据到路径
	connect(ui->save, &QPushButton::pressed, this, [ & ] () {
		graph[ 0 ]->saveGraphData2File(ui->path->text());
		ui->path->setText("");
		});
	//从文件加载数据-----------------------------------------------------------------------------------------
	//选择目录
	ui->path_2->setReadOnly(true);
	connect(ui->choose_path_2, &QPushButton::pressed, this, [ & ] () {
		QFileDialog fd;
		fd.setFileMode(QFileDialog::AnyFile);
		ui->path_2->setText(fd.getOpenFileName(this, "Choose Target File", "./"));
		});
	connect(ui->load_file, &QPushButton::pressed, this, [ & ] () {
		graph[ 0 ]->loadFileData2Graph(ui->path_2->text());
		ui->path_2->setText("");
		});
	//定时器启动-----------------------------------------------------------------------------------------
	handle_timer->start(HANDLE_TIMER_GAP);
	replot_timer->start(REPLOT_TIMER_GAP);
}

MyCharts::~MyCharts()
{
	delete ui, m_plot, m_tracer, handle, m_serial;
}

void MyCharts::initAxis() {
	m_plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
	m_plot->setBackground(QBrush(QColor(247, 242, 213)));
	m_plot->axisRect()->setBackground(QBrush(QColor(246, 244, 221)));

	AxisStyle as_x;
	as_x.style = "double";
	as_x.lable_name = "（时间/Date）";
	as_x.tick_count = 10;
	as_x.tick_rotate = 30;
	as_x.min_tick = 0;
	as_x.max_tick = as_x.min_tick + 100;
	MyCharts::axisStyle(as_x, m_plot->xAxis);

	AxisStyle as_y;
	as_y.style = "double";
	as_y.lable_name = "（大小/Double）";
	as_y.tick_count = 10;
	as_y.min_tick = -100;
	as_y.max_tick = 100;
	MyCharts::axisStyle(as_y, m_plot->yAxis);
}

//ticker type of axis
void MyCharts::axisStyle(AxisStyle style, QCPAxis *axis)
{
	if (style.style == "date") {
		QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
		dateTicker->setDateTimeFormat("hh:mm:ss.zzz");
		axis->setTicker(dateTicker);
	}
	else if (style.style == "double") {
		//do some set...
	}
	axis->setLabel(style.lable_name);
	axis->setRange(style.min_tick, style.max_tick);
	axis->ticker()->setTickCount(style.tick_count);
	axis->setTickLabelRotation(style.tick_rotate);
	axis->setSubTicks(style.sub_tick_visible);
	axis->grid()->setVisible(style.grid_visible);
}



