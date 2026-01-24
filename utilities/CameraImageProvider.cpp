#include "CameraImageProvider.h"

CameraImageProvider::CameraImageProvider(CameraService* cameraService) : QQuickImageProvider(QQuickImageProvider::Image), m_camera(cameraService) {}

QImage CameraImageProvider::requestImage(
    const QString &,
    QSize *size,
    const QSize &requestedSize)
{
    QImage img = this->m_camera->frame();

    if (img.isNull()) {
        return QImage();
    }

    if (size)
        *size = img.size();

    if (requestedSize.isValid())
        return img.scaled(requestedSize, Qt::KeepAspectRatio);

    return img;
}
