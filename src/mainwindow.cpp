#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QPixmap>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowIcon(QIcon("://images/picture.jpg"));
    ui->tabWidget_main_->setStyleSheet("QTabBar::tab {"
                                       "   background: transparent;"
                                       "   margin-right: 15px; "
                                       "   border: none;"
                                       "}"
                                       "QTabBar::tab:selected {"
                                       "   color: rgb(50, 205, 50);"
                                       "   font-weight: bold"
                                       "}"
                                       "QTabWidget::pane {"
                                       "   border: none;"
                                       "}");
    ui->label_status_connect_->setStyleSheet("QLabel { color: #00FFFF; } ");
    ui->lineEdit_name_bd_->setStyleSheet("QLineEdit:hover { background-color: rgba(144, 238, 144, 0.5); }""QLineEdit {""font-family: Arial;"  "font-size: 13px;"   "}");
    ui->lineEdit_name_host_->setStyleSheet("QLineEdit:hover { background-color: rgba(144, 238, 144, 0.5); }""QLineEdit {""font-family: Arial;"  "font-size: 13px;"   "}");
    ui->lineEdit_port_->setStyleSheet("QLineEdit:hover { background-color: rgba(144, 238, 144, 0.5); }""QLineEdit {""font-family: Arial;"  "font-size: 13px;"   "}");
    ui->lineEdit_user_->setStyleSheet("QLineEdit:hover { background-color: rgba(144, 238, 144, 0.5);  }""QLineEdit {""font-family: Arial;"  "font-size: 13px;"   "}");
    ui->lineEdit_user_password_->setStyleSheet("QLineEdit:hover { background-color: rgba(144, 238, 144, 0.5); }""QLineEdit {""font-family: Arial;"  "font-size: 13px;"   "}");
    ui->pushButton_clear_->setStyleSheet("QPushButton {"
                                         "   background-color: rgb(60, 179, 113);"
                                         "   color: rgb(255, 255, 255);"
                                         "   border: 2px solid rgb(144, 238, 144);"
                                         "   border-radius: 5px;"
                                         "   padding: 5px;"
                                         "}"
                                         "QPushButton:hover {"
                                         "   background-color: rgb(50, 205, 50);"
                                         "}");
    ui->pushButton_connect_->setStyleSheet("QPushButton {"
                                           "   background-color: rgb(60, 179, 113);"
                                           "   color: rgb(255, 255, 255);"
                                           "   border: 2px solid rgb(144, 238, 144);"
                                           "   border-radius: 5px;"
                                           "   padding: 5px;"
                                           "}"
                                           "QPushButton:hover {"
                                           "   background-color: rgb(50, 205, 50);"
                                           "}");

    ui->groupBox_menu_->setStyleSheet("QGroupBox {"
                                      "    border: 2px solid rgb(169, 169, 169);"
                                      "    margin-top: 20px;"
                                      "}"
                                      "QGroupBox::title {"
                                      "    subcontrol-origin: margin;"
                                      "    subcontrol-position: top left;"
                                      "    padding: 0 1px;"
                                      "    color: black;"

                                      "}"
                                      "QLabel {"
                                      "     color: black;"

                                      "}"
                                      "QLineEdit {"
                                      "    background-color: white;"
                                      "    border: 1px solid rgb(192, 192, 192);"
                                      "    color: black;"
                                      "}");

    ui->groupBox_buttons_->setStyleSheet("QGroupBox {   border: none; }");

    ui->lineEdit_name_host_->setText("981757-ca08998.tmweb.ru");
    ui->lineEdit_port_->setText("5432");
    ui->lineEdit_name_bd_->setText("demo");
    ui->lineEdit_user_->setText("netology_usr_cpp");
    ui->lineEdit_user_password_->setText("CppNeto3");
    ui->radioButton_arrival_->setChecked(true);
    ui->radioButton_departure_->setChecked(false);
    ui->tableView_flights_->setStyleSheet( "QTableView {"
                                          "    background-color: #888888;"  // Серый фон всей таблицы
                                          "    gridline-color: #888888;"    // Серые линии сетки
                                          "    color: #000000;"             // Черный цвет текста в таблице
                                          "}"
                                          "QHeaderView::section {"
                                          "    background-color: #888888;"  // Серый фон заголовков
                                          "    color: #000000;"             // Черный цвет текста заголовков
                                          "    border: 1px solid #888888;"  // Серые границы вокруг заголовков
                                          "}"
                                          "QTableCornerButton::section {"
                                          "    background-color: #888888;"  // Серый фон угловой ячейки
                                          "}"
                                          "QTableView::item:selected {"
                                          "    background-color: #666666;"  // Более темный серый фон для выбранной строки
                                          "}"
                                          "QTableView::item:hover {"
                                          "    background-color: #AAAAAA;"  // Более светлый серый фон при наведении
                                          "}"
                                          "QTableView {"
                                          "    border-radius: 10px;"        // Скругленные углы таблицы
                                          "    border: 1px solid #888888;"  // Серые внешние границы таблицы
                                          "    padding: 5px;"               // Поля внутри таблицы
                                          "}");
    ui->tableView_flights_-> horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_flights_->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    QPixmap pixmap("://images/plane.jpg");
    pixmap = pixmap.scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    QPalette palette;
    palette.setBrush(QPalette::Window, pixmap);
    this->setPalette(palette);
    ui->tabWidget_main_->setCurrentIndex(0);
    myDataBase = new DataBase(this);
    msg  = new QMessageBox(this);
    timer = new QTimer(this);
    workloadDialog = new workload(nullptr);
    myDataBase->addDataBase(POSTGRE_DRIVER, DB_NAME);
    dataForConnect.resize(NUM_DATA_FOR_CONNECT_TO_DB);
    connect(myDataBase, &DataBase::sig_SendStatusConnection, this, &MainWindow::slot_status_connection);
    connect(myDataBase, &DataBase::sig_SendStatusConnection, workloadDialog, &workload::slot_status_connection);
    connect(timer, &QTimer::timeout, this, &MainWindow::slot_refresh_connection);
    myConnect();
}

