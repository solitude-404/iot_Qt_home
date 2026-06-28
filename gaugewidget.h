#ifndef GAUGEWIDGET_H
#define GAUGEWIDGET_H

#include <QWidget>
#include <QPainter>

class GaugeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GaugeWidget(QWidget *parent = nullptr);
    void setValue(double value);
    void setRange(double min, double max);
    void setColor(QColor color);
    void setUnit(QString unit);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    double m_value = 0;
    double m_min = 0;
    double m_max = 100;
    QColor m_color = QColor("#4FACFE");
    QString m_unit = "";
};

#endif // GAUGEWIDGET_H