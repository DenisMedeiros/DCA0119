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
    void nextPoint(float x, float y);
    void bufferAdd(char c);

private:
    Ui::MainWindow *ui;
    Ui::Dialog *aboutUi;
    QDialog *aboutDialog;
    QChart *chartMode1;
    QChart *chartMode2;
    QChartView *chartView;
    QVBoxLayout *layout;
    QLabel *statusMessage;

    SerialPortHandler* sph;
    QByteArray  *serialBuffer;

    QLineSeries *seriesLineMode1;
    QLineSeries *seriesLineMode2;
    QScatterSeries *seriesPointsMode1;
    QScatterSeries *seriesPointsMode2;

    QTimer *timer;

public slots:
    void about(void);
    void handleTimeout(void);
};


#endif // MAINWINDOW_H
