#ifndef CAMERAWORKER_H
#define CAMERAWORKER_H
#include <QThread>

class CameraWorker : public QThread
{
    Q_OBJECT
public:
    explicit CameraWorker(int cameraIndex, QObject* parent = nullptr);
    void stop();

signals:
    void frameReady(const QImage&);

protected:
    void run() override;

private:
    std::atomic_bool running{true};
    int camIndex;
};

#endif // CAMERAWORKER_H
