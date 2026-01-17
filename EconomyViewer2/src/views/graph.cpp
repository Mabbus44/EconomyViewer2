#include "../../include/core/economyviewer.h"
#include "../../include/views/graph.h"
#include <QSizePolicy>
#include <QLayout>


namespace views{

Graph::Graph() {
    createViewElements();
}

Graph::Graph(QWidget* parent, core::EconomyViewer* core):QFrame(parent){
    _core = core;
    createViewElements();
}

void Graph::createViewElements(){
    btnBack.setParent(this);
    btnBack.setText("Back");
    btnBack.setToolTip("Goes back to transactions page");
    btnBack.show();

    // Chart
    _chart.reset(new QChart());
    _chart->legend()->hide();

    // Chart view
    auto *chartView = new QChartView(_chart.get());
    chartView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Chart axes
    _xAxis.reset(new QDateTimeAxis);
    _xAxis->setFormat("dd-MM-yyyy");
    _chart->addAxis(_xAxis.get(), Qt::AlignBottom);
    _yAxis.reset(new QValueAxis);
    _chart->addAxis(_yAxis.get(), Qt::AlignLeft);

    QVBoxLayout *layout = new QVBoxLayout();    
    layout->addWidget(chartView, 1);
    layout->addWidget(&btnBack);
    this->setLayout(layout);

    QSizePolicy sp3;
    sp3.setHorizontalPolicy(QSizePolicy::Expanding);
    sp3.setVerticalPolicy(QSizePolicy::Expanding);
    this->setSizePolicy(sp3);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    this->show();
    QObject::connect(&btnBack, &QPushButton::clicked, this, &Graph::btnBackClick);
}

void Graph::setGraphData(){
    // Create series
    auto *series = new QLineSeries();
    std::map<QDate, int> chartData = amountPerDay(_core->getTransactionsRef());
    QDate qdate = _transactionMinX;
    while(qdate <= _transactionMaxX){
        QDateTime xValue;
        xValue.setDate(qdate);
        int amount = 0;
        if(chartData.find(qdate) != chartData.end())
            amount = chartData[qdate];
        series->append(xValue.toMSecsSinceEpoch(), amount);
        qdate = qdate.addDays(1);
    }

    // Add series to chart
    //_chart->removeAllSeries();
    _chart->setTitle("All transactions");
    _chart->addSeries(series);

    // Set tickcount and visibility, otherwise labels wont show up
    const int days = _transactionMinX.daysTo(_transactionMaxX);
    _xAxis->setTickCount(std::clamp(days + 1, 2, 12));
    _xAxis->setLabelsVisible(true);

    // Attach series to axis
    _xAxis->setRange(QDateTime(_transactionMinX, QTime(0,0)), QDateTime(_transactionMaxX, QTime(0,0)));
    series->attachAxis(_xAxis.get());
    series->attachAxis(_yAxis.get());
}

bool Graph::openView(){
    _core->layout->setCurrentWidget(this);
    setGraphData();
    return true;
}

void Graph::btnBackClick(bool checked){
    if(checked){}     //Remove warning "checked unused"
    _core->openTransactionsView();
}

void Graph::calculateTransactionMinMaxes(){
    bool first = true;
    std::map<unsigned int, core::Transaction> transactions = _core->getTransactionsRef();
    for(auto& transaction: transactions){
        std::tm date = transaction.second.transactionDate();
        int amount = transaction.second.transactionAmount();
        QDate qdate(date.tm_year+1900, date.tm_mon+1 , date.tm_mday);
        if(first || qdate < _transactionMinX)
            _transactionMinX = qdate;
        if(first || qdate > _transactionMaxX)
            _transactionMaxX = qdate;
        if(first || amount < _transactionMinY)
            _transactionMinY = amount;
        if(first || amount > _transactionMaxY)
            _transactionMaxY = amount;
        first = false;
    }
}

std::map<QDate, int> Graph::amountPerDay(std::map<unsigned int, core::Transaction> transactions){
    bool first = true;
    std::map<QDate, int> ret;
    for(auto& transaction: transactions){
        // Add amount to date
        std::tm date = transaction.second.transactionDate();
        int amount = transaction.second.transactionAmount();
        QDate qdate(date.tm_year+1900, date.tm_mon+1 , date.tm_mday);
        ret[qdate] += amount;

        // Calculate min/max for x/y
        if(first || qdate < _transactionMinX)
            _transactionMinX = qdate;
        if(first || qdate > _transactionMaxX)
            _transactionMaxX = qdate;
        if(first || amount < _transactionMinY)
            _transactionMinY = amount;
        if(first || amount > _transactionMaxY)
            _transactionMaxY = amount;
        first = false;
    }
    return ret;
}

}
