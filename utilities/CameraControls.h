#pragma once

#include <QObject>
#include "controllers/CameraService.h"

class CameraService;

class CameraControls : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool hdrIsActive READ hdrIsActive NOTIFY hdrIsActiveChanged)
    Q_PROPERTY(bool faceDetectorIsActive READ faceDetectorIsActive NOTIFY faceDetectorIsActiveChanged)
    Q_PROPERTY(bool timerIsActive READ timerIsActive NOTIFY timerIsActiveChanged)
    Q_PROPERTY(bool gridIsActive READ gridIsActive NOTIFY gridIsActiveChanged)

    Q_PROPERTY(bool cameraButtonIsActive READ cameraButtonIsActive NOTIFY cameraButtonIsActiveChanged)
    Q_PROPERTY(bool videoButtonIsActive READ videoButtonIsActive NOTIFY videoButtonIsActiveChanged)
    Q_PROPERTY(bool qrButtonIsActive READ qrButtonIsActive NOTIFY qrButtonIsActiveChanged)

    Q_PROPERTY(bool showSettings READ showSettings NOTIFY showSettingsChanged)
    Q_PROPERTY(bool showFilters READ showFilters NOTIFY showFiltersChanged)

public:
    explicit CameraControls(CameraService* camera, QObject* parent = nullptr);

    bool hdrIsActive() const;
    bool faceDetectorIsActive() const;
    bool timerIsActive() const;
    bool gridIsActive() const;

    bool cameraButtonIsActive() const;
    bool videoButtonIsActive() const;
    bool qrButtonIsActive() const;

    Q_INVOKABLE void toggleHDR();
    Q_INVOKABLE void toggleFaceDetector();
    Q_INVOKABLE void toggleTimer();
    Q_INVOKABLE void toggleGrid();
    Q_INVOKABLE void toggleSettings();
    Q_INVOKABLE void toggleFilters();

    Q_INVOKABLE void onCameraPressed();
    Q_INVOKABLE void onVideoPressed();
    Q_INVOKABLE void onQRPressed();

    bool showSettings() const;
    bool showFilters() const;

signals:
    void hdrIsActiveChanged();
    void faceDetectorIsActiveChanged();
    void timerIsActiveChanged();
    void gridIsActiveChanged();

    void cameraButtonIsActiveChanged();
    void videoButtonIsActiveChanged();
    void qrButtonIsActiveChanged();

    void showSettingsChanged();
    void showFiltersChanged();


private:
    void activateCamera();
    void activateVideo();
    void activateQR();

    bool m_hdrIsActive = true;
    bool m_faceDetectorIsActive = false;
    bool m_timerIsActive = false;
    bool m_gridIsActive = true;

    bool m_cameraButtonIsActive = true;
    bool m_videoButtonIsActive = false;
    bool m_qrButtonIsActive = false;

    CameraService* m_camera;
    bool m_showSettings = false;
    bool m_showFilters = false;
};
