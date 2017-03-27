#ifndef SERIALREADER_H
#define SERIALREADER_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QByteArray>
#include <QBuffer>
#include <QTimer>

class SerialPortHandler : public QObject
{
    Q_OBJECT

private:
    QSerialPort *serialPort;
    QByteArray  *readData;
    QBuffer *buffer;
    QString *portName;
    QTimer *timer;

    QString getAtmegaSerialPort(void);

public:
    explicit SerialPortHandler(QObject *parent = 0);
     ~SerialPortHandler();

    QString getPortName();
    QString getReadData();
    bool isConnected();

    /* async */
    bool startReadingWriting();
    bool stopReadingWriting();
    int writeData(QByteArray data);


signals:

private slots:
    void handleReadyRead();
    void handleError(QSerialPort::SerialPortError error);

public slots:
};

#endif // SERIALREADER_H
