#ifndef DBCONNECTION_H
#define DBCONNECTION_H

#include <QObject>
#include <QSqlDatabase>

class DbConnection
{
public:
    DbConnection();
    DbConnection(QString name);
    bool connectToServerMSSQL(QString driver, QString server, int port, QString database,QString login, QString password);
    bool connectToServerMSSQL(QString driver, QString server, int port, QString database);
    void changeConnName(QString name);
    bool cancelButton=false, acceptButton=false;
    QSqlDatabase db;
    static const QString connection_string_sqlauth;
    static const QString connection_string_winauth;
};

#endif // DBCONNECTION_H
