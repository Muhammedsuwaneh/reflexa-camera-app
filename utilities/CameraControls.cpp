#include "CameraControls.h"
#include <QDebug>

CameraControls::CameraControls(CameraService* camera, QObject *parent) : QObject{parent}, m_camera(camera) {}

bool CameraControls::hdrIsActive() const
{
    return this->m_hdrIsActive;
}

bool CameraControls::faceDetectorIsActive() const
{
    return this->m_faceDetectorIsActive;
}

bool CameraControls::timerIsActive() const
{
    return this->m_timerIsActive;
}

bool CameraControls::gridIsActive() const
{
    return this->m_gridIsActive;
}

bool CameraControls::cameraButtonIsActive() const
{
    return this->m_cameraButtonIsActive;
}

bool CameraControls::videoButtonIsActive() const
{
    return this->m_videoButtonIsActive;
}

bool CameraControls::qrButtonIsActive() const
{
    return this->m_qrButtonIsActive;
}

void CameraControls::toggleHDR()
{
    this->m_hdrIsActive = !this->m_hdrIsActive;
    emit hdrIsActiveChanged();
}

void CameraControls::toggleFaceDetector()
{
    this->m_faceDetectorIsActive = !this->m_faceDetectorIsActive;
    this->m_camera->setDetectingFace(this->m_faceDetectorIsActive);
    emit faceDetectorIsActiveChanged();
}

void CameraControls::toggleTimer()
{
    this->m_timerIsActive = !this->m_timerIsActive;
    emit timerIsActiveChanged();
}

void CameraControls::toggleGrid()
{
    this->m_gridIsActive = !this->m_gridIsActive;
    emit gridIsActiveChanged();
}

void CameraControls::toggleSettings()
{
    this->m_showSettings = !this->m_showSettings;
    emit showSettingsChanged();
}

void CameraControls::toggleFilters()
{
    this->m_showFilters = !this->m_showFilters;
    emit showFiltersChanged();
}

void CameraControls::activateCamera()
{
    if(!this->m_cameraButtonIsActive)
    {
        this->m_cameraButtonIsActive = true;
        this->m_videoButtonIsActive = false;
        this->m_qrButtonIsActive = false;

        this->m_camera->setMode(CameraService::Photo);

        emit cameraButtonIsActiveChanged();
        emit videoButtonIsActiveChanged();
        emit qrButtonIsActiveChanged();
    }
}

void CameraControls::activateVideo()
{
    if(!this->m_videoButtonIsActive)
    {
        this->m_camera->setMode(CameraService::Video);

        this->m_cameraButtonIsActive = false;
        this->m_videoButtonIsActive = true;
        this->m_qrButtonIsActive = false;

        emit cameraButtonIsActiveChanged();
        emit videoButtonIsActiveChanged();
        emit qrButtonIsActiveChanged();
    }
}

void CameraControls::activateQR()
{
    if(!this->m_qrButtonIsActive)
    {
        this->m_camera->setMode(CameraService::QRCode);

        this->m_cameraButtonIsActive = false;
        this->m_videoButtonIsActive = false;
        this->m_qrButtonIsActive = true;
        this->m_faceDetectorIsActive = false;

        emit cameraButtonIsActiveChanged();
        emit videoButtonIsActiveChanged();
        emit qrButtonIsActiveChanged();
    }
}

void CameraControls::onCameraPressed()
{
    if (this->m_camera->mode() != CameraService::Photo)
    {
        activateCamera();
        return;
    }

    this->m_camera->takeShot();
}

void CameraControls::onVideoPressed()
{
    if (this->m_camera->mode() != CameraService::Video)
    {
        activateVideo();
        return;
    }

    this->m_camera->startVideoCapture();
}

void CameraControls::onQRPressed()
{
    if (this->m_camera->mode() != CameraService::QRCode)
    {
        activateQR();
        return;
    }
}

bool CameraControls::showSettings() const
{
    return this->m_showSettings;
}

bool CameraControls::showFilters() const
{
    return this->m_showFilters;
}
