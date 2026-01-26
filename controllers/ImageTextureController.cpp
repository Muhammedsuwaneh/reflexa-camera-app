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
    cv::Mat src = this->m_camera->originalFrame();
    if (src.empty()) return;

    cv::Mat dst;
    src.convertTo(dst, -1, 1.0, brightness);

    this->m_camera->setOriginalFrame(dst);
}

void ImageTextureController::adjustContrast(int contrast)
{
    cv::Mat src = this->m_camera->originalFrame();
    if (src.empty()) return;

    double alpha = std::clamp(contrast / 100.0 + 1.0, 0.0, 3.0);

    cv::Mat dst;
    src.convertTo(dst, -1, alpha, 0);

    this->m_camera->setOriginalFrame(dst);
}

void ImageTextureController::adjustSaturation(int saturation)
{
    cv::Mat src = this->m_camera->originalFrame();
    if (src.empty() || src.channels() < 3) return;

    cv::Mat hsv;
    cv::cvtColor(src, hsv, cv::COLOR_BGR2HSV);

    std::vector<cv::Mat> channels;
    cv::split(hsv, channels);

    double scale = std::clamp(saturation / 100.0, 0.0, 3.0);
    channels[1].convertTo(channels[1], -1, scale, 0);

    cv::merge(channels, hsv);

    cv::Mat dst;
    cv::cvtColor(hsv, dst, cv::COLOR_HSV2BGR);

    this->m_camera->setOriginalFrame(dst);
}

void ImageTextureController::adjustExposure(int exposure)
{
    cv::Mat src = this->m_camera->originalFrame();
    if (src.empty()) return;

    double gamma = std::clamp(exposure / 100.0 + 1.0, 0.1, 3.0);

    cv::Mat dst;
    cv::Mat f;
    src.convertTo(f, CV_32F, 1.0 / 255.0);

    cv::pow(f, 1.0 / gamma, f);
    f *= 255.0;
    f.convertTo(dst, CV_8U);

    this->m_camera->setOriginalFrame(dst);
}


void ImageTextureController::adjustGrayScale(int)
{
    cv::Mat src = this->m_camera->originalFrame();
    if (src.empty()) return;

    cv::Mat gray, dst;
    cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
    cv::cvtColor(gray, dst, cv::COLOR_GRAY2BGR);

    this->m_camera->setOriginalFrame(dst);
}

double ImageTextureController::zoomFactor() const
{
    return m_zoomFactor;
}
