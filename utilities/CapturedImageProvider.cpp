#include "CapturedImageProvider.h"

CapturedImageProvider::CapturedImageProvider(CameraService* cameraService) : QQuickImageProvider(QQuickImageProvider::Image), m_camera(cameraService) {}

QImage CapturedImageProvider::requestImage(
    const QString &,
    QSize *size,
    const QSize &requestedSize)
{
    QImage img = this->m_camera->recentCaptured();

    if (img.isNull()) {
        return QImage();
    }

    if (size)
        *size = img.size();

    if (requestedSize.isValid())
        return img.scaled(requestedSize, Qt::KeepAspectRatio);

    return img;
}
