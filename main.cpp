#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QIcon>
#include "controllers/CameraService.h"
#include "utilities/CameraImageProvider.h"
#include "controllers/WindowController.h"
#include <QQmlContext>
#include "utilities/CameraControls.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/assets/logo.ico"));

    QQmlApplicationEngine engine;

    engine.loadFromModule("reflexaCameraApp", "Main");

    if (engine.rootObjects().isEmpty())
        return -1;

    QWindow *window = qobject_cast<QWindow*>(engine.rootObjects().first());
    if (!window)
        return -1;

    // controller and service objects
    CameraService camera;
    WindowController windowController(window);
    CameraControls* cameraControls = new CameraControls(&camera);
    engine.rootContext()->setContextProperty("cameraControls", cameraControls);

    // providers
    CameraImageProvider* cameraProvider = new CameraImageProvider(&camera);
    engine.addImageProvider("camera", cameraProvider);

    // context
    engine.rootContext()->setContextProperty("WindowController", &windowController);
    qmlRegisterType<CameraService>("Camera", 1, 0, "CameraService");
    //engine.rootContext()->setContextProperty("camera", &camera);


    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    return app.exec();
}
