#include <QtGui/QGuiApplication>
#include <QtCore/QTranslator>
#include <QtCore/QLocale>
#include <QtQml>
#include "qtquick2applicationviewer.h"
#include "comikedatamodel.h"
#include "operatingsystem.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

#if defined(Q_OS_MAC) && !defined(QT_NO_DEBUG)
    //Mac OS X用のパスを作成する
    QString dir = QString("%1/../Resources/qml/C3Viewer/i18n")
            .arg(QCoreApplication::applicationDirPath());
#else
    //WindowsとLinux用のパスを作成する
    QString dir("qml/C3Viewer/i18n");
#endif
    QTranslator translator;
    //言語ファイルの読み込み
    translator.load(QString("qml_%1" ).arg(QLocale::system().name()), dir);
    //言語データを登録する
    app.installTranslator(&translator);

    qmlRegisterType<ComikeListModel>("jp.relog.comikedatamodel"
                                     , 1, 0, "ComikeDataModel");
    qmlRegisterType<OperatingSystem>("jp.relog.operatingsystem"
                                     , 1, 0, "OperatingSystem");

    QtQuick2ApplicationViewer viewer;
    viewer.setMainQmlFile(QStringLiteral("qml/C3Viewer/main.qml"));
    viewer.showExpanded();

    return app.exec();
}
