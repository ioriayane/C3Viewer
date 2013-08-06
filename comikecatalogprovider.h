#ifndef COMIKECATALOGPROVIDER_H
#define COMIKECATALOGPROVIDER_H

#include <QCryptographicHash>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QStringList>
#include <QFile>
#include <QDir>


class ComikeCatalogProvider
{
public:
    ComikeCatalogProvider();
    ~ComikeCatalogProvider();

    bool open(const QString &path);
    void close();

    bool isReady();

private:
    QSqlDatabase m_db;
    QString m_source;     //DBの元になるファイル
    QString m_sourceMd5;

    bool createData(const QString &path);
};

#endif // COMIKECATALOGPROVIDER_H
