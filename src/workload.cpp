#include "workload.h"
#include "ui_workload.h"

workload::workload(QDialog *parent)
    : QDialog(parent)
    , ui(new Ui::workload)
{
    ui->setupUi(this);
    tabWidget = new QTabWidget(this);

    tabYear = new QWidget(this);
    tabMonths = new QWidget(this);

    chartYear = new QChart();
    chartYear->legend()->setVisible(false);


    chartMonths = new QChart();
    chartMonths->legend()->setVisible(false);

    chartViewYear = new QChartView(chartYear, this);
    chartViewMonths = new QChartView(chartMonths, this);
    chartViewMonths->setRenderHint(QPainter::Antialiasing);

    listMonths = new QComboBox(this);
    listMonths->addItems( QStringList{"Январь", "Февраль","Март", "Апрель", "Май", "Июнь", "Июль", "Август", "Сентябрь", "Октябрь", "Ноябрь", "Декабрь"});
    connect(listMonths, &QComboBox::currentTextChanged, this, &workload::slot_month_is_changed);
    setFixedSize(1000,600);
}

void workload::slot_status_connection(bool status)
{
    if(status && !existGrafic)
    {
    tabWidget->addTab(tabYear, "Загруженность за год");
    tabWidget->addTab(tabMonths, "Загруженность за месяц");
    QVBoxLayout * tabYearLayout = new QVBoxLayout(tabYear);
    QVBoxLayout * tabMonthsLayout = new QVBoxLayout(tabMonths);
    tabYearLayout->addWidget(chartViewYear);
    tabMonthsLayout->addWidget(listMonths);
    tabMonthsLayout->addWidget(chartViewMonths);
    ui->verticalLayout->addWidget(tabWidget);
    existGrafic = true;
    }
}

void workload::yearRequest(QSqlQueryModel * myModelQuery )
{
    if(seriesYear)
    {
        chartYear->removeSeries(seriesYear);
        delete arrivalAndDepartureBar;
        delete seriesYear;
    }
    seriesYear = new QBarSeries(this);
    seriesYear->setLabelsVisible(true);
    seriesYear->setLabelsPosition(QAbstractBarSeries::LabelsPosition::LabelsOutsideEnd);
    arrivalAndDepartureBar = new QBarSet("Суммарное количетсво прилётов и вылетов", this);
    arrivalAndDepartureBar->setLabelColor(Qt::black);
    int size = myModelQuery->rowCount();
    int index = 0;
    int max = 0;
    QSqlRecord record;
    QVariant variant;
    while(size--)
    {
        record = myModelQuery->record(index++);
        variant = record.value(0);;
        arrivalAndDepartureBar->append(variant.toInt());

        if(max < variant.toInt())
        {
            max = variant.toInt();
        }
    }
    if(!yearAxisY)
    {
        yearAxisY = new QValueAxis(this);
        yearAxisY->setTitleText("Суммарное количество прилётов и вылетов");
        chartYear->addAxis(yearAxisY, Qt::AlignLeft);
    }
    yearAxisY->setRange(0, max*1.5);
    yearAxisY->setLabelFormat("%.0f");
    yearAxisY->setTickCount(5);
    arrivalAndDepartureBar->setColor(Qt::green);
    seriesYear->append(arrivalAndDepartureBar);
    chartYear->setAnimationOptions(QChart::SeriesAnimations);
    chartYear->addSeries(seriesYear);
    chartYear->setTitle(nameCityStat);

    if(!yearAxisX)
    {
        QStringList months = {"Сентябрь", "Октябрь", "Ноябрь", "Декабрь", "Январь", "Февраль", "Март", "Апрель", "Май", "Июнь", "Июль", "Август"};
        yearAxisX = new QBarCategoryAxis(this);
        yearAxisX->append(months);
        chartYear->addAxis(yearAxisX, Qt::AlignBottom);

    }
    seriesYear->attachAxis(yearAxisX);
    seriesYear->attachAxis(yearAxisY);

}

