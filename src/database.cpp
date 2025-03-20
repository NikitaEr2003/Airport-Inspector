#include "database.h"

DataBase::DataBase(QObject *parent)
    : QObject{parent}
{
    dataBase = new QSqlDatabase();
    modelQueryAirports = new QSqlQueryModel(this);
    modelQueryFlights = new QSqlQueryModel(this);
    modelQueryYear = new QSqlQueryModel(this);
    modelQueryMonths = new QSqlQueryModel(this);
}

DataBase::~DataBase()
{
     delete dataBase;
}

void DataBase::addDataBase(QString driver, QString nameDB)
{

    *dataBase = QSqlDatabase::addDatabase(driver, nameDB);

}

void DataBase::connectToDataBase(QVector<QString> data)
{
    dataBase->setHostName(data[HOST_NAME_]);
    dataBase->setDatabaseName(data[DBNAME_]);
    dataBase->setUserName(data[LOGIN_]);
    dataBase->setPassword(data[PASS_]);
    dataBase->setPort(data[PORT_].toInt());

    bool status = dataBase->open();

    if (!status)
    {
        qDebug() << "Не удалось подключиться к базе данных!";
        emit sig_SendStatusConnection(false);
        return;
    }
    QSqlQuery query(*dataBase);
    if (!query.exec("SET TIMEZONE = 'UTC';"))
    {
        qDebug() << "Не удалось установить часовой пояс!";
    }

    emit sig_SendStatusConnection(true);
}

QSqlQueryModel * DataBase::getAirportsRequests(QString str, QuerySelect SELECT)
{

    if(SELECT == QuerySelect::CITIES_SELECT_)
    {
    modelQueryAirports->setQuery(QSqlQuery(str, *dataBase));
        if (modelQueryAirports->lastError().isValid()) {
        qDebug() << "Ошибка в запросе: " << modelQueryAirports->lastError().text();
         }
        else
        {
        qDebug() << "Запрос выполнен успешно";
        }

    int size = modelQueryAirports->rowCount();

    while(size--)
    {
    QSqlRecord record = modelQueryAirports->record(size);
    QVariant NameCity;
    QVariant Code;
    for (int i = 0; i < record.count(); ++i)
    {
        if(i == 0){
           NameCity = record.value(i);
        }
        else
        {
           Code = record.value(i);
        }
    }
    mapCityAndCode.insert(NameCity.toString(),Code.toString());
    }
    return modelQueryAirports;
    }

    else if(SELECT == QuerySelect::MAINFORM_SELECT_)
    {
        modelQueryFlights->setQuery(QSqlQuery(str, *dataBase));
        if (modelQueryFlights->lastError().isValid())
        {
            qDebug() << "Ошибка в запросе: " << modelQueryFlights->lastError().text();
        } else
        {
            qDebug() << "Запрос выполнен успешно";

        }
    return modelQueryFlights;
    }
    else if (SELECT == QuerySelect::YEAR_SELECT_)
    {
        modelQueryYear->setQuery(QSqlQuery(str, *dataBase));
        if ( modelQueryYear->lastError().isValid()) {
            qDebug() << "Ошибка в запросе: " << modelQueryYear->lastError().text();
        }
        else
        {
            qDebug() << "Запрос выполнен успешно";

        }
    return modelQueryYear;
    }
    else if(SELECT == QuerySelect::MONTHS_SELECT_)
    {
        modelQueryMonths->setQuery(QSqlQuery(str, *dataBase));
        if (modelQueryMonths->lastError().isValid())
        {
            qDebug() << "Ошибка в запросе: " << modelQueryMonths->lastError().text();
        }
        else
        {
            qDebug() << "Запрос выполнен успешно"  << Qt::endl;
        }

    return modelQueryMonths;
    }
    else
    {
        qDebug() << "Ошибка в типе запросе" << Qt::endl;

        return  nullptr;
    }

}

void DataBase::disconnectFromDataBase(QString nameDb)
{

    *dataBase = QSqlDatabase::database(nameDb);
    dataBase->close();

}

QSqlError DataBase::getLastError()
{
    return dataBase->lastError();
}

bool DataBase::isOpen()
{
    return dataBase->isOpen();
}

QString  DataBase::getCode(QString city)
{
    return mapCityAndCode[city];
}


