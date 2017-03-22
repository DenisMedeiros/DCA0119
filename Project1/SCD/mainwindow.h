#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_about.h"
#include <QtCharts>
#include <QtSerialPort/QtSerialPort>
#include "serialporthandler.h"

using namespace QtCharts;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Ui::Dialog *aboutUi;
    QDialog *aboutDialog;
    QChart *chartMode1;
    QChart *chartMode2;
    QChartView *chartView;
    QVBoxLayout *layout;
    SerialPortHandler* sph;



public slots:
    void about(void);
};

#endif // MAINWINDOW_H
