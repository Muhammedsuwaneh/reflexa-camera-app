#include "ImageFiltersController.h"
#include <opencv2/imgproc.hpp>

ImageFiltersController::ImageFiltersController(CameraService* camera, QObject* parent)
    : QObject(parent), m_camera(camera) {}

void ImageFiltersController::applyFilter(const QString& filter)
{
    if (!m_camera)
        return;

    if (filter == "Gray Scale")
        applyGrayScale();
    else if (filter == "Negative / Invert")
        applyInvert();
    else if (filter == "High Contrast")
        applyHighContrast();
    else if (filter == "Gaussian Blur")
        applyGaussianBlur();
    else if (filter == "Skin Smoothing")
        applySkinSmoothing();
    else if (filter == "Sepia (Warm)")
        applySepia();
}

void ImageFiltersController::applyGrayScale()
{
    cv::Mat src = m_camera->originalFrame();
    if (src.empty()) return;

    cv::Mat gray, dst;
    cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
    cv::cvtColor(gray, dst, cv::COLOR_GRAY2BGR);

    m_camera->setOriginalFrame(dst);
}

void ImageFiltersController::applyInvert()
{
    cv::Mat src = m_camera->originalFrame();
    if (src.empty()) return;

    cv::Mat inverted;
    cv::bitwise_not(src, inverted);

    m_camera->setOriginalFrame(inverted);
}

void ImageFiltersController::applyHighContrast()
{
    cv::Mat src = m_camera->originalFrame();
    if (src.empty()) return;

    cv::Mat contrast;
    src.convertTo(contrast, -1, 1.8, -50); // alpha, beta

    m_camera->setOriginalFrame(contrast);
}

void ImageFiltersController::applyGaussianBlur()
{
    cv::Mat src = m_camera->originalFrame();
    if (src.empty()) return;

    cv::Mat blurred;
    cv::GaussianBlur(src, blurred, cv::Size(9, 9), 0);

    m_camera->setOriginalFrame(blurred);
}

void ImageFiltersController::applySkinSmoothing()
{
    cv::Mat src = m_camera->originalFrame();
    if (src.empty()) return;

    cv::Mat smooth;
    cv::bilateralFilter(src, smooth, 9, 75, 75);

    m_camera->setOriginalFrame(smooth);
}

void ImageFiltersController::applySepia()
{
    cv::Mat src = m_camera->originalFrame();
    if (src.empty()) return;

    cv::Mat sepia;
    cv::Mat kernel = (cv::Mat_<float>(3,3) <<
                          0.272, 0.534, 0.131,
                      0.349, 0.686, 0.168,
                      0.393, 0.769, 0.189);

    cv::transform(src, sepia, kernel);
    sepia.convertTo(sepia, CV_8UC3);

    m_camera->setOriginalFrame(sepia);
}
