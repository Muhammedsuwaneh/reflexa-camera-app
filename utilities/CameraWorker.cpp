#include "CameraWorker.h"
#include <opencv2/opencv.hpp>
#include <Windows.h>
#include <QImage>

CameraWorker::CameraWorker(int cameraIndex, QObject* parent)
    : QThread(parent), camIndex(cameraIndex) {}

void CameraWorker::stop()
{
    running = false;
}

void CameraWorker::run()
{
    CoInitializeEx(nullptr, COINIT_MULTITHREADED);

    cv::VideoCapture cap;
    if (!cap.open(camIndex, cv::CAP_DSHOW))
    {
        CoUninitialize();
        return;
    }

    cv::Mat frame;
    while (running)
    {
        if (!cap.read(frame) || frame.empty())
            continue;

        cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);

        QImage img(frame.data, frame.cols, frame.rows,
                   frame.step, QImage::Format_RGB888);

        emit frameReady(img.copy());
        msleep(15);
    }

    cap.release();
    CoUninitialize();
}
