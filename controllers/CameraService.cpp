#include "CameraService.h"
#include <QDebug>
#include <QMediaDevices>
#include <QCameraDevice>
#include <QMessageBox>
#include <opencv2/opencv.hpp>
#include <algorithm>

#include <QDateTime>
#include <QStandardPaths>
#include <QDir>
#include <QDebug>
#include <QString>

CameraService::CameraService(QObject *parent)
    : QObject{parent}
{
    this->m_mediaModel = new MediaListModel(this);
    connect(&timer, &QTimer::timeout, this, &CameraService::processFrame, Qt::DirectConnection);
}

inline QImage matToQImage(const cv::Mat& mat)
{
    if (mat.empty())
        return QImage();

    switch (mat.type())
    {
        case CV_8UC1:
            return QImage(mat.data, mat.cols, mat.rows, mat.step,
                          QImage::Format_Grayscale8).copy();

        case CV_8UC3:
            return QImage(mat.data, mat.cols, mat.rows, mat.step,
                          QImage::Format_BGR888).copy();

        case CV_8UC4:
            return QImage(mat.data, mat.cols, mat.rows, mat.step,
                          QImage::Format_RGBA8888).copy();

        default:
            return QImage();
    }
}

void CameraService::printMediaItems()
{
    qDebug() << "Current media items : " << this->m_mediaModel->rowCount();

    int count = this->m_mediaModel->rowCount();

    if(count <= 0) return;

    for (int row = 0; row < count; ++row)
    {
        QModelIndex index = m_mediaModel->index(row, 0);

        QString filePath = m_mediaModel->data(index, MediaListModel::FilePathRole).toString();
        QString type     = m_mediaModel->data(index, MediaListModel::TypeRole).toString();
        QDateTime created= m_mediaModel->data(index, MediaListModel::CreatedAtRole).toDateTime();

        qDebug() << "[" << row << "]"
                 << "Type:" << type
                 << "| Path:" << filePath
                 << "| Created:" << created.toString(Qt::ISODate);
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
    try
    {
        if (!running || !cap.isOpened())
            return;

        if (!cap.read(this->m_originalFrame) || m_originalFrame.empty())
            return;

        this->m_processedFrame = this->m_originalFrame.clone();

        if(this->detectingFace && this->scanningQRCode == false)
        {
            this->faceDetector.detect(this->m_processedFrame);
        }

        if(this->scanningQRCode && this->detectingFace == false)
        {
            QString decodedText;
            if (qRCodeDetector.detect(this->m_processedFrame, decodedText))
            {
                qDebug() << "QR detected:" << decodedText;

                this->m_qrDetected = decodedText;
                emit qrDetectedChanged();
            }
        }

        if(!this->scanningQRCode)
        {
            applyLiveAdjustments();
            applyLiveFilters();
        }


        if (this->m_capturingVideo && this->writer.isOpened())
        {
            writer.write(this->m_processedFrame);
        }

        this->m_frame = matToQImage(this->m_processedFrame);

        emit originalFrameChanged();
        emit frameChanged();
    }
    catch(const std::exception &e)
    {
        qDebug() << e.what();
    }
}

void CameraService::applyLiveAdjustments()
{
    if (m_brightness != 0) adjustBrightness();
    if (m_contrast != 0)   adjustContrast();
    if (m_saturation != 100) adjustSaturation();
    if (m_exposure != 0)   adjustExposure();
    if (m_grayScale > 0)   adjustGrayScale();
}

void CameraService::applyLiveFilters()
{
    if (this->m_activeFilter == "Gray Scale")
        applyGrayScale();
    else if (this->m_activeFilter == "Negative / Invert")
        applyInvert();
    else if (this->m_activeFilter == "High Contrast")
        applyHighContrast();
    else if (this->m_activeFilter == "Gaussian Blur")
        applyGaussianBlur();
    else if (this->m_activeFilter == "Skin Smoothing")
        applySkinSmoothing();
    else if (this->m_activeFilter == "Sepia (Warm)")
        applySepia();
    else
    {
        this->m_grayScale = 0;
    }
}

void CameraService::adjustBrightness()
{
    if (this->m_processedFrame.empty()) return;

    cv::Mat dst;
    this->m_processedFrame.convertTo(dst, -1, 1.0, this->m_brightness);

    setProcessedFrame(dst);
}

void CameraService::adjustContrast()
{
    if (this->m_processedFrame.empty()) return;

    double alpha = std::clamp(this->m_contrast / 100.0 + 1.0, 0.0, 3.0);

    cv::Mat dst;
    this->m_processedFrame.convertTo(dst, -1, alpha, 0);

    setProcessedFrame(dst);
}

void CameraService::adjustSaturation()
{
    if (this->m_processedFrame.empty() || this->m_processedFrame.channels() < 3) return;

    cv::Mat hsv;
    cv::cvtColor(this->m_processedFrame, hsv, cv::COLOR_BGR2HSV);

    std::vector<cv::Mat> channels;
    cv::split(hsv, channels);

    double scale = std::clamp(this->m_saturation / 100.0, 0.0, 3.0);
    channels[1].convertTo(channels[1], -1, scale, 0);

    cv::merge(channels, hsv);

    cv::Mat dst;
    cv::cvtColor(hsv, dst, cv::COLOR_HSV2BGR);

    setProcessedFrame(dst);
}

void CameraService::adjustExposure()
{
    if (this->m_processedFrame.empty()) return;

    double gamma = std::clamp(this->m_exposure / 100.0 + 1.0, 0.1, 3.0);

    cv::Mat dst;
    cv::Mat f;
    this->m_processedFrame.convertTo(f, CV_32F, 1.0 / 255.0);

    cv::pow(f, 1.0 / gamma, f);
    f *= 255.0;
    f.convertTo(dst, CV_8U);

    setProcessedFrame(dst);
}

void CameraService::adjustGrayScale()
{
    if (m_grayScale <= 0)
        return;

    if (m_processedFrame.empty())
        return;

    cv::Mat gray, grayRGB, blended;

    cv::cvtColor(m_processedFrame, gray, cv::COLOR_RGB2GRAY);
    cv::cvtColor(gray, grayRGB, cv::COLOR_GRAY2RGB);

    double g = std::clamp(m_grayScale / 100.0, 0.0, 1.0);

    cv::addWeighted(
        m_processedFrame, 1.0 - g,
        grayRGB, g,
        0.0,
        blended
        );

    setProcessedFrame(blended);
}

void CameraService::applyGrayScale()
{
    m_grayScale = 100;
    emit grayScaleChanged();
    adjustGrayScale();
}

void CameraService::applyInvert()
{    
    if (this->m_processedFrame.empty()) return;

    this->m_grayScale = 0;

    cv::Mat inverted;
    cv::bitwise_not(this->m_processedFrame, inverted);

    setProcessedFrame(inverted);
}

void CameraService::applyHighContrast()
{   
    if (this->m_processedFrame.empty()) return;

    this->m_grayScale = 0;

    cv::Mat contrast;
    this->m_processedFrame.convertTo(contrast, -1, 1.8, -50);

    setProcessedFrame(contrast);
}

void CameraService::applyGaussianBlur()
{
    if (this->m_processedFrame.empty()) return;

    this->m_grayScale = 0;

    cv::Mat blurred;
    cv::GaussianBlur(this->m_processedFrame, blurred, cv::Size(9, 9), 0);

    setProcessedFrame(blurred);
}

void CameraService::applySkinSmoothing()
{
    if (this->m_processedFrame.empty()) return;

    cv::Mat smooth;
    cv::bilateralFilter(this->m_processedFrame, smooth, 9, 75, 75);

    setProcessedFrame(smooth);
}

void CameraService::applySepia()
{
    if (this->m_processedFrame.empty()) return;

    this->m_grayScale = 0;

    cv::Mat sepia;
    cv::Mat kernel = (cv::Mat_<float>(3,3) <<
                          0.272, 0.534, 0.131,
                      0.349, 0.686, 0.168,
                      0.393, 0.769, 0.189);

    cv::transform(this->m_processedFrame, sepia, kernel);
    sepia.convertTo(sepia, CV_8UC3);

    setProcessedFrame(sepia);
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

            const QSize size = photoSizes[this->m_currentQualityIndex];

            cap.set(cv::CAP_PROP_FRAME_WIDTH, size.width());
            cap.set(cv::CAP_PROP_FRAME_HEIGHT, size.height());

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
    try
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
    catch (const std::exception &e)
    {
        QMessageBox::information(nullptr, tr("Camera Error"), QString::fromStdString(e.what()), QMessageBox::Ok);
    }
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

void CameraService::takeShot()
{
    try
    {
        if(this->m_processedFrame.empty()) return;

        QString picturesDir = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);

        QDir dir(picturesDir + "/ReflexaCamCaptures");
        if (!dir.exists())
            dir.mkpath(".");

        QString fileName =
            dir.filePath("IMG_" +
                         QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss") +
                         ".jpg");

        std::vector<int> params = {
            cv::IMWRITE_JPEG_QUALITY, 95
        };

        cv::Mat frameToSave = this->m_processedFrame.clone();

        bool success = cv::imwrite(fileName.toStdString(), frameToSave, params);

        if (success)
        {
            setRecentCaptured(matToQImage(frameToSave));
            this->m_currentMediaType = "photo";
            emit currentMediaTypeChanged();

            this->m_mediaModel->addMedia(fileName, "photo");
            printMediaItems();
        }
        else
        {
            qDebug() << "Failed to save image";
        }
    }
    catch (const std::exception &e)
    {
        QMessageBox::information(nullptr, tr("Camera Error"), QString::fromStdString(e.what()), QMessageBox::Ok);
    }
}

void CameraService::startVideoCapture()
{
    try
    {
        if(this->m_capturingVideo) return;

        const int width = static_cast<int>(this->cap.get(cv::CAP_PROP_FRAME_WIDTH));
        const int height = static_cast<int>(this->cap.get(cv::CAP_PROP_FRAME_HEIGHT));
        const double fps = this->cap.get(cv::CAP_PROP_FPS) > 0 ? this->cap.get(cv::CAP_PROP_FPS) : 30;

        QString vidDir = QStandardPaths::writableLocation(QStandardPaths::MoviesLocation);

        QDir dir(vidDir + "/ReflexaCamCaptures");
        if (!dir.exists())
            dir.mkpath(".");

        QString filename =
            dir.filePath("VID_" +
                         QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss") +
                         ".avi");

        this->m_lastVideoPath = filename;

        writer.open(filename.toLocal8Bit().constData(), cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), fps, cv::Size(width, height));

        if(!writer.isOpened())
        {
            qDebug() << "Failed to start video writer";
            return;
        }

        this->m_capturingVideo = true;
        emit capturingVideoChanged();
    }
    catch (const std::exception &e)
    {
        qDebug() << e.what();
    }
}

