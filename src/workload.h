#ifndef WORKLOAD_H
#define WORKLOAD_H
#include <QDialog>
#include <QtCharts/QtCharts>
#include <QtCharts/QBarSeries>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSet>
#include <QtCharts/QLineSeries>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QGraphicsSimpleTextItem>
#include <QVector>
namespace Ui
{
class workload;
}

enum  MONTHS
{
    JANUARY = 31,
    FEBRUARY = 28,
    MARCH = 31,
    APRIL = 30,
    MAY = 31,
    JUNE = 30,
    JULY = 31,
    AUGUST = 31,
    SEPTEMBER = 30,
    OCTOBER = 31,
    NOVEMBER = 30,
    DECEMBER = 31
};

class workload : public QDialog
{
    Q_OBJECT

public:

    explicit workload(QDialog *parent = nullptr);
    void yearRequest(QSqlQueryModel * myModelQuery = nullptr);
    void monthsRequest(QSqlQueryModel * myModelQuery = nullptr);
    void setNameCityStat(QString name);
    void setIndexMonths();
    ~workload();

private:

    int maxCountMonths = 0;
    int minCountMonths = 0;
    QString nameCityStat;
    bool existGrafic = false;

    QChart* chartYear = nullptr;
    QChart* chartMonths = nullptr;

    QChartView* chartViewYear = nullptr;
    QChartView* chartViewMonths = nullptr;

    QBarSeries* seriesYear = nullptr;

    QTabWidget * tabWidget = nullptr;

    QWidget * tabYear = nullptr;
    QWidget * tabMonths = nullptr;

    QBarSet * arrivalAndDepartureBar = nullptr;
    QBarCategoryAxis * yearAxisX = nullptr;
    QValueAxis *  yearAxisY = nullptr;

    QComboBox * listMonths = nullptr;
    QLineSeries * seriesMonths = nullptr;
    QValueAxis *  daysAxisY = nullptr;
    QValueAxis *  daysAxisX = nullptr;

    QMap <QString, int> daysMonthsMap;

    void Parcingdays(const QString & str, MONTHS month);

    Ui::workload * ui;

public slots:

    void slot_status_connection(bool status);

private slots:

    void slot_month_is_changed(const QString & str);

};

#endif // WORKLOAD_H
