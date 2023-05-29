#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include <QObject>
#include <QGraphicsView>

QT_BEGIN_NAMESPACE
class QWheelEvent;
QT_END_NAMESPACE

class MyGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit MyGraphicsView(QWidget *parent = 0);
    ~MyGraphicsView();
signals:
    void pressed(QMouseEvent *event);  // 鼠标按下信号
    void moved(QMouseEvent *event);    // 鼠标移动信号
    void released(QMouseEvent *event); // 鼠标释放信号


protected:

    void magnify();
    void shrink();
    void scaling(qreal scaleFactor);
    bool eventFilter(QObject *obj, QEvent *event) override; // 添加事件过滤器声明
    void mousePressEvent(QMouseEvent *event) override;
       void mouseMoveEvent(QMouseEvent *event) override;
       void mouseReleaseEvent(QMouseEvent *event) override;

public slots:
     void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;



private:
    qreal m_scalingOffset;
    bool m_mousePressed;  // 鼠标是否按下
      QPoint m_lastMousePos;  // 上一次鼠标位置

};

#endif // MYGRAPHICSVIEW_H
