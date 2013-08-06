#ifndef COMIKEDATAACCESS_H
#define COMIKEDATAACCESS_H

#include <QThread>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QStringList>
#include <QMutex>
#include <QEventLoop>

#include "comikecircle.h"

class ComikeDataAccess;

//queue
class QueryQueue
{
public:
    QueryQueue(int operation, const QString &query);
    int operation() const;
    QString query() const;
private:
    int m_operation;
    QString m_query;
};

typedef QList<QueryQueue> QueryQueueList;
typedef QList<ComikeCircle> ComikeCircleList;


class ComikeDataAccess : public QThread
{
    Q_OBJECT
public:
    explicit ComikeDataAccess(QObject *parent = 0);
    ~ComikeDataAccess();
    
    enum OperationType{
        Circle
        , Day
        , Block
        , Genre
    };

    QString m_dbFilePath;

    //クエリーを作成する
    QString makeCircleQuery(const QString &query_dayid = "", const QString &query_blockid = ""
            , const QString &query_genreid = "", const QString &query_keyword = "");
    QString makeDaysQuery();
    QString makeBlocksQuery();
    QString makeGenresQuery();

    //DB操作を登録
    void addQueue(OperationType operation, const QString &query);
    void clearQueue();

private:
    QSqlDatabase m_db;
    QueryQueueList m_queryQueue;
    QMutex m_mutex;
    QEventLoop *m_loop;

    bool openDb(const QString &path);
    void closeDb();
    bool readFromDb(const QString &query_str);
    bool readDaysFromDb(const QString &query_str);
    bool readBlocksFromDb(const QString &query_str);
    bool readGenresFromDb(const QString &query_str);

signals:
    //DBアクセスの完了シグナル
    void finished(bool result, int operation);

    //データを突っ込むためのシグナル
    void addCircle(const ComikeCircleList &circles);
    void addDays(const QString &id, const QString &day);
    void addBlocks(const QString &id, const QString &name);
    void addGenres(const QString &id, const QString &name);

public slots:
    //DB処理待ちで止まってるのを動かす
    void wakeup();
    //スレッドを止める
    void stopThread();

protected:
    void run();
};

#endif // COMIKEDATAACCESS_H
