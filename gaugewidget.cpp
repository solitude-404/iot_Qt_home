#include "GaugeWidget.h"

GaugeWidget::GaugeWidget(QWidget *parent)
    : QWidget(parent)
{
}

void GaugeWidget::setValue(double value)
{
    m_value = value;
    update(); // 触发重绘
}

void GaugeWidget::setRange(double min, double max)
{
    m_min = min;
    m_max = max;
    update();
}

void GaugeWidget::setColor(QColor color)
{
    m_color = color;
    update();
}

void GaugeWidget::setUnit(QString unit)
{
    m_unit = unit;
    update();
}

void GaugeWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    int w = width();
    int h = height();
    int side = qMin(w, h);
    QRectF rect((w - side) / 2, (h - side) / 2, side, side);

    // 背景圆弧
    p.setPen(QPen(QColor("#E0E0E0"), 8));
    p.drawArc(rect.adjusted(10, 10, -10, -10), 135 * 16, 270 * 16);

    // 进度圆弧
    p.setPen(QPen(m_color, 8));
    double percent = (m_value - m_min) / (m_max - m_min);
    if (percent < 0) percent = 0;
    if (percent > 1) percent = 1;
    p.drawArc(rect.adjusted(10, 10, -10, -10), 135 * 16, -270 * 16 * percent);

    // 指针
    p.setPen(Qt::NoPen);
    p.setBrush(m_color);
    double angle = qDegreesToRadians(135 - 270 * percent);
    int len = side * 0.35;
    int x = w / 2 + len * qCos(angle);
    int y = h / 2 - len * qSin(angle);
    p.drawEllipse(QPoint(x, y), 6, 6);

    // 数值
    QFont f = p.font();
    f.setBold(true);
    f.setPixelSize(22);
    p.setFont(f);
    p.setPen(Qt::black);
    p.drawText(rect, Qt::AlignCenter, QString::number(m_value, 'f', 1));

    // 单位
    f.setPixelSize(12);
    p.setFont(f);
    p.setPen(Qt::darkGray);
    p.drawText(rect.adjusted(0, 40, 0, 0), Qt::AlignCenter, m_unit);
}