#include "CameraService.h"
#include <QDebug>
#include <QThread>

CameraService::CameraService(QObject *parent)
    : QObject{parent}

{
    setCameraDefaults();
    startCamera();
}

inline QImage matToQImage(const cv::Mat& mat)
{
    switch (mat.type())
    {
    case CV_8UC3:
        return QImage(
                   mat.data,
                   mat.cols,
                   mat.rows,
                   mat.step,
                   QImage::Format_BGR888
                   ).copy();

    case CV_8UC1:
        return QImage(
                   mat.data,
                   mat.cols,
                   mat.rows,
                   mat.step,
                   QImage::Format_Grayscale8
                   ).copy();

    default:
        return QImage();
    }
}

QImage CameraService::frame() const
{
    return m_frame;
}

void CameraService::setDetectingFace(bool detecting)
{
    this->detectingFace = detecting;
}

void CameraService::setCameraDefaults()
{
    // set resolution

    // set frequency

    // set is detecting
}

void CameraService::startCamera()
{
    if (!cap.open(0))
        this->running = false;

    this->running = true;

    std::thread([this]() { processFrame(); }).detach();

    this->running = true;
}

void CameraService::stopCamera()
{
    running = false;

    if (cap.isOpened())
        cap.release();

    m_frame = QImage();
    emit frameCleared();
}

void CameraService::processFrame()
{
    try
    {
        cv::Mat mat;

        while(true)
        {
            if(!running) break;

            this->cap >> mat;
            if (mat.empty())
                continue;

            // start detecting face
            if(this->detectingFace) {
                // detect face
            }

            cv::cvtColor(mat, mat, cv::COLOR_BGR2RGB);

            this->m_frame = QImage(
                                mat.data,
                                mat.cols,
                                mat.rows,
                                static_cast<int>(mat.step),
                                QImage::Format_RGB888
                                ).copy();


            if (!running)
                break;

            emit frameChanged();

            QThread::msleep(15);// delay
        }
    }
    catch(QString msg)
    {
        qDebug() << msg;
    }
}
