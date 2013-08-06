#include "comikecircle.h"

ComikeCircle::ComikeCircle(const QString &id
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
                           , const QString &genreName
                           )
    : m_id(id)
    ,m_day(day)
    ,m_blockId(blockId)
    ,m_spaceNo(spaceNo)
    ,m_spaceNoSub(spaceNoSub)
    ,m_genreId(genreId)
    ,m_circleName(circleName)
    ,m_circleKana(circleKana)
    ,m_penName(penName)
    ,m_bookName(bookName)
    ,m_url(url)
    ,m_mailAddr(mailAddr)
    ,m_description(description)
    ,m_memo(memo)
    ,m_updateId(updateId)
    ,m_dayFull(dayFull)
    ,m_blockName(blockName)
    ,m_genreName(genreName)
{
}

QString ComikeCircle::id() const
{
    return m_id;
}

QString ComikeCircle::day() const
{
    return m_day;
}

QString ComikeCircle::blockId() const
{
    return m_blockId;
}

QString ComikeCircle::spaceNo() const
{
    return m_spaceNo;
}

QString ComikeCircle::spaceNoSub() const
{
    return m_spaceNoSub;
}

QString ComikeCircle::genreId() const
{
    return m_genreId;
}

QString ComikeCircle::circleName() const
{
    return m_circleName;
}

QString ComikeCircle::circleKana() const
{
    return m_circleKana;
}

QString ComikeCircle::penName() const
{
    return m_penName;
}

QString ComikeCircle::bookName() const
{
    return m_bookName;
}

QString ComikeCircle::url() const
{
    return m_url;
}

QString ComikeCircle::mailAddr() const
{
    return m_mailAddr;
}

QString ComikeCircle::description() const
{
    return m_description;
}

QString ComikeCircle::memo() const
{
    return m_memo;
}

QString ComikeCircle::updateId() const
{
    return m_updateId;
}

QString ComikeCircle::dayFull() const
{
    return m_dayFull;
}

QString ComikeCircle::blockName() const
{
    return m_blockName;
}

QString ComikeCircle::genreName() const
{
    return m_genreName;
}