MainWindow::~MainWindow()
{
    myDataBase->disconnectFromDataBase(DB_NAME);
    delete ui;
    delete workloadDialog;
}

void MainWindow::on_pushButton_connect__clicked()
{
    myConnect();

    bool  state = myDataBase->isOpen();

    if(state)
    {
        qDebug() << "Вы подлючились к базе данных" << Qt::endl;
    }
    else
    {
       qDebug() << "Вы отключились от базы данных" << Qt::endl;
    }
}

void MainWindow::slot_status_connection(bool flag)
{
    connectionStatus = flag;
    if(!flag)
    {
        msg->setText(myDataBase->getLastError().text());
        msg->setIcon(QMessageBox::Critical);
        timer->start(5000);
        msg->show();
    }
    else
    {
        timer->stop();
    }
}


void MainWindow::on_pushButton_clear__clicked()
{
    if(ui->tabWidget_main_->currentIndex() == 0)
    {
        ui->lineEdit_name_bd_->clear();
        ui->lineEdit_name_host_->clear();
        ui->lineEdit_port_->clear();
        ui->lineEdit_user_->clear();
        ui->lineEdit_user_password_->clear();
    }
    else if(ui->tabWidget_main_->currentIndex() == 1)
    {
       QSqlQueryModel* mod = qobject_cast<QSqlQueryModel*>(ui->tableView_flights_->model());

        if (mod)
        {
         mod->clear();
        }

    }
}

void MainWindow::slot_refresh_connection()
{

    msg->close();
    myConnect();
}

void MainWindow::myConnect()
{
    if(ui->label_status_connect_->text() == "Отключено")
    {
        dataForConnect[HOST_NAME_] = ui->lineEdit_name_host_->text();
        dataForConnect[DBNAME_] = ui->lineEdit_name_bd_->text();
        dataForConnect[LOGIN_] = ui->lineEdit_user_->text();
        dataForConnect[PASS_] = ui->lineEdit_user_password_->text();
        dataForConnect[PORT_] = ui->lineEdit_port_->text();
        myDataBase->connectToDataBase(dataForConnect);
        if(connectionStatus)
        {
            ui->label_status_connect_->setText("Подключено");
            ui->pushButton_connect_->setText("Отключится");
            ui->comboBox_airports_->setModel(myDataBase->getAirportsRequests("SELECT airport_name->>'ru' as ""airportName"", airport_code FROM bookings.airports_data", QuerySelect::CITIES_SELECT_));
            ui->tabWidget_main_->setCurrentIndex(1);
            ui->tabWidget_main_->widget(1)->setEnabled(true);
        }
    }
    else if(ui->label_status_connect_->text() == "Подключено")
    {
        ui->label_status_connect_->setText("Отключено");
        ui->pushButton_connect_->setText("Подключится");
        myDataBase->disconnectFromDataBase(DB_NAME);
        ui->tabWidget_main_->setCurrentIndex(0);
        ui->tabWidget_main_->widget(1)->setEnabled(false);
    }
}

