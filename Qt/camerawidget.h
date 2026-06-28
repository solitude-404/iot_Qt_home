#ifndef CAMERAWIDGET_H
#define CAMERAWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPixmap>

class CameraWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CameraWidget(QWidget *parent = nullptr);

signals:
    void backToHome();

private slots:
    void openDoor();
    void hangUp();

private:
    QLabel *imgMonitor;
    QPushButton *btnOpenDoor;
    QPushButton *btnHangUp;
    QPushButton *btnBack;
};

#endif // CAMERAWIDGET_H