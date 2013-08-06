#ifndef COMIKEDATAMODEL_H
#define COMIKEDATAMODEL_H

#include <QAbstractListModel>
#include <QStringList>
//#include "comikecatalogprovider.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QFile>
#include <QDir>

#include "comikecircle.h"
#include "comikedataaccess.h"

class ComikeListModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(QString dbFilePath READ dbFilePath WRITE setdbFilePath NOTIFY dbFilePathChanged)
    Q_PROPERTY(QStringList days READ days NOTIFY daysChanged)
    Q_PROPERTY(QStringList blocks READ blocks NOTIFY blocksChanged)
    Q_PROPERTY(QStringList genres READ genres NOTIFY genresChanged)

    Q_PROPERTY(QString queryDayId READ queryDayId WRITE setQueryDayId)
    Q_PROPERTY(QString queryBlockId READ queryBlockId WRITE setQueryBlockId)
    Q_PROPERTY(QString queryGenreId READ queryGenreId WRITE setQueryGenreId)
    Q_PROPERTY(QString queryKeyword READ queryKeyword WRITE setQueryKeyword NOTIFY queryKeywordChanged)

public:
    enum ComikeCircleRoles {
        IdRole = Qt::UserRole + 1
        ,DayRole
        ,BlockIdRole
        ,SpaceNoRole
        ,SpaceNoSubRole
        ,GenreIdRole
        ,CircleNameRole
        ,CircleKanaRole
        ,PenNameRole
        ,BookNameRole
        ,UrlRole
        ,MailAddrRole
        ,DescriptionRole
        ,MemoRole
        ,UpdateIdRole
        ,DayFullRole
        ,BlockNameRole
        ,GenreNameRole
    };

    explicit ComikeListModel(QObject *parent = 0);
    ~ComikeListModel();
    
    Q_INVOKABLE void update(bool all = false);
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    Q_INVOKABLE int getDayStartIndex(const QString &dayId);
    Q_INVOKABLE int getBlockStartIndex(const QString &blockId, const int id = 0);
    Q_INVOKABLE int getGenreStartIndex(const QString &genreId);


    QString dbFilePath() const;
    void setdbFilePath(const QString &path);
    //各種情報
    QStringList days() const;
    QStringList blocks() const;
    QStringList genres() const;
    //検索条件
    QString queryDayId() const;
    void setQueryDayId(const QString &dayId);
    QString queryBlockId() const;
    void setQueryBlockId(const QString &blockId);
    QString queryGenreId() const;
    void setQueryGenreId(const QString &genreId);
    QString queryKeyword() const;
    void setQueryKeyword(const QString &keyword);

protected:
    QHash<int, QByteArray> roleNames() const;
private:
    QList<ComikeCircle> m_circles;
    ComikeDataAccess m_dataAccess;
    QString m_dbFilePath;
//    QSqlDatabase m_db;
    QStringList m_days;
    QStringList m_blocks;
    QStringList m_genres;
    QString m_queryDayId;
    QString m_queryBlockId;
    QString m_queryGenreId;
    QString m_queryKeyword;

signals:
    void dbFilePathChanged();
    //DB検索結果の反映されたシグナル
    void circlesChanged();
    void daysChanged();
    void blocksChanged();
    void genresChanged();
    //検索条件
    void queryKeywordChanged();

public slots:
    //DB操作スレッドから結果の受け取り
    void addCircle(const ComikeCircleList &circles);
    void addCircle(const ComikeCircle &circle);
    void addDays(const QString &id, const QString &day);
    void addBlocks(const QString &id, const QString &name);
    void addGenres(const QString &id, const QString &name);

    void clearCircle();
    void clearDays();
    void clearBlocks();
    void clearGenres();

    void finished(bool result, int operation);
};

#endif // COMIKEDATAMODEL_H
