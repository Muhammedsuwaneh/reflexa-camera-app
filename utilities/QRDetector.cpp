#include "QRDetector.h"
#include <QDebug>

QRDetector::QRDetector() {}

bool QRDetector::detect(const cv::Mat& frame, QString& decodedString)
{
    try
    {
        decodedString.clear();

        if (frame.empty())
            return false;

        std::vector<cv::Point2f> points;
        std::string decoded = detector.detectAndDecode(frame, points);

        if (decoded.empty())
            return false;

        QString text = QString::fromStdString(decoded);

        if (text == lastDecoded)
            return false;

        lastDecoded = text;
        decodedString = text;

        return true;

    }
    catch(const std::exception& e)
    {
        qDebug() << "[ERROR]: " << e.what();
        return false;
    }
}
