#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "database.h"
#include <QMessageBox>
#include <QTimer>
#include "workload.h"
QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_pushButton_connect__clicked();
    void slot_status_connection(bool);
    void slot_refresh_connection();
    void on_pushButton_clear__clicked();
    void on_pushButton_flights__clicked();
    void on_pushButton_workload__clicked();

private:

    void myConnect();
    bool connectionStatus = false;;
    Ui::MainWindow * ui;
    DataBase * myDataBase;
    QMessageBox * msg;
    QVector<QString> dataForConnect;
    QTimer * timer;
    workload * workloadDialog;

};
#endif // MAINWINDOW_H
