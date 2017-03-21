#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_about.h"
#include <QtCharts>
#include <QtSerialPort/QtSerialPort>

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

    QString getAtmegaSerialPort(void);
    QByteArray readPort(QSerialPort *port);
    void writePort(QSerialPort *port, const QByteArray &writeData);

public slots:
    void about(void);
};

#endif // MAINWINDOW_H
