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
    /* Serial port object. */
    QSerialPort *serialPort;
    /* Bytes read from USART. */
    QByteArray  *readData;
    /* Buffer to save data tah has been read. */
    QBuffer *buffer;
    /* Serial system port. */
    QString *portName;
    /* Returns the Atmega serial port name (e.g. COM3, ttyUSB0). */
    QString getAtmegaSerialPort(void);

public:
    explicit SerialPortHandler(QObject *parent = 0);
     ~SerialPortHandler();
    /* Returns the port name. */
    const QString getPortName();
    /* Return read data. */
    const QString getReadData();
    /* Check if the microcontroller is connected. */
    bool isConnected();

    /* Open the connection and start to read and write data. */
    bool startReadingWriting();
    /* Close the connection and stop to read and write data. */
    bool stopReadingWriting();
    /* Write data on the serial port buffer. */
    int writeData(QByteArray data);


signals:

private slots:
    /* Handle signal when data has been arrive. */
    void handleReadyRead();
    /* Handle error with the connection. */
    void handleError(QSerialPort::SerialPortError error);

public slots:
};

#endif // SERIALREADER_H
