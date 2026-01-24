#ifndef CAMERAIMAGEPROVIDER_H
#define CAMERAIMAGEPROVIDER_H
#include <QQuickImageProvider>
#include "controllers/CameraService.h"
#include <QImage>

class CameraImageProvider : public QQuickImageProvider
{
public:
    CameraImageProvider(CameraService* camera);
    virtual QImage requestImage(const QString &, QSize *size, const QSize &requestedSize) override;
private:
    CameraService* m_camera;
};

#endif // CAMERAIMAGEPROVIDER_H
