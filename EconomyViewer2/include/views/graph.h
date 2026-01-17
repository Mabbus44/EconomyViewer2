#ifndef GRAPH_H
#define GRAPH_H

#include <QFrame>
#include <QPushButton>
#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QHeaderView>
#include <QValueAxis>
#include <QDateTimeAxis>
#include <QDateTime>
#include <QDate>
#include "../core/transaction.h"

namespace core{
class EconomyViewer;
}

namespace views{

class Graph: public QFrame
{
    Q_OBJECT
public:
    Graph();
    Graph(QWidget* parent, core::EconomyViewer* core);
    QPushButton btnBack;
    bool openView();
private:
    void createViewElements();
    void calculateTransactionMinMaxes();
    void setGraphData();
    std::map<QDate, int> amountPerDay(std::map<unsigned int, core::Transaction> transactions);
    core::EconomyViewer* _core=nullptr;
    QDate _transactionMinX, _transactionMaxX;
    int _transactionMinY, _transactionMaxY;
    QScopedPointer<QChart> _chart;
    QScopedPointer<QDateTimeAxis> _xAxis;
    QScopedPointer<QValueAxis> _yAxis;

public slots:
    void btnBackClick(bool checked);
};

}
#endif // GRAPH_H
