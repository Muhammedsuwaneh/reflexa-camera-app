#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QIcon>
#include "controllers/CameraService.h"
#include "utilities/CameraImageProvider.h"
#include "controllers/WindowController.h"
#include <QQmlContext>
#include "utilities/CameraControls.h"
#include "controllers/ImageFiltersController.h"
#include "controllers/ImageTextureController.h"
#include "utilities/CapturedImageProvider.h"

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
    camera.startCamera();

    WindowController windowController(window);
    CameraControls* cameraControls = new CameraControls(&camera);
    ImageFiltersController* filterController = new ImageFiltersController(&camera);
    ImageTextureController* textureController = new ImageTextureController(&camera);


    // providers
    CameraImageProvider* cameraProvider = new CameraImageProvider(&camera);
    CapturedImageProvider* captureProvider = new CapturedImageProvider(&camera);
    engine.addImageProvider("camera", cameraProvider);
    engine.addImageProvider("captured", captureProvider);

    // context
    engine.rootContext()->setContextProperty("Camera", &camera);
    engine.rootContext()->setContextProperty("cameraControls", cameraControls);
    engine.rootContext()->setContextProperty("WindowController", &windowController);
    engine.rootContext()->setContextProperty("ImageFiltersController", filterController);
    engine.rootContext()->setContextProperty("ImageTextureController", textureController);


    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    return app.exec();
}
