#include "comikedataaccess.h"
#include <QDebug>


QueryQueue::QueryQueue(int operation, const QString &query)
    : m_operation(operation), m_query(query)
{
}

int QueryQueue::operation() const
{
    return m_operation;
}

QString QueryQueue::query() const
{
    return m_query;
}


ComikeDataAccess::ComikeDataAccess(QObject *parent) :
    QThread(parent), m_db(QSqlDatabase::addDatabase("QSQLITE")), m_loop(NULL)
{
}

ComikeDataAccess::~ComikeDataAccess()
{
    stopThread();
}

//クエリーを作成する
QString ComikeDataAccess::makeCircleQuery(const QString &query_dayid, const QString &query_blockid
                                          , const QString &query_genreid, const QString &query_keyword)
{
    bool condition = false;
    QString query_str("select ComiketCircle.id,ComiketCircle.day,ComiketCircle.blockId,ComiketCircle.spaceNo"
                      ",ComiketCircle.spaceNoSub,ComiketCircle.genreId"
                      ",ComiketCircle.circleName,ComiketCircle.circleKana,ComiketCircle.penName"
                      ",ComiketCircle.bookName,ComiketCircle.url,ComiketCircle.mailAddr"
                      ",ComiketCircle.description,ComiketCircle.memo,ComiketCircle.updateId"
                      ",ComiketDate.year,ComiketDate.month,ComiketDate.day,ComiketDate.weekday"
                      ",ComiketArea.name,ComiketBlock.name "
                      ",ComiketGenre.name"
                      " from ComiketCircle "
                      " inner join ComiketGenre on ComiketCircle.genreId = ComiketGenre.id"
                      " inner join ComiketDate on ComiketCircle.day = ComiketDate.id"
                      " inner join ComiketBlock on ComiketCircle.blockId = ComiketBlock.id"
                      " inner join ComiketArea on ComiketBlock.areaId = ComiketArea.id");
    if((query_dayid.length() > 0)
            || (query_blockid.length() > 0)
            || (query_genreid.length() > 0)
            || (query_keyword.length() > 0))
        query_str.append(" where");
    if(query_dayid.length() > 0){
        query_str.append(" ComiketCircle.day=" + query_dayid);
        if(query_blockid.length() > 0){
            query_str.append(" and ComiketCircle.blockId=" + query_blockid);
        }
        condition = true;
    }
    if(query_genreid.length() > 0){
        if(condition)   query_str.append(" and ");
        query_str.append(" ComiketCircle.genreId=" + query_genreid);
        condition = true;
    }
    if(query_keyword.length() > 0){
        if(condition)   query_str.append(" and ");
        QString t = query_keyword;
        QStringList words = (t.replace(QString("　"), QString(" "))).split(" ");
        for(int i=0; i<words.length(); i++){
            if(i > 0)   query_str.append(" and ");
            query_str.append(" (");
            query_str.append(" (ComiketCircle.description || ComiketCircle.circleName || ComiketCircle.penName || ComiketCircle.circleKana) like '%" + words[i] + "%'");
            query_str.append(" )");
        }
    }

    return query_str;
}
//クエリーを作成する
QString ComikeDataAccess::makeDaysQuery()
{
    return QString("select * from ComiketDate");
}
//クエリーを作成する
QString ComikeDataAccess::makeBlocksQuery()
{
    return QString("select ComiketBlock.id,ComiketBlock.name,ComiketArea.name from ComiketBlock "
                   "inner join ComiketArea on ComiketBlock.areaId = ComiketArea.id ");
}
//クエリーを作成する
QString ComikeDataAccess::makeGenresQuery()
{
    return QString("select * from ComiketGenre");
}

//DB操作の情報をキューに追加する
void ComikeDataAccess::addQueue(ComikeDataAccess::OperationType operation, const QString &query)
{
    m_mutex.lock();
    m_queryQueue << QueryQueue(static_cast<int>(operation), query);
    m_mutex.unlock();
}
//DB操作の情報をクリアする
void ComikeDataAccess::clearQueue()
{
    m_mutex.lock();
    m_queryQueue.clear();
    m_mutex.unlock();
}

//DB操作を止める
void ComikeDataAccess::stopThread()
{
    if(m_loop == NULL)
        return;

    m_loop->exit(0);
}

//DB処理待ちで止まってるのを動かす
void ComikeDataAccess::wakeup()
{
    if(isRunning()){
        if(m_loop == NULL)
            return;

        m_loop->exit(1);
    }else{
        start();
    }
}

