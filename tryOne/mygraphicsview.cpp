#include "mygraphicsview.h"
#include <QWheelEvent>
#include <QPoint>
#include <QDebug>

MyGraphicsView::MyGraphicsView(QWidget *parent)
    : QGraphicsView(parent)
{
    //设置缩放的初始偏移量为1
    m_scalingOffset=1;
    // 连接QGraphicsView的wheelEvent信号到自定义的槽函数
        connect(this, SIGNAL(wheelEvent(QWheelEvent*)), this, SLOT(wheelEvent(QWheelEvent*)));

        // 添加双击事件过滤器
            this->viewport()->installEventFilter(this);
        connect(this, SIGNAL(mousePressEvent(QMouseEvent*)), this, SLOT(mousePressEvent(QMouseEvent*)));
        connect(this, SIGNAL(mouseMoveEvent(QMouseEvent*)), this, SLOT(mouseMoveEvent(QMouseEvent*)));
        connect(this, SIGNAL(mouseReleaseEvent(QMouseEvent*)), this, SLOT(mouseReleaseEvent(QMouseEvent*)));



        //鼠标追踪
        setMouseTracking(true);
        m_mousePressed = false;

}

MyGraphicsView::~MyGraphicsView()
{


}
//函数用于放大图形视图。如果当前的缩放偏移量m_scalingOffset大于1.3，函数直接返回。否则，将缩放偏移量增加0.1，并调用scaling函数进行缩放操作。
void MyGraphicsView::magnify()
{
    if(m_scalingOffset>1.3)
        return;

    m_scalingOffset+=0.1;
    scaling(m_scalingOffset);
}
//函数用于缩小图形视图。如果当前的缩放偏移量m_scalingOffset小于0.9，函数直接返回。否则，将缩放偏移量减去0.1，并调用scaling函数进行缩放操作。
void MyGraphicsView::shrink()
{
    if(m_scalingOffset<0.9)
        return;

    m_scalingOffset-=0.1;
    scaling(m_scalingOffset);
}
//函数用于进行缩放操作。函数接受一个缩放因子scaleFactor作为参数。在函数中，通过调用scale函数对图形视图进行缩放操作，并更新缩放偏移量m_scalingOffset。
void MyGraphicsView::scaling(qreal scaleFactor)
{
    qDebug()<<this->sceneRect();
    scale(scaleFactor,scaleFactor);
    m_scalingOffset = scaleFactor;

}
//鼠标滚轮事件，通过获取滚轮滚动的角度差判断滚轮的滚动方向，如果向上则调用magnify放大，如果向下则调用shrink
void MyGraphicsView::wheelEvent(QWheelEvent *event)
{
    QPoint sroll=event->angleDelta();
    sroll.y()>0?magnify():shrink();

}
//eventfilter是一个时间过滤器函数，过滤特定的事件类型，如果是双击事件则调用重置函数进行双击复原操作
bool MyGraphicsView::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonDblClick) {
        // 处理双击事件
        resetTransform();
        m_scalingOffset = 1;
        return true;
    }
    return QObject::eventFilter(obj, event);
}
void MyGraphicsView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_mousePressed = true;
        m_lastMousePos = event->pos();
    }
    QGraphicsView::mousePressEvent(event);


}

void MyGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    if (m_mousePressed) {
        QPoint currentMousePos = event->pos();
        QPoint delta = currentMousePos - m_lastMousePos;
        QGraphicsView::translate(delta.x(), delta.y());
        m_lastMousePos = currentMousePos;
    }
    QGraphicsView::mouseMoveEvent(event);

}

void MyGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_mousePressed = false;
    }
    QGraphicsView::mouseReleaseEvent(event);

}


