#include "comikecatalogprovider.h"

#include <QDebug>

ComikeCatalogProvider::ComikeCatalogProvider() :
    m_source(""), m_sourceMd5("")
{
}

ComikeCatalogProvider::~ComikeCatalogProvider()
{
    close();
}

bool ComikeCatalogProvider::open(const QString &path)
{
    m_source = path;

    qDebug() << m_source;

    //DBを開く
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(m_source);
    if(m_db.open()){
        qDebug() << "open db";
        for(int i=0; i<m_db.tables().count(); i++){
            qDebug() << m_db.tables().at(i);
        }

        if(m_db.tables().count() < 1){
            //テーブルが無い
            qDebug() << "not found table";

        }

        //        QSqlQuery query("select id,circleName from ComiketCircle where id<100", db);
        //        qDebug() << query.lastError().text();
    }
}

void ComikeCatalogProvider::close()
{
    if(m_db.isOpen()){
        m_db.close();
        qDebug() << "close db";
    }
}

bool ComikeCatalogProvider::isReady()
{
    return false;
}

bool ComikeCatalogProvider::createData(const QString &path)
{
    bool result = false;

    //

    QSqlQuery query(m_db);
    query.exec("CREATE TABLE IF NOT EXISTS ComiketCircle("
               "comiketNo,"
               "id,"     //-- サークルID
               "pageNo,"//        -- ページ番号         漏れの場合は 0
               "cutIndex,"//        -- カットインデックス 漏れの場合は 0
               "day,"//        -- 参加日   漏れの場合は 0
               "blockId,"//        -- ブロックID         漏れの場合は 0
               "spaceNo,"//        -- スペース番号       漏れの場合は 0
               "spaceNoSub,"//	   -- スペース番号補助   0:a 1:b
               "genreId,"//        -- ジャンルID
               "circleName,"//    -- サークル名
               "circleKana,"//    -- サークル名(読みがな) 全角カナで正規化
               "penName,"//   -- 執筆者名
               "bookName,"//   -- 発行誌名
               "url,"//   -- URL
               "mailAddr,"//   -- メールアドレス
               "description,"//  -- 補足説明
               "memo,"//  -- サークルメモ
               "updateId,"//        -- 更新用ID
               "updateData,"//  -- 更新情報
               "circlems,"//   -- Circle.ms URL
               "rss,"//   -- RSS
               "updateFlag,"//        -- 更新フラグ
               "PRIMARY KEY (comiketNo,id)"//
               ")"
               );
    if(query.lastError().isValid())
        qDebug() << query.lastError().text();
    else
        qDebug() << "create table ComiketCircle";
    //    query.exec("create index ComiketCirclePage on ComiketCircle(pageNo,comiketNo)");
    //    query.exec("create index ComiketCircleName on ComiketCircle(circleName,comiketNo)");
    //    query.exec("create index ComiketCircleKana on ComiketCircle(circleKana,comiketNo)");
    //    query.exec("create index ComiketCirclePenName on ComiketCircle(penName,comiketNo)");
    //    query.exec("create index ComiketCircleUpdateId on ComiketCircle(updateId,comiketNo)");
    //    qDebug() << query.lastError().text();


    QFile file(path);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << file.errorString();
    }else{
        QTextStream in(&file);
        QString line;
        QStringList items;
        int i=0;
        in.setCodec("UTF-8");

        //クエリ作成
        query.prepare("insert into ComiketCircle (comiketNo,id,pageNo,cutIndex,day,blockId,spaceNo,spaceNoSub,"
                      "genreId,circleName,circleKana,"
                      "penName,bookName,url,mailAddr,description,memo,updateId,updateData,circlems,rss,updateFlag) "
                      "values ('', ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, '', '', '', '')");
        //インサート
        while(!in.atEnd()){
//            if(i++ > 20) break;
            //行を読み込む
            line = in.readLine();
            items = line.split("\t");
            //落選は除外
            if(items.at(0).toLong() > 200000)
                break;
            //関連付け
            for(int k=0; k<17; k++){
                query.bindValue(k, items.at(k));
            }
            //実行
            query.exec();
            if(query.lastError().isValid())
                qDebug() << query.lastError().text();
        }
        file.close();

        result = true;
    }
    return result;
}
