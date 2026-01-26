#ifndef CAPTUREDIMAGEPROVIDER_H
#define CAPTUREDIMAGEPROVIDER_H

#include <QQuickImageProvider>
#include "controllers/CameraService.h"

class CapturedImageProvider : public QQuickImageProvider
{
public:
    CapturedImageProvider(CameraService* camera);
    virtual QImage requestImage(const QString &, QSize *size, const QSize &requestedSize) override;
private:
    CameraService* m_camera;
};

#endif // CAPTUREDIMAGEPROVIDER_H
