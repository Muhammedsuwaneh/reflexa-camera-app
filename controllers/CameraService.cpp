#include "CameraService.h"
#include <QDebug>
#include <QThread>
#include <QMediaDevices>
#include <QCameraDevice>
#include <QMessageBox>

CameraService::CameraService(QObject *parent)
    : QObject{parent}

{
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
    // set PHOTO Quality
    applyPhotoQuality(0);

    // set is detecting
    setDetectingFace(false);
}

void CameraService::applyPhotoQuality(int qualityIndex)
{
    const auto devices = QMediaDevices::videoInputs();
    if (devices.isEmpty())
        return;

    if (m_currentCameraIndex < 0 || m_currentCameraIndex >= devices.size())
        return;

    const QCameraDevice &device = devices[m_currentCameraIndex];

    QList<QSize> photoResolutions;
    for (const QCameraFormat &f : device.videoFormats())
    {
        if (!photoResolutions.contains(f.resolution()))
            photoResolutions << f.resolution();
    }

    if (qualityIndex < 0 || qualityIndex >= photoResolutions.size())
        return;

    const QSize size = photoResolutions[qualityIndex];
    const int width  = size.width();
    const int height = size.height();

    running = false;
    QThread::msleep(50);

    if (cap.isOpened())
        cap.release();

    if (!cap.open(m_currentCameraIndex, cv::CAP_MSMF))
    {
        QMessageBox::critical(
            nullptr,
            tr("Camera Error"),
            tr("Failed to reopen camera device."),
            QMessageBox::Ok
            );
        return;
    }

    cap.set(cv::CAP_PROP_FRAME_WIDTH,  width);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, height);

    setCurrentQualityIndex(qualityIndex);

    running = true;
    std::thread([this]() { processFrame(); }).detach();
}


void CameraService::applyVideoQuality(int formatIndex)
{
    const auto devices = QMediaDevices::videoInputs();
    if (devices.isEmpty())
        return;

    if (m_currentCameraIndex < 0 || m_currentCameraIndex >= devices.size())
        return;

    const QCameraDevice &device = devices[m_currentCameraIndex];

    const auto formats = device.videoFormats();
    if (formatIndex < 0 || formatIndex >= formats.size())
        return;

    const QCameraFormat &format = formats[formatIndex];

    const QSize res = format.resolution();
    const int width  = res.width();
    const int height = res.height();
    const double fps = format.maxFrameRate();

    running = false;
    QThread::msleep(50);

    if (cap.isOpened())
        cap.release();

    if (!cap.open(m_currentCameraIndex, cv::CAP_MSMF))
    {
        QMessageBox::critical(
            nullptr,
            tr("Camera Error"),
            tr("Failed to reopen camera device."),
            QMessageBox::Ok
            );
        return;
    }

    cap.set(cv::CAP_PROP_FRAME_WIDTH,  width);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, height);
    cap.set(cv::CAP_PROP_FPS, fps);

    setCurrentVideoQualityIndex(formatIndex);

    running = true;
    std::thread([this]() { processFrame(); }).detach();
}

void CameraService::takeShot()
{
    qDebug() << "Captured ...";
}

void CameraService::record()
{
    qDebug() << "Capturing ...";
}

void CameraService::scanQR()
{
    qDebug() << "Scanning QR ...";
}

static QString formatVideoQuality(const QCameraFormat &f)
{
    return QString("%1 x %2  (%3–%4 FPS)")
        .arg(f.resolution().width())
        .arg(f.resolution().height())
        .arg(int(f.minFrameRate()))
        .arg(int(f.maxFrameRate()));
}

static QString formatPhotoQuality(const QSize &size)
{
    return QString("%1 x %2")
    .arg(size.width())
        .arg(size.height());
}

void CameraService::startCamera()
{
    try
    {
        m_camQualities.clear();
        m_videoQualities.clear();

        const QList<QCameraDevice> cameraDevices = QMediaDevices::videoInputs();

        if (!cameraDevices.isEmpty())
        {
            for(const QCameraDevice& device: cameraDevices)
            {
                this->m_cameraNames.push_back(device.description());
            }

            const QCameraDevice &currentDevice = cameraDevices[m_currentCameraIndex];

            QList<QSize> photoSizes;

            for (const QCameraFormat &format : currentDevice.videoFormats())
            {
                m_videoQualities.push_back(formatVideoQuality(format));

                if (!photoSizes.contains(format.resolution()))
                    photoSizes << format.resolution();
            }

            std::sort(photoSizes.begin(), photoSizes.end(),
                      [](const QSize &a, const QSize &b) {
                          return a.width() * a.height() > b.width() * b.height();
                      });

            for (const QSize &size : photoSizes)
                m_camQualities.push_back(formatPhotoQuality(size));

            emit camQualitiesChanged();
            emit videoQualitiesChanged();
            emit camerasChanged();

            if (!cap.open(m_currentCameraIndex))
                this->running = false;

            this->running = true;

            setCameraDefaults();

            std::thread([this]() { processFrame(); }).detach();

            this->running = true;
        }
        else
        {
            QMessageBox::information(
                nullptr,
                tr("Device Error"),
                tr("No camera devices connected.\nPlease connect a camera device."),
                QMessageBox::Ok
                );
        }
    }
    catch (const std::exception &e)
    {
        QMessageBox::critical(
            nullptr,
            tr("Camera Error"),
            QString::fromStdString(e.what()),
            QMessageBox::Ok
            );
    }
}

void CameraService::stopCamera()
{
    try
    {
        running = false;

        if (cap.isOpened())
            cap.release();

        m_frame = QImage();
        emit frameCleared();
    }
    catch(const char* msg)
    {
        qDebug() << msg;
    }
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
    catch (const std::exception &e)
    {
        QMessageBox::critical(
            nullptr,
            tr("Camera Error"),
            QString::fromStdString(e.what()),
            QMessageBox::Ok
            );
    }
}

QStringList CameraService::cameraNames() const
{
    return m_cameraNames;
}

void CameraService::switchCam(int index)
{
    setCurrentCameraIndex(index);
    stopCamera();
    startCamera();
}

int CameraService::currentCameraIndex() const
{
    return m_currentCameraIndex;
}

void CameraService::setCurrentCameraIndex(int newCurrentCameraIndex)
{
    if (m_currentCameraIndex == newCurrentCameraIndex)
        return;
    m_currentCameraIndex = newCurrentCameraIndex;
    emit cameraChanged();
}

QStringList CameraService::camQualities() const
{
    return m_camQualities;
}

int CameraService::currentQualityIndex() const
{
    return m_currentQualityIndex;
}

void CameraService::setCurrentQualityIndex(int newCurrentQualityIndex)
{
    if (m_currentQualityIndex == newCurrentQualityIndex)
        return;
    m_currentQualityIndex = newCurrentQualityIndex;
    emit currentQualityIndexChanged();
}

QStringList CameraService::videoQualities() const
{
    return m_videoQualities;
}

int CameraService::currentVideoQualityIndex() const
{
    return m_currentVideoQualityIndex;
}

void CameraService::setCurrentVideoQualityIndex(int newCurrentVideoQualityIndex)
{
    if (m_currentVideoQualityIndex == newCurrentVideoQualityIndex)
        return;
    m_currentVideoQualityIndex = newCurrentVideoQualityIndex;
    emit currentVideoQualityIndexChanged();
}

CameraService::Mode CameraService::mode() const
{
    return m_mode;
}

void CameraService::setMode(const Mode &newMode)
{
    if (m_mode == newMode)
        return;
    m_mode = newMode;
    emit modeChanged();
}
