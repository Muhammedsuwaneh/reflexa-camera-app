#ifndef IMAGETEXTURECONTROLLER_H
#define IMAGETEXTURECONTROLLER_H

#include <QObject>
#include "CameraService.h"

class CameraService;

class ImageTextureController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double zoomFactor READ zoomFactor NOTIFY zoomFactorChanged)
public:
    explicit ImageTextureController(CameraService* camera, QObject *parent = nullptr);

    Q_INVOKABLE void adjustBrightness(int brightness);
    Q_INVOKABLE void adjustContrast(int contrast);
    Q_INVOKABLE void adjustSaturation(int saturation);
    Q_INVOKABLE void adjustExposure(int exposure);
    Q_INVOKABLE void adjustGrayScale(int grayScale);
    Q_INVOKABLE void zoomCapture(double value);

    double zoomFactor() const;

signals:
    void zoomFactorChanged();

private:
    CameraService* m_camera;
    double m_zoomFactor = 1.0;
};

#endif // IMAGETEXTURECONTROLLER_H
