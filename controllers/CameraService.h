#ifndef CAMERASERVICE_H
#define CAMERASERVICE_H

#include <QObject>
#include <QImage>
#include <opencv2/opencv.hpp>
#include <QThread>
#include <QTimer>

class CameraService : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QImage frame READ frame NOTIFY frameChanged)
    Q_PROPERTY(cv::Mat originalFrame READ originalFrame WRITE setOriginalFrame NOTIFY originalFrameChanged)

    // CAM DEVICES
    Q_PROPERTY(QStringList cameraNames READ cameraNames NOTIFY camerasChanged)
    Q_PROPERTY(int currentCameraIndex READ currentCameraIndex WRITE setCurrentCameraIndex NOTIFY cameraChanged)

    // PHOTO QUALITY
    Q_PROPERTY(QStringList camQualities READ camQualities NOTIFY camQualitiesChanged)
    Q_PROPERTY(int currentQualityIndex READ currentQualityIndex WRITE setCurrentQualityIndex NOTIFY currentQualityIndexChanged)

    // VIDEO QUALITY
    Q_PROPERTY(QStringList videoQualities READ videoQualities NOTIFY videoQualitiesChanged)
    Q_PROPERTY(int currentVideoQualityIndex READ currentVideoQualityIndex WRITE setCurrentVideoQualityIndex NOTIFY currentVideoQualityIndexChanged)

public:
    enum Mode {
        Photo = 0,
        Video = 1,
        QRCode = 2
    };
    Q_ENUM(Mode)
    Q_PROPERTY(Mode mode READ mode WRITE setMode NOTIFY modeChanged)

    explicit CameraService(QObject *parent = nullptr);

    QImage frame() const;
    Q_INVOKABLE void setDetectingFace(bool detecting);

    QStringList cameraNames() const;
    Q_INVOKABLE void switchCam(int index);

    Q_INVOKABLE void applyPhotoQuality(int index);
    Q_INVOKABLE void applyVideoQuality(int index);

    void takeShot();
    void record();
    void scanQR();

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

private:
    QImage m_frame;
    cv::VideoCapture cap;

    //FaceDetector faceDetector;
    bool detectingFace = false;

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
};

#endif // CAMERASERVICE_H
