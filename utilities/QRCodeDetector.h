#ifndef QRCODEDETECTOR_H
#define QRCODEDETECTOR_H

#include <opencv2/opencv.hpp>
#include <QString>

class QRCodeDetector
{
public:
    QRCodeDetector();
    void detectAndDecode(cv::Mat& frame, QString& decodedString);
};

#endif // QRCODEDETECTOR_H
