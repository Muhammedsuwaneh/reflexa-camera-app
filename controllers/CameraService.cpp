#include "CameraService.h"
#include <QDebug>
#include <QMediaDevices>
#include <QCameraDevice>
#include <QMessageBox>
#include <opencv2/opencv.hpp>
#include <algorithm>

CameraService::CameraService(QObject *parent)
    : QObject{parent}
{
    connect(&timer, &QTimer::timeout, this, &CameraService::processFrame);
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
                       QImage::Format_RGB888
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

void CameraService::init()
{
    applyPhotoQuality(this->m_currentQualityIndex);
}

QImage CameraService::frame() const
{
    return m_frame;
}

void CameraService::setDetectingFace(bool detecting)
{
    this->detectingFace = detecting;
}

void CameraService::startCamera()
{
    if (this->running)
        return;

    getCaptureData();

    if (!this->cap.open(this->m_currentCameraIndex, cv::CAP_DSHOW))
    {
        qDebug() << "Camera open failed";
        return;
    }

    this->running = true;
    this->timer.start(33); // 30FPS
}

void CameraService::stopCamera()
{
    this->running = false;
    this->timer.stop();

    if (this->cap.isOpened())
        this->cap.release();

    emit frameCleared();
}

void CameraService::processFrame()
{
    if (!running || !cap.isOpened())
        return;

    if (!cap.read(this->m_originalFrame) || m_originalFrame.empty())
        return;

    cv::cvtColor(this->m_originalFrame, this->m_originalFrame, cv::COLOR_BGR2RGB);

    if(this->detectingFace)
    {
        qDebug() << "detecting face";
    }


    this->m_frame = matToQImage(this->m_originalFrame);

    emit originalFrameChanged();
    emit frameChanged();
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

void CameraService::getCaptureData()
{
    try
    {
        m_camQualities.clear();
        m_videoQualities.clear();
        m_cameraNames.clear();

        const QList<QCameraDevice> cameraDevices = QMediaDevices::videoInputs();

        if (!cameraDevices.isEmpty())
        {
            for(const QCameraDevice& device: cameraDevices)
                m_cameraNames.push_back(device.description());

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
        }
        else
        {
            QMessageBox::information(nullptr, tr("Device Error"),
                                     tr("No camera devices connected.\nPlease connect a camera device."),
                                     QMessageBox::Ok);
        }
    }
    catch (const std::exception &e)
    {
        QMessageBox::information(nullptr, tr("Camera Error"), QString::fromStdString(e.what()), QMessageBox::Ok);
    }
}


void CameraService::applyPhotoQuality(int index)
{
    stopCamera();

    const auto devices = QMediaDevices::videoInputs();
    if (devices.isEmpty()) return;

    const QCameraDevice &device = devices[m_currentCameraIndex];
    QList<QSize> sizes;

    for (const QCameraFormat &f : device.videoFormats())
        if (!sizes.contains(f.resolution()))
            sizes << f.resolution();

    if (index < 0 || index >= sizes.size()) return;

    const QSize size = sizes[index];

    cap.set(cv::CAP_PROP_FRAME_WIDTH, size.width());
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, size.height());

    setCurrentQualityIndex(index);
    startCamera();
}


void CameraService::applyVideoQuality(int formatIndex)
{
    try
    {
        stopCamera();

        const auto devices = QMediaDevices::videoInputs();
        if (devices.isEmpty()) return;
        if (m_currentCameraIndex < 0 || m_currentCameraIndex >= devices.size()) return;

        const QCameraDevice &device = devices[m_currentCameraIndex];
        const auto formats = device.videoFormats();
        if (formatIndex < 0 || formatIndex >= formats.size()) return;

        const QCameraFormat &format = formats[formatIndex];
        const QSize res = format.resolution();
        const double fps = format.maxFrameRate();

        if (running) stopCamera();

        cap.set(cv::CAP_PROP_FRAME_WIDTH, res.width());
        cap.set(cv::CAP_PROP_FRAME_HEIGHT, res.height());
        cap.set(cv::CAP_PROP_FPS, fps);

        QThread::msleep(50);

        if (!cap.open(m_currentCameraIndex, cv::CAP_DSHOW))
        {
            QMessageBox::information(nullptr, tr("Camera Error"), tr("Failed to open camera device."), QMessageBox::Ok);
            return;
        }

        setCurrentVideoQualityIndex(formatIndex);
        startCamera();
    }
    catch (const std::exception &e)
    {
        QMessageBox::information(nullptr, tr("Camera Error"), QString::fromStdString(e.what()), QMessageBox::Ok);
    }
}

void CameraService::takeShot() { qDebug() << "Captured ..."; }
void CameraService::record() { qDebug() << "Capturing ..."; }
void CameraService::scanQR() { qDebug() << "Scanning QR ..."; }

QStringList CameraService::cameraNames() const { return this->m_cameraNames; }

void CameraService::switchCam(int index)
{
    setCurrentCameraIndex(index);
    stopCamera();
    startCamera();
}

int CameraService::currentCameraIndex() const { return this->m_currentCameraIndex; }

void CameraService::setCurrentCameraIndex(int newIndex)
{
    if (this->m_currentCameraIndex == newIndex) return;
    this->m_currentCameraIndex = newIndex;
    emit cameraChanged();
}

QStringList CameraService::camQualities() const { return m_camQualities; }
int CameraService::currentQualityIndex() const { return m_currentQualityIndex; }

void CameraService::setCurrentQualityIndex(int newIndex)
{
    if (m_currentQualityIndex == newIndex) return;
    m_currentQualityIndex = newIndex;
    emit currentQualityIndexChanged();
}

QStringList CameraService::videoQualities() const { return m_videoQualities; }
int CameraService::currentVideoQualityIndex() const { return m_currentVideoQualityIndex; }

void CameraService::setCurrentVideoQualityIndex(int newIndex)
{
    if (m_currentVideoQualityIndex == newIndex) return;
    m_currentVideoQualityIndex = newIndex;
    emit currentVideoQualityIndexChanged();
}

CameraService::Mode CameraService::mode() const { return m_mode; }

void CameraService::setMode(const Mode &newMode)
{
    if (m_mode == newMode) return;
    m_mode = newMode;

    switch (newMode)
    {
        case Photo: applyPhotoQuality(m_currentQualityIndex); break;
        case Video: applyVideoQuality(m_currentQualityIndex); break;
        case QRCode: break;
    }

    emit modeChanged();
}

cv::Mat CameraService::originalFrame() const { return m_originalFrame; }

void CameraService::setOriginalFrame(const cv::Mat &newFrame)
{
    this->m_originalFrame = newFrame;
    this->m_frame = matToQImage(this->m_originalFrame);

    emit frameChanged();
    emit originalFrameChanged();
}
