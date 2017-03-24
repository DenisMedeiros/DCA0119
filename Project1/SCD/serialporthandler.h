#ifndef SERIALREADER_H
#define SERIALREADER_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QByteArray>
#include <QBuffer>

class SerialPortHandler : public QObject
{
    Q_OBJECT

private:
    QSerialPort *serialPort;
    QByteArray  *readData;
    QBuffer *buffer;

    QString getAtmegaSerialPort(void);

public:
    explicit SerialPortHandler(QObject *parent = 0);
     ~SerialPortHandler();

    QString getReadData();

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
