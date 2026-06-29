#ifndef CAMERAWIDGET_H
#define CAMERAWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPixmap>
#include <QCamera>
#include <QMediaCaptureSession>
#include <QVideoWidget>
#include <QComboBox>
#include <QCameraDevice>
#include <QMediaDevices>

class CameraWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CameraWidget(QWidget *parent = nullptr);
    ~CameraWidget();

signals:
    void backToHome();
    void doorOpened();

private slots:
    void openDoor();
    void hangUp();
    void startCamera();
    void stopCamera();
    void onCameraChanged(int index);

private:
    QCamera *camera;
    QMediaCaptureSession *captureSession;
    QVideoWidget *videoWidget;
    QComboBox *cameraSelector;
    QLabel *statusLabel;
    QPushButton *btnOpenDoor;
    QPushButton *btnHangUp;
    QPushButton *btnStart;
    QPushButton *btnStop;
    QPushButton *btnBack;
    bool isCameraActive;

};

#endif // CAMERAWIDGET_H