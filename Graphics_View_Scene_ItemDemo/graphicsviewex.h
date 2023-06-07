#ifndef GRAPHICSVIEWEX_H
#define GRAPHICSVIEWEX_H
#include <QWidget>
#include <QGraphicsView>



//重载：获取鼠标键盘事件
class GraphicsViewEx : public QGraphicsView
{
    Q_OBJECT
public:
    explicit GraphicsViewEx(QWidget *parent = nullptr);

protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

signals:
    void mouseMovePoint(QPoint point);
    void mouseDoubleClickedPoint(QPoint point);
    void mousePressPoint(QPoint point);
    void mouse_Right_PressPoint(QPoint point);//右键单击o
    void keyPress(QKeyEvent *event);

};

#endif // GRAPHICSVIEWEX_H