void MainWindow::on_pushButton_flights__clicked()
{
    QString NameCity = ui->comboBox_airports_->currentText();

    QString Code = myDataBase->getCode(NameCity);

    QString dateTimeStr = ui->dateEdit_flights_date_->text();


        if(ui->radioButton_arrival_->isChecked())
        {
            ui->tableView_flights_->setModel(myDataBase->getAirportsRequests(
                "SELECT "
                "flight_no AS \"Номер рейса\", "
                "TO_CHAR(scheduled_arrival AT TIME ZONE ad.timezone, 'DD.MM.YYYY HH24:MI') AS \"Время вылета\", "
                "ad.airport_name->>'ru' AS \"Аэропорт отправления\" "
                "FROM bookings.flights f "
                "JOIN bookings.airports_data ad ON ad.airport_code = f.arrival_airport "
                "WHERE f.arrival_airport = '" + Code + "' "
                "AND DATE(scheduled_arrival AT TIME ZONE ad.timezone) = to_date('" + dateTimeStr + "', 'DD/MM/YY')", QuerySelect::MAINFORM_SELECT_));
        } else
        {
            ui->tableView_flights_->setModel(myDataBase->getAirportsRequests(
                "SELECT "
                "flight_no AS \"Номер рейса\", "
                "TO_CHAR(scheduled_departure AT TIME ZONE ad.timezone, 'DD.MM.YYYY HH24:MI') AS \"Время вылета\", "
                "ad.airport_name->>'ru' AS \"Аэропорт назначения\" "
                "FROM bookings.flights f "
                "JOIN bookings.airports_data ad ON ad.airport_code = f.arrival_airport "
                "WHERE f.departure_airport = '" + Code + "' "
                "AND DATE(scheduled_departure AT TIME ZONE ad.timezone) = to_date('" + dateTimeStr + "', 'DD/MM/YY')",
                QuerySelect::MAINFORM_SELECT_));
        }
}


void MainWindow::on_pushButton_workload__clicked()
{
    QString NameCity = ui->comboBox_airports_->currentText();
    QString Code = myDataBase->getCode(NameCity);
    workloadDialog->setNameCityStat(NameCity);
    workloadDialog->yearRequest(myDataBase->getAirportsRequests(
    "SELECT count(flight_no), "
    "date_trunc('month', scheduled_departure AT TIME ZONE ad.timezone)::date AS \"Month\" "
    "FROM bookings.flights f "
    "JOIN bookings.airports_data ad "
    "ON (ad.airport_code = f.departure_airport OR ad.airport_code = f.arrival_airport) "
    "WHERE (scheduled_departure::date >= DATE('2016-09-01') AND scheduled_departure::date < DATE('2017-09-01')) "
    "AND (departure_airport = '" + Code + "' OR arrival_airport = '" + Code + "') "
    "GROUP BY \"Month\"",
    QuerySelect::YEAR_SELECT_));
    workloadDialog->setIndexMonths();
    workloadDialog->monthsRequest(myDataBase->getAirportsRequests(
    "SELECT count(flight_no),  date_trunc('day', scheduled_departure AT TIME ZONE ad.timezone)::date AS \"Day\" "
    "FROM bookings.flights f "
    "JOIN bookings.airports_data ad ON (ad.airport_code = f.departure_airport OR ad.airport_code = f.arrival_airport) "
    "WHERE (scheduled_departure >= DATE('2016-09-01') AND scheduled_departure::date < DATE('2017-09-01')) "
    "AND (departure_airport = '" + Code + "' OR arrival_airport = '" + Code + "') "
    "GROUP BY \"Day\"",
    QuerySelect::MONTHS_SELECT_));
    workloadDialog->exec();

}

