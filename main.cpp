#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <QGuiApplication>
#include <QtQuick/QQuickView>
#include <QSurfaceFormat>
#include <QGLFormat>
#include <QOpenGLContext>

#include <iostream>
#include <QFontDatabase>
#include "mousemover.h"
#include "camera.h"
#include "controller.h"
using namespace std;

# if defined (Q_OS_IOS)
extern "C" int qtmn (int argc, char * argv [])
#else
int main (int argc, char * argv [])
# endif
{
    qmlRegisterType<CompPhys::Controller>("Controller", 1, 0, "Controller");
    qmlRegisterType<MouseMover>("MouseMover", 1, 0, "MouseMover");
    qmlRegisterType<Camera>("Camera", 1, 0, "Camera");

    QGuiApplication app(argc, argv);
    QQuickView view;

    QFontDatabase::addApplicationFont(":/fonts/SourceSansPro-Regular.ttf");
    app.setFont(QFont("Source Sans Pro"));

    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setSource(QUrl("qrc:///main.qml"));
    view.show();

    return app.exec();
}
