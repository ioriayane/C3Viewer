#include "comikedatamodel.h"

#include "QDebug"


ComikeListModel::ComikeListModel(QObject *parent) :
    QAbstractListModel(parent)//, m_db(QSqlDatabase::addDatabase("QSQLITE"))
{
    //型の情報を登録しないとシグナルでエラーになる
    qRegisterMetaType<ComikeCircleList>("ComikeCircleList");

    //
    connect(&m_dataAccess, SIGNAL(finished(bool,int)), this, SLOT(finished(bool,int)));
    connect(&m_dataAccess, SIGNAL(addCircle(ComikeCircleList)), this, SLOT(addCircle(ComikeCircleList)));
    connect(&m_dataAccess, SIGNAL(addDays(QString,QString)), this, SLOT(addDays(QString,QString)));
    connect(&m_dataAccess, SIGNAL(addBlocks(QString,QString)), this, SLOT(addBlocks(QString,QString)));
    connect(&m_dataAccess, SIGNAL(addGenres(QString,QString)), this, SLOT(addGenres(QString,QString)));

    //スレッドは開始済み
//    m_dataAccess.start();
}

ComikeListModel::~ComikeListModel()
{
}

void ComikeListModel::update(bool all)
{

    m_dataAccess.m_dbFilePath = dbFilePath();
    clearCircle();
    m_dataAccess.addQueue(ComikeDataAccess::Circle
                          , m_dataAccess.makeCircleQuery(m_queryDayId, m_queryBlockId, m_queryGenreId, m_queryKeyword));
    if(all){
        clearDays();
        clearBlocks();
        clearGenres();
        m_dataAccess.addQueue(ComikeDataAccess::Day, m_dataAccess.makeDaysQuery());
        m_dataAccess.addQueue(ComikeDataAccess::Block, m_dataAccess.makeBlocksQuery());
        m_dataAccess.addQueue(ComikeDataAccess::Genre, m_dataAccess.makeGenresQuery());
    }
    m_dataAccess.wakeup();
}

//サークル情報を複数一気登録
void ComikeListModel::addCircle(const ComikeCircleList &circles)
{
    foreach (ComikeCircle circle, circles) {
        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        m_circles << circle;
        endInsertRows();
    }
}
//サークル情報を登録
void ComikeListModel::addCircle(const ComikeCircle &circle)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_circles << circle;
    endInsertRows();
}

void ComikeListModel::addDays(const QString &id, const QString &day)
{
//    qDebug() << "addDays:" << day;
    m_days << (id + "\t" + day);
}

void ComikeListModel::addBlocks(const QString &id, const QString &name)
{
//    qDebug() << "addBlocks:" << name;
    m_blocks << (id + "\t" + name);
}

void ComikeListModel::addGenres(const QString &id, const QString &name)
{
//    qDebug() << "addGenres:" << name;
    m_genres << (id + "\t" + name);
}

void ComikeListModel::clearCircle()
{
    beginResetModel();
    m_circles.clear();
    endResetModel();
}

void ComikeListModel::clearDays()
{
    m_days.clear();
}

void ComikeListModel::clearBlocks()
{
    m_blocks.clear();
}

void ComikeListModel::clearGenres()
{
    m_genres.clear();
}

int ComikeListModel::rowCount(const QModelIndex &parent) const
{
    return m_circles.count();
}

QVariant ComikeListModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_circles.count())
        return QVariant();

    const ComikeCircle &circle = m_circles[index.row()];
    if (role == IdRole)
        return circle.id();
    else if (role == DayRole)
        return circle.day();
    else if (role == BlockIdRole)
        return circle.blockId();
    else if (role == SpaceNoRole)
        return circle.spaceNo();
    else if (role == SpaceNoSubRole)
        return circle.spaceNoSub();
    else if (role == GenreIdRole)
        return circle.genreId();
    else if (role == CircleNameRole)
        return circle.circleName();
    else if (role == CircleKanaRole)
        return circle.circleKana();
    else if (role == PenNameRole)
        return circle.penName();
    else if (role == BookNameRole)
        return circle.bookName();
    else if (role == UrlRole)
        return circle.url();
    else if (role == MailAddrRole)
        return circle.mailAddr();
    else if (role == DescriptionRole)
        return circle.description();
    else if (role == MemoRole)
        return circle.memo();
    else if (role == UpdateIdRole)
        return circle.updateId();
    else if (role == DayFullRole)
        return circle.dayFull();
    else if (role == BlockNameRole)
        return circle.blockName();
    else if (role == GenreNameRole)
        return circle.genreName();

    return QVariant();
}

