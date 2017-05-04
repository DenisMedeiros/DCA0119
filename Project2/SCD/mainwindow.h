#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_about.h"
#include <QtCharts>
#include <QtSerialPort/QtSerialPort>
#include "networkhandler.h"

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
    QChart *chart;
    QChartView *chartView;
    QVBoxLayout *layout;
    QLabel *statusMessage;

    NetworkHandler *nh;

    QLineSeries *seriesLine;
    QScatterSeries *seriesPoints;

    QTimer *timer;
    int mode;

    /* Functions that define how the dryer works based on current time. */
    int dryer_mode1(int time);
    int dryer_mode2(int time);

public slots:
    void about(void);
    void startSystem(void);
    void stopSystem(void);
    void setMode1(void);
    void setMode2(void);

    /* Handler timer timeout. */
    void handleTimeout(void);
};


#endif // MAINWINDOW_H
