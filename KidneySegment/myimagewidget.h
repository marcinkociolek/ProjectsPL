#ifndef MYIMAGEWIDGET_H
#define MYIMAGEWIDGET_H

#include <QWidget>
#include <opencv2/core/core.hpp>

class MyImageWidget : public QWidget
{
    Q_OBJECT
public:
    int x;
    int y;
    cv::Mat Im;
    explicit MyImageWidget(QWidget *parent = 0);
    void paintEvent(QPaintEvent *event);
    void paintBitmap(cv::Mat Im);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
signals:
    void on_mousePressed(QPoint point, int butPressed);
    void on_mouseMove(QPoint point, int butPressed);
    //void mouseMoveEvent(QMouseEvent *event);
public slots:
        //void on_mousePressed(QPoint point);
};

#endif // MYIMAGEWIDGET_H
