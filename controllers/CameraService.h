#ifndef CAMERASERVICE_H
#define CAMERASERVICE_H

#include <QObject>
#include <QImage>
#include <opencv2/opencv.hpp>

class CameraService : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QImage frame READ frame NOTIFY frameChanged)
public:
    explicit CameraService(QObject *parent = nullptr);

    QImage frame() const;
    Q_INVOKABLE void setDetectingFace(bool detecting);

signals:
    void frameChanged();
    void frameCleared();

private:
    QImage m_frame;
    cv::VideoCapture cap;
    bool running = false;

    //FaceDetector faceDetector;
    bool detectingFace = false;

    QTimer* timer;

    void setCameraDefaults();
    void startCamera();
    void stopCamera();
    void processFrame();
};

#endif // CAMERASERVICE_H