void CameraService::stopVideoCapture()
{
    try
    {
        if (!m_capturingVideo)
            return;

        m_capturingVideo = false;

        if (writer.isOpened())
            writer.release();

        this->m_currentMediaType = "video";
        setRecentCaptured(matToQImage(m_processedFrame.clone()));
        emit capturingVideoChanged();
        emit currentMediaTypeChanged();

        this->m_mediaModel->addMedia(this->m_lastVideoPath, "video");
        printMediaItems();
    }
    catch (const std::exception &e)
    {
        QMessageBox::information(nullptr, tr("Camera Error"), QString::fromStdString(e.what()), QMessageBox::Ok);
    }
}

void CameraService::scanQR()
{
    qDebug() << "Scanning QR ...";
}

QStringList CameraService::cameraNames() const { return this->m_cameraNames; }

void CameraService::switchCam(int index)
{
    setCurrentCameraIndex(index);
    stopCamera();
    startCamera();
}

int CameraService::currentCameraIndex() const
{
    return this->m_currentCameraIndex;
}

void CameraService::setCurrentCameraIndex(int newIndex)
{
    if (this->m_currentCameraIndex == newIndex) return;
    this->m_currentCameraIndex = newIndex;
    emit cameraChanged();
}

QStringList CameraService::camQualities() const
{
    return this->m_camQualities;
}

