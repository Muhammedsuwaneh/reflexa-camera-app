#ifndef IMAGEFILTERSCONTROLLER_H
#define IMAGEFILTERSCONTROLLER_H

#include <QObject>
#include "CameraService.h"

class ImageFiltersController : public QObject
{
    Q_OBJECT
public:
    explicit ImageFiltersController(CameraService* camera, QObject* parent = nullptr);

    Q_INVOKABLE void applyFilter(const QString& filter);
    CameraService* m_camera;
};

#endif // IMAGEFILTERSCONTROLLER_H
