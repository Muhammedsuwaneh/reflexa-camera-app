#include "ImageTextureController.h"

ImageTextureController::ImageTextureController(CameraService* camera, QObject *parent)
    : QObject{parent}, m_camera(camera)
{}

void ImageTextureController::zoomCapture(double value)
{
    value = qBound(0.0, value, 100.0);

    const double minZoom = 0.1;
    const double maxZoom = 3.0;

    double normalized = value / 100.0;
    double newZoom = minZoom + normalized * (maxZoom - minZoom);

    if (qFuzzyCompare(m_zoomFactor, newZoom))
        return;

    m_zoomFactor = newZoom;
    emit zoomFactorChanged();
}

void ImageTextureController::adjustBrightness(int brightness)
{
    this->m_camera->setBrightness(brightness);
}

void ImageTextureController::adjustContrast(int contrast)
{
    this->m_camera->setContrast(contrast);
}

void ImageTextureController::adjustSaturation(int saturation)
{
    this->m_camera->setSaturation(saturation);
}

void ImageTextureController::adjustExposure(int exposure)
{
    this->m_camera->setExposure(exposure);
}

void ImageTextureController::adjustGrayScale(int scale)
{
    this->m_camera->setGrayScale(scale);
}

double ImageTextureController::zoomFactor() const
{
    return m_zoomFactor;
}
