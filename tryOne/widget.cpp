#include "widget.h"
#include "ui_widget.h"
#include <QGraphicsRectItem>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    m_scene=new QGraphicsScene;
    QGraphicsRectItem *item=new QGraphicsRectItem;
    item->setPos(0,0);
    item->setBrush(QBrush(Qt::red));
    item->setRect(0,0,100,100);
    m_scene->addItem(item);
    ui->graphicsView->setSceneRect(-100,-100,100,100);
    ui->graphicsView->setScene(m_scene);



}

Widget::~Widget()
{
    delete ui;
}
