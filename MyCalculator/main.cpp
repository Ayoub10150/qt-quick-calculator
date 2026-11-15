#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QIcon>
#include "AppController.h"

int main(int argc, char *argv[])
{
#if defined(Q_OS_WIN) && QT_VERSION_CHECK(5, 6, 0) <= QT_VERSION && QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

    app.setWindowIcon(QIcon(":/app_icon.ico"));

    QQmlApplicationEngine engine;
    AppController appController;
    engine.rootContext()->setContextProperty(QStringLiteral("appController"), &appController);
    engine.load(QUrl(QStringLiteral("qrc:/qt/qml/mycalculator/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
