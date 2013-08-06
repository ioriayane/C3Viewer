#ifndef COMIKECIRCLE_H
#define COMIKECIRCLE_H

#include <QString>

class ComikeCircle
{
public:
    ComikeCircle(const QString &id
                 , const QString &day
                 , const QString &blockId
                 , const QString &spaceNo
                 , const QString &spaceNoSub
                 , const QString &genreId
                 , const QString &circleName
                 , const QString &circleKana
                 , const QString &penName
                 , const QString &bookName
                 , const QString &url
                 , const QString &mailAddr
                 , const QString &description
                 , const QString &memo
                 , const QString &updateId
                 , const QString &dayFull
                 , const QString &blockName
                 , const QString &genreName);

    QString id() const;
    QString day() const;
    QString blockId() const;
    QString spaceNo() const;
    QString spaceNoSub() const;
    QString genreId() const;
    QString circleName() const;
    QString circleKana() const;
    QString penName() const;
    QString bookName() const;
    QString url() const;
    QString mailAddr() const;
    QString description() const;
    QString memo() const;
    QString updateId() const;
    QString dayFull() const;
    QString blockName() const;
    QString genreName() const;

private:
    QString m_id;
    QString m_day;
    QString m_blockId;
    QString m_spaceNo;
    QString m_spaceNoSub;
    QString m_genreId;
    QString m_circleName;
    QString m_circleKana;
    QString m_penName;
    QString m_bookName;
    QString m_url;
    QString m_mailAddr;
    QString m_description;
    QString m_memo;
    QString m_updateId;
    QString m_dayFull;
    QString m_blockName;
    QString m_genreName;

};

#endif // COMIKECIRCLE_H
