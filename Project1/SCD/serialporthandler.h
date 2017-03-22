#ifndef SERIALREADER_H
#define SERIALREADER_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QTextStream>
#include <QTimer>
#include <QByteArray>


class SerialPortHandler : public QObject
{
    Q_OBJECT

private:
    QSerialPort *serialPort;
    QByteArray  *readData;
    QTextStream *standardOutput;
    QTimer      *timer;

    QString getAtmegaSerialPort(void);

public:
    explicit SerialPortHandler(QObject *parent = 0);
     ~SerialPortHandler();

    /* async */
    bool startReadingWriting();
    bool stopReadingWriting();

    /* sync */
    QByteArray readPort(QSerialPort *port);
    int writeData(QByteArray data);


signals:

private slots:
    void handleReadyRead();
    void handleTimeout();
    void handleError(QSerialPort::SerialPortError error);

public slots:
};

#endif // SERIALREADER_H
