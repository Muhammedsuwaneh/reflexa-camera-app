#include "ImageFiltersController.h"
#include <opencv2/imgproc.hpp>

ImageFiltersController::ImageFiltersController(CameraService* camera, QObject* parent)
    : QObject(parent), m_camera(camera) {}

void ImageFiltersController::applyFilter(const QString& filter)
{
    if (!m_camera)
        return;

    this->m_camera->setActiveFilter(filter);
}