int CameraService::currentQualityIndex() const
{
    return this->m_currentQualityIndex;
}

void CameraService::setCurrentQualityIndex(int newIndex)
{
    if (this->m_currentQualityIndex == newIndex) return;
    this->m_currentQualityIndex = newIndex;
    emit currentQualityIndexChanged();
}

QStringList CameraService::videoQualities() const
{
    return this->m_videoQualities;
}

int CameraService::currentVideoQualityIndex() const
{
    return this->m_currentVideoQualityIndex;
}

void CameraService::setCurrentVideoQualityIndex(int newIndex)
{
    if (this->m_currentVideoQualityIndex == newIndex) return;
    this->m_currentVideoQualityIndex = newIndex;
    emit currentVideoQualityIndexChanged();
}

CameraService::Mode CameraService::mode() const { return m_mode; }

void CameraService::setMode(const Mode &newMode)
{
    if (this->m_mode == newMode) return;

    this->m_mode = newMode;

    switch (newMode)
    {
        case Photo:
            applyPhotoQuality(this->m_currentQualityIndex);
            this->scanningQRCode = false;
            break;
        case Video:
            applyVideoQuality(this->m_currentQualityIndex);
            this->scanningQRCode = false;
            break;
        case QRCode:
            this->detectingFace = false;
            this->scanningQRCode = true;
            break;
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

int CameraService::brightness() const
{
    return this->m_brightness;
}

void CameraService::setBrightness(int newBrightness)
{
    if (this->m_brightness == newBrightness)
        return;
    this->m_brightness = newBrightness;
    emit brightnessChanged();
}

cv::Mat CameraService::processedFrame() const
{
    return m_processedFrame;
}

void CameraService::setProcessedFrame(const cv::Mat &newProcessedFrame)
{
    this->m_processedFrame = newProcessedFrame;
    emit processedFrameChanged();
}

QString CameraService::activeFilter() const
{
    return this->m_activeFilter;
}

void CameraService::setActiveFilter(const QString &newActiveFilter)
{
    if (this->m_activeFilter == newActiveFilter)
        return;
    this->m_activeFilter = newActiveFilter;
    emit activeFilterChanged();
}

int CameraService::contrast() const
{
    return this->m_contrast;
}

void CameraService::setContrast(int newContrast)
{
    if (this->m_contrast == newContrast)
        return;
    this->m_contrast = newContrast;
    emit contrastChanged();
}

int CameraService::saturation() const
{
    return m_saturation;
}

void CameraService::setSaturation(int newSaturation)
{
    if (this->m_saturation == newSaturation)
        return;
    this->m_saturation = newSaturation;
    emit saturationChanged();
}

int CameraService::exposure() const
{
    return this->m_exposure;
}

void CameraService::setExposure(int newExposure)
{
    if (this->m_exposure == newExposure)
        return;
    this->m_exposure = newExposure;
    emit exposureChanged();
}

int CameraService::grayScale() const
{
    return this->m_grayScale;
}

void CameraService::setGrayScale(int newGrayScale)
{
    if (m_grayScale == newGrayScale)
        return;
    m_grayScale = newGrayScale;
    emit grayScaleChanged();
}

void CameraService::setRecentCaptured(const QImage &newRecentCaptured)
{
    if (this->m_recentCaptured == newRecentCaptured)
        return;
    this->m_recentCaptured = newRecentCaptured;
    emit recentCapturedChanged();
}

bool CameraService::capturingVideo() const
{
    return this->m_capturingVideo;
}

QImage CameraService::recentCaptured() const
{
    return this->m_recentCaptured;
}


CameraService::~CameraService()
{
   qDebug() << "CameraService destructor called";

    if (this->timer.isActive())
    {
        timer.stop();
    }

    if (this->m_capturingVideo)
    {
        this->m_capturingVideo = false;
        emit capturingVideoChanged();
    }

    if (cap.isOpened())
    {
        cap.release();
    }

    this->m_originalFrame.release();
    this->m_processedFrame.release();

    disconnect();
    qDebug() << "CameraService cleaned up safely";
}

QString CameraService::currentMediaType() const
{
    return m_currentMediaType;
}

QString CameraService::qrDetected() const
{
    return m_qrDetected;
}

MediaListModel *CameraService::mediaModel() const
{
    return m_mediaModel;
}
