#ifndef QRDETECTOR_H
#define QRDETECTOR_H

#pragma once
#include <opencv2/opencv.hpp>
#include <QString>

class QRDetector
{
public:
    QRDetector();
    bool detect(const cv::Mat& frame, QString& decodedString);

private:
    cv::QRCodeDetector detector;
    QString lastDecoded;
};


#endif // QRDETECTOR_H
