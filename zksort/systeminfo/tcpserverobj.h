#ifndef TCPSERVEROBJ_H
#define TCPSERVEROBJ_H

#include <QObject>
#include "logger.h"
#include "global.h"

#define POWER_STATUS_BIT 0

#define COLOR_SORTING_STATUS_BIT 1

#define AIR_PRESSURE_ALARM_BIT 0

#define CIRCUIT_BOARD_ALARM_BIT 1

#define DEEP_LEARNING_ALARM_BIT 2

#define MATERIAL_LEVEL_ALARM_BIT 3

#define LIGHT_ALARM_BIT 4

#define VIBRATOR_ALARM_BIT 5

class TcpServerObj : public QObject
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER

    public:
        explicit TcpServerObj(QObject *parent = 0);
    ~TcpServerObj();

    QString IPforConnect;
    QString PORTforConnect;
    QTcpSocket *tcpSocket;

    QTcpServer *tcpServer;
    QList<QTcpSocket*> m_clients; // 客户端连接列表
    QMap<QTcpSocket*, QByteArray> m_buffers;

    void updateUiState(bool isConnected);

    bool isconnected;

public slots:

    void onConnectButtonPressed();

    void onDisconnectButtonPressed();

    void newConnection();

    void disconnected();

    void readData();

    void displayError(QAbstractSocket::SocketError socketError);

    void sendResponse(QTcpSocket *clientSocket, const Json::Value &response);

    void handleConCommand(QTcpSocket *clientSocket, const Json::Value &request);

    void handleCoffCommand(QTcpSocket *clientSocket, const Json::Value &request);

    void handlePoffCommand(QTcpSocket *clientSocket, const Json::Value &request);

    void handleQsCommand(QTcpSocket *clientSocket, const Json::Value &request);

    void handleContentVCommand(QTcpSocket *clientSocket);

    void sendUnknownCommandResponse(QTcpSocket *clientSocket);

    void serverError(QAbstractSocket::SocketError socketError);
    // 向所有客户端发送消息
    void sendToAllClients(const Json::Value &response);
    // 向指定客户端发送消息
    void sendToClient(QTcpSocket *socket, const Json::Value &response);

signals:
    void clientConnected(const QString &clientInfo);

    void clientDisconnected(const QString &clientInfo);

    void messageReceived(const QString &message);

    void messageSent(const QString &message);
    void updateui();




};

extern TcpServerObj *mytcpServer;


#endif // TCPSERVEROBJ_H