//指定したIDの始まるインデックスを調べる
int ComikeListModel::getDayStartIndex(const QString &dayId)
{
    int index = -1;
    for(int i=0; i<m_circles.length(); i++){
        if(m_circles[i].day().compare(dayId) == 0){
            index = i;
            break;
        }
    }
    return index;
}
//指定したIDの始まるインデックスを調べる
int ComikeListModel::getBlockStartIndex(const QString &blockId, const int id)
{
    int index = -1;
    for(int i=0; i<m_circles.length(); i++){
        if((i >= id) && (m_circles[i].blockId().compare(blockId) == 0)){
            index = i;
            break;
        }
    }
    return index;
}
//指定したIDの始まるインデックスを調べる
int ComikeListModel::getGenreStartIndex(const QString &genreId)
{
    int index = -1;
    for(int i=0; i<m_circles.length(); i++){
        if(m_circles[i].genreId().compare(genreId) == 0){
            index = i;
            break;
        }
    }
    return index;
}

//DBのパス
QString ComikeListModel::dbFilePath() const
{
    return m_dbFilePath;
}
//DBのパス
void ComikeListModel::setdbFilePath(const QString &path)
{
    if(m_dbFilePath.compare(path) == 0)
        return;

    m_dbFilePath = path;
    emit dbFilePathChanged();
}
//日付一覧
QStringList ComikeListModel::days() const
{
    return m_days;
}
//ブロック一覧
QStringList ComikeListModel::blocks() const
{
    return m_blocks;
}
//ジャンル一覧
QStringList ComikeListModel::genres() const
{
    return m_genres;
}

//日付での絞り込み
QString ComikeListModel::queryDayId() const
{
    return m_queryDayId;
}
//日付での絞り込み
void ComikeListModel::setQueryDayId(const QString &dayId)
{
    if(m_queryDayId.compare(dayId) == 0)
        return;

    //データ更新
    m_queryDayId = dayId;
    update();
}
//ブロックで絞り込み
QString ComikeListModel::queryBlockId() const
{
    return m_queryBlockId;
}
//ブロックで絞込み
void ComikeListModel::setQueryBlockId(const QString &blockId)
{
    if(m_queryBlockId.compare(blockId) == 0)
        return;

    //データ更新
    m_queryBlockId = blockId;
    update();
}
//ジャンルで絞込み
QString ComikeListModel::queryGenreId() const
{
    return m_queryGenreId;
}
//ジャンルで絞込み
void ComikeListModel::setQueryGenreId(const QString &genreId)
{
    if(m_queryGenreId.compare(genreId) == 0)
        return;

    //データ更新
    m_queryGenreId = genreId;
    update();
}
//キーワードで絞込み
QString ComikeListModel::queryKeyword() const
{
    return m_queryKeyword;
}
//キーワードで絞込み
void ComikeListModel::setQueryKeyword(const QString &keyword)
{
    if(m_queryKeyword.compare(keyword) == 0)
        return;

    //データ更新
    m_queryKeyword = keyword;
    update();
}

QHash<int, QByteArray> ComikeListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[IdRole] = "id";
    roles[DayRole] = "day";
    roles[BlockIdRole] = "blockId";
    roles[SpaceNoRole] = "spaceNo";
    roles[SpaceNoSubRole] = "spaceNoSub";
    roles[GenreIdRole] = "genreId";
    roles[CircleNameRole] = "circleName";
    roles[CircleKanaRole] = "circleKana";
    roles[PenNameRole] = "penName";
    roles[BookNameRole] = "bookName";
    roles[UrlRole] = "url";
    roles[MailAddrRole] = "mailAddr";
    roles[DescriptionRole] = "description";
    roles[MemoRole] = "memo";
    roles[UpdateIdRole] = "updateId";
    roles[DayFullRole] = "dayFull";
    roles[BlockNameRole] = "blockName";
    roles[GenreNameRole] = "genreName";
    return roles;
}


void ComikeListModel::finished(bool result, int operation)
{
    qDebug() << "DB access finish:" << result << "," << operation;

    switch(static_cast<ComikeDataAccess::OperationType>(operation)){
    case ComikeDataAccess::Circle:
        emit circlesChanged();
        break;
    case ComikeDataAccess::Day:
        emit daysChanged();
        break;
    case ComikeDataAccess::Block:
        emit blocksChanged();
        break;
    case ComikeDataAccess::Genre:
        emit genresChanged();
        break;
    default:
        break;
    }

}