//メインスレッド
void ComikeDataAccess::run()
{

    qDebug() << "START DB Access:" << m_dbFilePath;

    m_loop = new QEventLoop();

    do{
        qDebug() << "Do query";

        if(!openDb(m_dbFilePath)){
            //DBが開けない
            qDebug() << m_db.lastError().text();
            emit finished(false, -1);

        }else{
            //実行！

                while(m_queryQueue.length() > 0){
                    //指示の取り出し
                    m_mutex.lock();
                    QueryQueue queue = m_queryQueue.first();
                    m_queryQueue.removeFirst();
                    m_mutex.unlock();

                    switch(static_cast<OperationType>(queue.operation())){
                    case Circle:
                        readFromDb(queue.query());
                        break;
                    case Day:
                        readDaysFromDb(queue.query());
                        break;
                    case Block:
                        readBlocksFromDb(queue.query());
                        break;
                    case Genre:
                        readGenresFromDb(queue.query());
                        break;
                    default:
                        break;
                    }
                    //完了通知
                    emit finished(true, queue.operation());
                }
            closeDb();
        }
    }while(m_loop->exec());

    delete m_loop;
    m_loop = NULL;

    qDebug() << "END DB Access";
}



bool ComikeDataAccess::openDb(const QString &path)
{
    //一旦閉じる
    closeDb();

    //DBを開く
    m_db.setDatabaseName(path);
    if(!m_db.open())
        return false;

    qDebug() << "open db";
//    for(int i=0; i<m_db.tables().count(); i++){
//        qDebug() << m_db.tables().at(i);
//    }

    if(m_db.tables().count() < 1){
        //テーブルが無い
        qDebug() << "not found table";

    }

    return true;
}

void ComikeDataAccess::closeDb()
{
    if(m_db.isOpen()){
        m_db.close();
        qDebug() << "close db";
    }
}

bool ComikeDataAccess::readFromDb(const QString &query_str)
{
    if(!m_db.isOpen())
        return false;

    QList<ComikeCircle> circles;
    QStringList weekday = (QStringList() << QString("日") << QString("月")
                           << QString("火") << QString("水") << QString("木")
                           << QString("金") << QString("土"));

    qDebug() << query_str;

    //クエリ作成
    QSqlQuery query(m_db);
    query.exec(query_str);
    if(query.lastError().isValid())
        qDebug() << query.lastError().text();

    while (query.next()) {
        QString id = query.value(0).toString();
        QString day = query.value(1).toString();
        QString blockId = query.value(2).toString();
        QString spaceNo = query.value(3).toString();
        QString spaceNoSub = query.value(4).toInt() == 0 ? "a" : "b";
        QString genreId = query.value(5).toString();
        QString circleName = query.value(6).toString();
        QString circleKana = query.value(7).toString();
        QString penName = query.value(8).toString();
        QString bookName = query.value(9).toString();
        QString url = query.value(10).toString();
        QString mailAddr = query.value(11).toString();
        QString description = query.value(12).toString();
        QString memo = query.value(13).toString();
        QString updateId = query.value(14).toString();
        QString dayFull = QString("%1/%2/%3(%4)").arg(query.value(15).toString())
                .arg(query.value(16).toString())
                .arg(query.value(17).toString())
                .arg(weekday.at(query.value(18).toInt()-1));
        QString blockName = QString("%1 %2").arg(query.value(19).toString()).arg(query.value(20).toString());
        QString genreName = query.value(21).toString();

//        qDebug() << QString("id(%1),name(%2),memo(%3)").arg(id).arg(circleName).arg(spaceNo);

        circles << ComikeCircle(id, day, blockId, spaceNo, spaceNoSub
                               , genreId, circleName
                               , circleKana, penName
                               , bookName, url, mailAddr
                               , description,memo, updateId, dayFull
                               , blockName, genreName);
    }

    if(circles.length() > 0)
        emit addCircle(circles);

    return true;
}
//日付一覧をDBから取得
bool ComikeDataAccess::readDaysFromDb(const QString &query_str)
{
    if(!m_db.isOpen())
        return false;

    QStringList weekday = (QStringList() << QString("日") << QString("月")
                           << QString("火") << QString("水") << QString("木")
                           << QString("金") << QString("土"));
    QSqlQuery query(m_db);
    query.exec(query_str);
    if(!query.isValid())
        qDebug() << query.lastError().text();

    while(query.next()){
        QString id = query.value(1).toString();
        QString day = QString("%1/%2/%3(%4)").arg(query.value(2).toString())
                .arg(query.value(3).toString())
                .arg(query.value(4).toString())
                .arg(weekday.at(query.value(5).toInt()-1));

        emit addDays(id, day);
    }

    return true;
}
//ブロック一覧をDBから取得
bool ComikeDataAccess::readBlocksFromDb(const QString &query_str)
{
    if(!m_db.isOpen())
        return false;

    QSqlQuery query(m_db);
    query.exec(query_str);
    if(!query.isValid())
        qDebug() << query.lastError().text();

    while(query.next()){
        QString id = query.value(0).toString();
        QString name = QString("%1 %2").arg(query.value(2).toString()).arg(query.value(1).toString());

        emit addBlocks(id, name);
    }

    return true;
}
//ジャンル一覧をDBから取得
bool ComikeDataAccess::readGenresFromDb(const QString &query_str)
{
    if(!m_db.isOpen())
        return false;

    QSqlQuery query(m_db);

    query.exec(query_str);
    if(!query.isValid())
        qDebug() << query.lastError().text();

    while(query.next()){
        QString id = query.value(1).toString();
        QString name = query.value(2).toString();

        emit addGenres(id, name);
    }

    return true;
}


