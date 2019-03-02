#include "dbconnection.h"

const QString DbConnection::connection_string_sqlauth =
        QString("DRIVER={%1};SERVER=%2;PORT=%3;DATABASE=%4;UID=%5;PWD=%6");

const QString DbConnection::connection_string_winauth =
        QString("DRIVER={%1};SERVER=%2;PORT=%3;DATABASE=%4");

DbConnection::DbConnection()
{
    db=QSqlDatabase::addDatabase("QODBC");
}

DbConnection::DbConnection(QString name)
{
    if(name==db.connectionName())
        db.close();
    else
        db=QSqlDatabase::addDatabase("QODBC",name);
}

bool DbConnection::connectToServerMSSQL(QString driver, QString server, int port, QString database, QString login, QString password)
{
    db.setDatabaseName(connection_string_sqlauth.arg(driver).arg(server).arg(port).arg(database).arg(login).arg(password));
    return db.open();
}

bool DbConnection::connectToServerMSSQL(QString driver, QString server, int port, QString database)
{
    db.setDatabaseName(connection_string_winauth.arg(driver).arg(server).arg(port).arg(database));
    return db.open();
}

void DbConnection::changeConnName(QString name)
{

}
