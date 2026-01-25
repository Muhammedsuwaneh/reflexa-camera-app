#ifndef IMAGETEXTURECONTROLLER_H
#define IMAGETEXTURECONTROLLER_H

#include <QObject>
#include "CameraService.h"

class CameraService;

class ImageTextureController : public QObject
{
    Q_OBJECT
public:
    explicit ImageTextureController(CameraService* camera, QObject *parent = nullptr);

    Q_INVOKABLE void adjustBrightness(int brightness);
    Q_INVOKABLE void adjustContrast(int contrast);
    Q_INVOKABLE void adjustSaturation(int saturation);
    Q_INVOKABLE void adjustExposure(int exposure);
    Q_INVOKABLE void adjustGrayScale(int grayScale);

signals:
private:
    CameraService* m_camera;
};

#endif // IMAGETEXTURECONTROLLER_H
