#include "graphicsviewex.h"
#include <QMouseEvent>

GraphicsViewEx::GraphicsViewEx(QWidget *parent) : QGraphicsView(parent)
{

}

void GraphicsViewEx::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        QPoint point = event->pos();
        emit mouseDoubleClickedPoint(point);
    }
    QGraphicsView::mouseDoubleClickEvent(event);
}

void GraphicsViewEx::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        QPoint point = event->pos();
        emit mousePressPoint(point);
    }

   if(event->button() == Qt::RightButton)//右键单击，弹出面积/长度
    {
       QPoint point = event->pos();
        emit mouse_Right_PressPoint(point);
   }

    QGraphicsView::mousePressEvent(event);
}

void GraphicsViewEx::mouseMoveEvent(QMouseEvent *event)
{
    QPoint point = event->pos();
    emit mouseMovePoint(point);
    QGraphicsView::mouseMoveEvent(event);
}

void GraphicsViewEx::keyPressEvent(QKeyEvent *event)
{
    emit keyPress(event);
    QGraphicsView::keyPressEvent(event);
}
