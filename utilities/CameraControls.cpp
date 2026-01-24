#include "CameraControls.h"

CameraControls::CameraControls(CameraService* camera, QObject *parent)
    : QObject{parent}, m_camera(camera)
{}

bool CameraControls::hdrIsActive() const { return m_hdrIsActive; }
bool CameraControls::faceDetectorIsActive() const { return m_faceDetectorIsActive; }
bool CameraControls::timerIsActive() const { return m_timerIsActive; }
bool CameraControls::gridIsActive() const { return m_gridIsActive; }

bool CameraControls::cameraButtonIsActive() const { return m_cameraButtonIsActive; }
bool CameraControls::videoButtonIsActive() const { return m_videoButtonIsActive; }
bool CameraControls::qrButtonIsActive() const { return m_qrButtonIsActive; }

void CameraControls::toggleHDR()
{
    m_hdrIsActive = !m_hdrIsActive;

    // call camera service
    //camera->setHdr(m_hdrIsActive);

    emit hdrIsActiveChanged();
}

void CameraControls::toggleFaceDetector()
{
    m_faceDetectorIsActive = !m_faceDetectorIsActive;
    emit faceDetectorIsActiveChanged();

    // camera->setDetectingFace(m_faceDetectorIsActive);
}

void CameraControls::toggleTimer()
{
    m_timerIsActive = !m_timerIsActive;
    emit timerIsActiveChanged();
}

void CameraControls::toggleGrid()
{
    m_gridIsActive = !m_gridIsActive;
    emit gridIsActiveChanged();
}

void CameraControls::toggleSettings()
{
    this->m_showSettings = !this->m_showSettings;
    emit showSettingsChanged();
}

void CameraControls::toggleFilter()
{
    this->m_showFilters = !this->m_showFilters;
    emit showFiltersChanged();
}

void CameraControls::activateCamera()
{
    m_cameraButtonIsActive = true;
    m_videoButtonIsActive = false;
    m_qrButtonIsActive = false;

    emit cameraButtonIsActiveChanged();
    emit videoButtonIsActiveChanged();
    emit qrButtonIsActiveChanged();
}

void CameraControls::activateVideo()
{
    m_cameraButtonIsActive = false;
    m_videoButtonIsActive = true;
    m_qrButtonIsActive = false;

    emit cameraButtonIsActiveChanged();
    emit videoButtonIsActiveChanged();
    emit qrButtonIsActiveChanged();
}

void CameraControls::activateQR()
{
    m_cameraButtonIsActive = false;
    m_videoButtonIsActive = false;
    m_qrButtonIsActive = true;

    emit cameraButtonIsActiveChanged();
    emit videoButtonIsActiveChanged();
    emit qrButtonIsActiveChanged();
}

void CameraControls::onCameraPressed()
{
    if (m_cameraButtonIsActive) {
        // Take photo
        //this->m_camera->takeShot();
    } else {
        activateCamera();
    }
}

void CameraControls::onVideoPressed()
{
    if (m_videoButtonIsActive) {
        // Start / stop recording
        //this->m_camera->record();
    } else {
        activateVideo();
    }
}

void CameraControls::onQRPressed()
{
    if (m_qrButtonIsActive) {
        // Scan QR
        //this->m_camera->scanQR();
    } else {
        activateQR();
    }
}

bool CameraControls::showSettings() const
{
    return m_showSettings;
}

bool CameraControls::showFilters() const
{
    return m_showFilters;
}