void workload::monthsRequest(QSqlQueryModel * myModelQuery)
{

    chartMonths->setTitle(nameCityStat);
    if(seriesMonths)
    {
        chartMonths->removeSeries(seriesMonths);
        delete seriesMonths;
    }

    seriesMonths = new QLineSeries(this);

    if(!daysAxisX)
    {
       daysAxisX = new QValueAxis(this);
       daysAxisX->setTitleText("Дни месяца");
       chartMonths->addAxis(daysAxisX, Qt::AlignBottom);
    }

    if(!daysAxisY)
    {
        daysAxisY = new QValueAxis(this);
        daysAxisY->setTitleText("Суммарное количество прилётов и вылетов");
        chartMonths->addAxis(daysAxisY, Qt::AlignLeft);
        daysAxisY->setLabelFormat("%d");
    }

    chartMonths->addSeries(seriesMonths);
    seriesMonths->attachAxis(daysAxisX);
    seriesMonths->attachAxis(daysAxisY);

    daysMonthsMap.clear();
    int size = myModelQuery->rowCount();
    int index = 0;

        while(size--)
        {
            QSqlRecord record;
            QVariant variantFirst;
            QVariant variantSecond;
            record = myModelQuery->record(index++);
            variantFirst = record.value(0);
            variantSecond = record.value(1);

            QString str = variantSecond.toString();

            daysMonthsMap[str] = variantFirst.toInt();
        }

    Parcingdays("2017-01-01", MONTHS::JANUARY);
}

void workload::Parcingdays(const QString & str, MONTHS month)
{
    auto itFirst = daysMonthsMap.find(str);
    auto itSecond = itFirst;
    int count = 0;
    if(itFirst!= daysMonthsMap.end())
    {
    maxCountMonths = itFirst.value();
    minCountMonths = itFirst.value();
    seriesMonths->clear();
    for(int i = 1; i <= month; i++)
    {
        if(maxCountMonths < itFirst.value())
        {
           maxCountMonths = itFirst.value();
        }

        if(minCountMonths > itFirst.value())
        {
            minCountMonths = itFirst.value();
        }

        count +=  itFirst.value();

        itFirst++;
    }


    daysAxisY->setMin(0);
    daysAxisX->setMin(1);
    daysAxisX->setLabelFormat("%d");
    daysAxisX->setMax(month);
    daysAxisX->setTickCount(month);
    daysAxisY->setMax(maxCountMonths*2);
    daysAxisY->setTickInterval(1);


    for(int i = 1; i <= month; i++)
    {
        int value = itSecond.value();
        seriesMonths->append(i,value);
        itSecond++;
    }

}
else
{
    qDebug() << "Iter of map is end" << Qt::endl;
}
}

void workload::slot_month_is_changed(const QString & str)
{

    if(str == "Январь"){
        Parcingdays("2017-01-01", MONTHS::JANUARY);
    }
    else if (str == "Сентябрь")
    {
        Parcingdays("2016-09-01", MONTHS::SEPTEMBER);
    }
    else if (str == "Октябрь")
    {
        Parcingdays("2016-10-01", MONTHS::OCTOBER);
    }
    else if (str == "Ноябрь")
    {
        Parcingdays("2016-11-01", MONTHS::NOVEMBER);
    }
    else if (str == "Декабрь")
    {
        Parcingdays("2016-12-01", MONTHS::DECEMBER);
    }
    else if (str == "Февраль")
    {
        Parcingdays("2017-02-01", MONTHS::FEBRUARY);
    }
    else if (str == "Март")
    {
        Parcingdays("2017-03-01", MONTHS::MARCH);
    }
    else if (str == "Апрель")
    {
        Parcingdays("2017-04-01", MONTHS::APRIL);
    }
    else if (str == "Май")
    {
        Parcingdays("2017-05-01", MONTHS::MAY);
    }
    else if (str == "Июнь")
    {
        Parcingdays("2017-06-01", MONTHS::JUNE);
    }
    else if (str == "Июль")
    {
        Parcingdays("2017-07-01", MONTHS::JULY);
    }
    else if (str == "Август")
    {
        Parcingdays("2017-08-01", MONTHS::AUGUST);
    }
}

void workload::setIndexMonths()
{

listMonths->setCurrentIndex(0);

}

workload::~workload()
{
    delete ui;

    if(chartYear)
    {
        delete chartYear;
    }

    if(chartMonths)
    {
        delete chartMonths;
    }

}

void workload::setNameCityStat(QString name)
{
    nameCityStat = name;
}
