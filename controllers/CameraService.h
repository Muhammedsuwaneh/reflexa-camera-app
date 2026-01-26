#ifndef CAMERASERVICE_H
#define CAMERASERVICE_H

#include <QObject>
#include <QImage>
#include <opencv2/opencv.hpp>
#include <QThread>
#include <QTimer>
#include "utilities/FaceDetector.h"

class CameraService : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QImage frame READ frame NOTIFY frameChanged)
    Q_PROPERTY(QImage recentCaptured READ recentCaptured WRITE setRecentCaptured NOTIFY recentCapturedChanged)
    Q_PROPERTY(cv::Mat originalFrame READ originalFrame WRITE setOriginalFrame NOTIFY originalFrameChanged)
    Q_PROPERTY(cv::Mat processedFrame READ processedFrame WRITE setProcessedFrame NOTIFY processedFrameChanged)

    // CAM DEVICES
    Q_PROPERTY(QStringList cameraNames READ cameraNames NOTIFY camerasChanged)
    Q_PROPERTY(int currentCameraIndex READ currentCameraIndex WRITE setCurrentCameraIndex NOTIFY cameraChanged)

    // PHOTO QUALITY
    Q_PROPERTY(QStringList camQualities READ camQualities NOTIFY camQualitiesChanged)
    Q_PROPERTY(int currentQualityIndex READ currentQualityIndex WRITE setCurrentQualityIndex NOTIFY currentQualityIndexChanged)

    // VIDEO QUALITY
    Q_PROPERTY(QStringList videoQualities READ videoQualities NOTIFY videoQualitiesChanged)
    Q_PROPERTY(int currentVideoQualityIndex READ currentVideoQualityIndex WRITE setCurrentVideoQualityIndex NOTIFY currentVideoQualityIndexChanged)

    // TEXTURES
    Q_PROPERTY(int brightness READ brightness WRITE setBrightness NOTIFY brightnessChanged)
    Q_PROPERTY(int contrast READ contrast WRITE setContrast NOTIFY contrastChanged)
    Q_PROPERTY(int saturation READ saturation WRITE setSaturation NOTIFY saturationChanged)
    Q_PROPERTY(int exposure READ exposure WRITE setExposure NOTIFY exposureChanged)
    Q_PROPERTY(int grayScale READ grayScale WRITE setGrayScale NOTIFY grayScaleChanged)

    // FILTERS
    Q_PROPERTY(QString activeFilter READ activeFilter WRITE setActiveFilter NOTIFY activeFilterChanged);

    Q_PROPERTY(bool capturingVideo READ capturingVideo NOTIFY capturingVideoChanged)

public:
    enum Mode {
        Photo = 0,
        Video = 1,
        QRCode = 2
    };
    Q_ENUM(Mode)
    Q_PROPERTY(Mode mode READ mode WRITE setMode NOTIFY modeChanged)

    explicit CameraService(QObject *parent = nullptr);
    ~CameraService();

    QImage frame() const;
    Q_INVOKABLE void setDetectingFace(bool detecting);

    QStringList cameraNames() const;
    Q_INVOKABLE void switchCam(int index);

    Q_INVOKABLE void stopVideoCapture();

    Q_INVOKABLE void applyPhotoQuality(int index);
    Q_INVOKABLE void applyVideoQuality(int index);

    void takeShot();
    void record();
    void scanQR();

    void applyAdjustmentsAndFilters();

    void init();

    int currentCameraIndex() const;
    void setCurrentCameraIndex(int newCurrentCameraIndex);

    QStringList camQualities() const;

    int currentQualityIndex() const;
    void setCurrentQualityIndex(int newCurrentQualityIndex);

    QStringList videoQualities() const;

    int currentVideoQualityIndex() const;
    void setCurrentVideoQualityIndex(int newCurrentVideoQualityIndex);

    Mode mode() const;
    void setMode(const Mode &newMode);

    cv::Mat originalFrame() const;
    void setOriginalFrame(const cv::Mat &newOriginalFrame);

    void startCamera();
    void stopCamera();

    int brightness() const;
    void setBrightness(int newBrightness);

    cv::Mat processedFrame() const;

    void setProcessedFrame(const cv::Mat &newProcessedFrame);

    QString activeFilter() const;
    void setActiveFilter(const QString &newActiveFilter);

    int contrast() const;
    void setContrast(int newContrast);

    int saturation() const;
    void setSaturation(int newSaturation);

    int exposure() const;
    void setExposure(int newExposure);

    int grayScale() const;
    void setGrayScale(int newGrayScale);

    QImage recentCaptured() const;

    void setRecentCaptured(const QImage &newRecentCaptured);
    bool capturingVideo() const;

signals:
    void frameChanged();
    void frameCleared();
    void camerasChanged();
    void cameraChanged();
    void camQualitiesChanged();
    void currentQualityIndexChanged();

    void videoQualitiesChanged();

    void currentVideoQualityIndexChanged();

    void modeChanged();

    void originalFrameChanged();

    void brightnessChanged();

    void processedFrameChanged();

    void activeFilterChanged();

    void contrastChanged();

    void saturationChanged();

    void exposureChanged();

    void grayScaleChanged();

    void recentCapturedChanged();

    void capturingVideoChanged();

private:
    QImage m_frame;
    cv::VideoCapture cap;

    FaceDetector faceDetector;
    bool detectingFace = true;

    QTimer timer;

    void getCaptureData();
    void processFrame();

    QStringList m_cameraNames;
    int m_currentCameraIndex = 0;
    QStringList m_camQualities;
    int m_currentQualityIndex = 0;
    QStringList m_videoQualities;
    int m_currentVideoQualityIndex = 0;
    Mode m_mode = Photo;
    cv::Mat m_originalFrame;

    std::atomic_bool running { false };

    // TEXTURES & FILTERS
    void applyLiveAdjustments();
    void applyLiveFilters();

    int m_brightness = 0;
    int m_contrast   = 0;
    int m_saturation = 100;
    int m_exposure   = 0;
    int m_grayScale  = 0;

    cv::Mat m_processedFrame;

    void adjustBrightness();
    void adjustContrast();
    void adjustSaturation();
    void adjustExposure();
    void adjustGrayScale();

    void applyGrayScale();
    void applyInvert();
    void applyHighContrast();
    void applyGaussianBlur();
    void applySkinSmoothing();
    void applySepia();

    QString m_activeFilter = "";
    QImage m_recentCaptured;
    bool m_capturingVideo = false;
};

#endif // CAMERASERVICE_H
