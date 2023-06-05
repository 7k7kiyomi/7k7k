#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QGraphicsEllipseItem>
#include <QGraphicsItem>
#include <QTime>
#include <QInputDialog>
#include <QColorDialog>
#include <QFontDialog>
#include <QKeyEvent>
#include <QPrinter>
#include <QPrintDialog>
#include <QDialog>
#include <QMessageBox>
#include <QString>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initMainWindow();
//    QString str;

//    str="矩形数量："+QString("%1").arg(RectNums);
//    //统计对话框：各类几何的数量、颜色
//    connect(ui->action_Statistics,&QAction::triggered,[=](){

//    QMessageBox::about(this,"统计信息",str);


//    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initMainWindow()
{
    //添加状态栏标签
    labViewCord = new QLabel("View坐标: ");
    labViewCord->setMinimumWidth(150);
    ui->statusbar->addWidget(labViewCord);

    labSceneCord = new QLabel("Scene坐标: ");
    labSceneCord->setMinimumWidth(150);
    ui->statusbar->addWidget(labSceneCord);

    labItemCord = new QLabel("Item坐标: ");
    labItemCord->setMinimumWidth(150);
    ui->statusbar->addWidget(labItemCord);

    labItemInfo = new QLabel("Item信息: ");
    labItemInfo->setMinimumWidth(150);
    ui->statusbar->addWidget(labItemInfo);

    //绑定场景和视图
    QRectF rect(-ui->View->width()/2, -ui->View->height()/2, ui->View->width(), ui->View->height());
    scene = new QGraphicsScene(rect);
    ui->View->setScene(scene);
    ui->View->setCursor(Qt::CrossCursor);
    ui->View->setMouseTracking(true);
    //设置橡皮筋模式
    ui->View->setDragMode(QGraphicsView::RubberBandDrag);
    setCentralWidget(ui->View);

    //    QGraphicsRectItem* rectItem = new QGraphicsRectItem(rect);
    //    rectItem->setFlags(QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemIsSelectable);
    //    QPen pen;
    //    pen.setWidth(2);
    //    pen.setColor(Qt::blue);
    //    rectItem->setPen(pen);
    //    scene->addItem(rectItem);

    connect(ui->View, SIGNAL(mouseMovePoint(QPoint)), this, SLOT(on_mouseMove(QPoint)));
    connect(ui->View, SIGNAL(mouseDoubleClickedPoint(QPoint)), this, SLOT(on_mouseDoubleClicked(QPoint)));
    connect(ui->View, SIGNAL(mousePressPoint(QPoint)), this, SLOT(on_mousePress(QPoint)));
    connect(ui->View, SIGNAL(keyPress(QKeyEvent*)), this, SLOT(on_keyPress(QKeyEvent*)));

    connect(ui->View, SIGNAL(mouse_Right_PressPoint(QPoint)), this, SLOT(on_mouse_Right_Press(QPoint)));//右键单击
    //随机数初始化
    qsrand((uint)QTime::currentTime().second());
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QRectF rect(-ui->View->width()/2, -ui->View->height()/2, ui->View->width(), ui->View->height());
    scene->setSceneRect(rect);
}

void MainWindow::on_mouse_Right_Press(QPoint point)//右键单击o
{
    QPointF pointScene = ui->View->mapToScene(point); //转换到Scene坐标
    QGraphicsItem *item = nullptr;
    item = scene->itemAt(pointScene, ui->View->transform()); //获取光标下的绘图项

}

void MainWindow::on_mousePress(QPoint point)
{
    QPointF pointScene = ui->View->mapToScene(point); //转换到Scene坐标
    QGraphicsItem *item = nullptr;
    item = scene->itemAt(pointScene, ui->View->transform()); //获取光标下的绘图项
    if (item != nullptr)
    {
        QPointF pointItem = item->mapFromScene(pointScene); //转换为绘图项的局部坐标
        labItemCord->setText(QString::asprintf("Item 坐标：%.0f,%.0f",pointItem.x(),pointItem.y()));
        labItemInfo->setText(item->data(ItemDes).toString()+", ItemId="+item->data(ItemId).toString());
    }
}

template<class T> void setBrushColor(T *item)
{//函数模板
    QColor color = item->brush().color();
    color = QColorDialog::getColor(color,nullptr,"选择填充颜色");
    if (color.isValid())
        item->setBrush(QBrush(color));
    qDebug()<<color.Rgb;
    qDebug()<<color;//输出选中颜色
    QString c_str=color.name();
    QColor color1;
    color1.setNamedColor(c_str);
    qDebug()<<color1;
}

void MainWindow::on_mouseDoubleClicked(QPoint point)
{
    QPointF pointScene = ui->View->mapToScene(point); //转换到Scene坐标
    QGraphicsItem  *item = nullptr;
    item=scene->itemAt(pointScene,ui->View->transform()); //获取光标下的绘图项

    if (item == nullptr)
        return;

    switch (item->type())  //绘图项的类型
    {
    case QGraphicsRectItem::Type: //矩形框
    {
        QGraphicsRectItem *theItem = qgraphicsitem_cast<QGraphicsRectItem*>(item);
        setBrushColor(theItem);
        break;
    }
    case QGraphicsEllipseItem::Type: //椭圆和圆都是 QGraphicsEllipseItem
    {
        QGraphicsEllipseItem *theItem;
        theItem = qgraphicsitem_cast<QGraphicsEllipseItem*>(item);
        setBrushColor(theItem);
        break;
    }

    case QGraphicsPolygonItem::Type: //梯形和三角形
    {
        QGraphicsPolygonItem *theItem = qgraphicsitem_cast<QGraphicsPolygonItem*>(item);
        setBrushColor(theItem);
        break;
    }
    case QGraphicsLineItem::Type: //直线，设置线条颜色
    {
        QGraphicsLineItem *theItem = qgraphicsitem_cast<QGraphicsLineItem*>(item);
        QPen pen = theItem->pen();
        QColor color = theItem->pen().color();
        color = QColorDialog::getColor(color,this,"选择线条颜色");
        if (color.isValid())
        {
            pen.setColor(color);
            theItem->setPen(pen);
        }
        qDebug()<<color;//输出选中颜色
        break;
    }
//    case QGraphicsTextItem::Type: //文字，设置字体
//    {
//        QGraphicsTextItem *theItem = qgraphicsitem_cast<QGraphicsTextItem*>(item);
//        QFont font = theItem->font();
//        bool ok = false;
//        font = QFontDialog::getFont(&ok,font,this,"设置字体");
//        if (ok)
//            theItem->setFont(font);
//        break;
//    }
    }
}

void MainWindow::on_mouseMove(QPoint point)
{
    labViewCord->setText(QString::asprintf("View坐标：%d,%d",point.x(),point.y()));
    QPointF pointScene = ui->View->mapToScene(point);
    labSceneCord->setText(QString::asprintf("Scene 坐标：%.0f,%.0f",pointScene.x(),pointScene.y()));
}

void MainWindow::on_keyPress(QKeyEvent *event)
{
    if (scene->selectedItems().count() != 1)
        return;

    QGraphicsItem *item = scene->selectedItems().at(0);

    if (event->key() == Qt::Key_Delete)//删除
        scene->removeItem(item);
    else if (event->key() == Qt::Key_Space) //顺时针旋转90度
        item->setRotation(90+item->rotation());
    else if (event->key() == Qt::Key_PageUp)//放大
        item->setScale(item->scale() * 1.1);
    else if (event->key() == Qt::Key_PageDown) //缩小
        item->setScale(item->scale() * 0.9);
    else if (event->key() == Qt::Key_Left)  //左移
        item->setX(-1+item->x());
    else if (event->key() == Qt::Key_Right) //右移
        item->setX(1+item->x());
    else if (event->key() == Qt::Key_Up) //上移
        item->setY(-1+item->y());
    else if (event->key() == Qt::Key_Down) //下移
        item->setY(1+item->y());
}

//创建椭圆
void MainWindow::on_actItem_Ellipse_triggered()
{
    QGraphicsEllipseItem *item = new QGraphicsEllipseItem(-50, -30, 100, 60);

    item->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
    //设置填充颜色
    item->setBrush(QBrush(Qt::blue));
    //设置叠放顺序
    item->setZValue(++frontZ);
    //设置初始位置
    item->setPos(-50 + (qrand() % 100), -50 + (qrand() % 100));
    //设置自定义的附加数据
    item->setData(ItemId, ++seqNum);
    item->setData(ItemDes, "椭圆");
    EllipseNums++;

    scene->addItem(item);
    scene->clearSelection();
    item->setSelected(true);
}

//绘制矩形
void MainWindow::on_actItem_Rect_triggered()
{
    QGraphicsRectItem   *item=new QGraphicsRectItem(-50,-25,100,50);

    item->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
    item->setBrush(QBrush(Qt::yellow));
    item->setZValue(++frontZ);
    item->setPos(-50+(qrand() % 100),-50+(qrand() % 100));

    item->setData(ItemId,++seqNum);
    item->setData(ItemDes,"矩形");
    RectNums++;
    qDebug()<<RectNums;

    scene->addItem(item);
    scene->clearSelection();
    item->setSelected(true);
}

//绘制圆形
void MainWindow::on_actItem_Circle_triggered()
{
    QGraphicsEllipseItem   *item=new QGraphicsEllipseItem(-50,-50,100,100);
    item->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
    item->setBrush(QBrush(Qt::cyan));
    item->setZValue(++frontZ);
    item->setPos(-50+(qrand() % 100),-50+(qrand() % 100));

    item->setData(ItemId,++seqNum);
    item->setData(ItemDes,"圆形");
    CircleNums++;

    scene->addItem(item);
    scene->clearSelection();
    item->setSelected(true);
}

//绘制三角形
void MainWindow::on_actItem_Triangle_triggered()
{
    QGraphicsPolygonItem   *item=new QGraphicsPolygonItem;
    QPolygonF   points;
    points.append(QPointF(0,-40));
    points.append(QPointF(60,40));
    points.append(QPointF(-60,40));

    item->setPolygon(points);
    item->setPos(-50+(qrand() % 100),-50+(qrand() % 100));

    item->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
    item->setBrush(QBrush(Qt::magenta));
    item->setZValue(++frontZ);

    item->setData(ItemId,++seqNum);
    item->setData(ItemDes,"三角形");
    TriangleNums++;

    scene->addItem(item);
    scene->clearSelection();
    item->setSelected(true);
}

//绘制梯形
void MainWindow::on_actItem_Polygon_triggered()
{
    QGraphicsPolygonItem   *item=new QGraphicsPolygonItem;

    QPolygonF   points;
    points.append(QPointF(-40,-40));
    points.append(QPointF(40,-40));
    points.append(QPointF(100,40));
    points.append(QPointF(-100,40));
    item->setPolygon(points);
    item->setPos(-50+(qrand() % 100),-50+(qrand() % 100));

    item->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
    item->setBrush(QBrush(Qt::green));
    item->setZValue(++frontZ);

    item->setData(ItemId,++seqNum);
    item->setData(ItemDes,"梯形");
    PolygonNums++;


    scene->addItem(item);
    scene->clearSelection();
    item->setSelected(true);
}

//绘制直线
void MainWindow::on_actItem_Line_triggered()
{
    QGraphicsLineItem   *item=new QGraphicsLineItem(-100,0,100,0);
    item->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);

    QPen pen(Qt::red);
    pen.setWidth(3);
    item->setPen(pen);

    item->setZValue(++frontZ);
    item->setPos(-50+(qrand() % 100),-50+(qrand() % 100));

    item->setData(ItemId,++seqNum);
    item->setData(ItemDes,"直线");
    LineNums++;

    scene->addItem(item);
    scene->clearSelection();
    item->setSelected(true);
}

//绘制文字
//void MainWindow::on_actItem_Text_triggered()
//{
//    QString str = QInputDialog::getText(this,"输入文字","请输入文字");
//    if (str.isEmpty())
//        return;

//    QGraphicsTextItem *item = new QGraphicsTextItem(str);

//    QFont font=this->font();
//    font.setPointSize(20);
//    font.setBold(true);
//    item->setFont(font);

//    item->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
//    item->setPos(-50+(qrand() % 100),-50+(qrand() % 100));
//    item->setZValue(++frontZ);

//    item->setData(ItemId,++seqNum);
//    item->setData(ItemDes,"文字");

//    scene->addItem(item);
//    scene->clearSelection();
//    item->setSelected(true);
//}

//放大
void MainWindow::on_actZoomIn_triggered()
{
    int nCount = scene->selectedItems().count();
    if(1 == nCount)
    {
        QGraphicsItem* item = scene->selectedItems().at(0);
        item->setScale(0.1 + item->scale());
    }
    else
    {
        ui->View->scale(1.1, 1.1);
    }
}

//缩小
void MainWindow::on_actZoomOut_triggered()
{
    int nCount = scene->selectedItems().count();
    if(1 == nCount)
    {
        QGraphicsItem* item = scene->selectedItems().at(0);
        item->setScale(item->scale() * 0.9);
    }
    else
    {
        ui->View->scale(0.9, 0.9);
    }
}

//左旋转
void MainWindow::on_actRotateLeft_triggered()
{
    int nCount = scene->selectedItems().count();
    if(1 == nCount)
    {
        QGraphicsItem* item = scene->selectedItems().at(0);
        item->setRotation(item->rotation() - 30);
    }
    else
    {
        ui->View->rotate(-30);
    }
}

//右旋转
void MainWindow::on_actRotateRight_triggered()
{
    int nCount = scene->selectedItems().count();
    if(1 == nCount)
    {
        QGraphicsItem* item = scene->selectedItems().at(0);
        item->setRotation(item->rotation() + 30);
    }
    else
    {
        ui->View->rotate(30);
    }
}

//恢复原始坐标变换
void MainWindow::on_actRestore_triggered()
{
    int nCount = scene->selectedItems().count();
    if(1 == nCount)
    {
        QGraphicsItem* item = scene->selectedItems().at(0);
        //item->resetTransform();
        item->setRotation(0);
        item->setScale(1.0);
    }
    else
    {
        ui->View->resetTransform();
    }
}

//前置
void MainWindow::on_actEdit_Front_triggered()
{
    int nCount = scene->selectedItems().count();
    if(nCount > 0)
    {
        QGraphicsItem* item = scene->selectedItems().at(0);
        item->setZValue(++frontZ);
    }
}

//后置
void MainWindow::on_actEdit_Back_triggered()
{
    int nCount = scene->selectedItems().count();
    if(nCount > 0)
    {
        QGraphicsItem* item = scene->selectedItems().at(0);
        item->setZValue(--backZ);
    }
}

//组合
void MainWindow::on_actGroup_triggered()
{
    int cnt = scene->selectedItems().count();
    if (cnt >  1)
    {
        //创建组合并添加到场景中
        QGraphicsItemGroup* group = new QGraphicsItemGroup;
        scene->addItem(group);

        for (int i = 0;i < cnt;i++)
        {
            //注意此处的at(0)
            QGraphicsItem* item = scene->selectedItems().at(0);
            item->setSelected(false);
            item->clearFocus();
            group->addToGroup(item);
        }
        group->setFlags(QGraphicsItem::ItemIsMovable
                        | QGraphicsItem::ItemIsSelectable
                        | QGraphicsItem::ItemIsFocusable);

        group->setZValue(++frontZ);
        //group->clearFocus();
        scene->clearSelection();
        group->setSelected(true);
    }
}

//打散
void MainWindow::on_actGroupBreak_triggered()
{
    int cnt = scene->selectedItems().count();
    if (cnt == 1)
    {
        QGraphicsItemGroup  *group = (QGraphicsItemGroup*)scene->selectedItems().at(0);
        scene->destroyItemGroup(group);
    }
}

//删除
void MainWindow::on_actEdit_Delete_triggered()
{
    int cnt = scene->selectedItems().count();
    if (cnt <= 0) return;

    for (int i = 0;i < cnt;i++)
    {
        QGraphicsItem* item = scene->selectedItems().at(0);
        scene->removeItem(item);
    }
}

//保存整个场景
//void MainWindow::on_actionSave_triggered()
//{
//    QPixmap pixmap(width(), height());
//    pixmap.fill(Qt::transparent);
//    QPainter painter;
//    painter.begin(&pixmap);
//    painter.setRenderHint(QPainter::Antialiasing);
//    scene->render(&painter);
//    painter.end();

//    pixmap.save("scene.png");
//}

//打印
//void MainWindow::on_actionPrint_triggered()
//{
//    QPrinter printer;
//    if(QPrintDialog(&printer).exec() == QDialog::Accepted)
//    {
//        QPainter painter(&printer);
//        painter.setRenderHint(QPainter::Antialiasing);
//        scene->render(&painter);
//    }
//}

void MainWindow::on_actionStatistics_triggered()
{
//    QString str;

//    str="矩形数量："+QString("%1").arg(RectNums);
//    //统计对话框：各类几何的数量、颜色
//    connect(ui->action_Statistics,&QAction::triggered,[=](){

//    QMessageBox::about(this,"统计信息",str);
//    });
}

void MainWindow::on_action_triggered()
{

}

void MainWindow::on_action_Statistics_triggered()
{
    QString Rectstr,Trianglestr,Circlestr,Linestr,Polygonstr,Ellipsestr,Str;

    Rectstr="\n矩形数量："+QString("%1").arg(RectNums)+"面积："+QString("%1").arg(RectArea);
    Trianglestr="\n三角形数量："+QString("%1").arg(TriangleNums)+"面积："+QString("%1").arg(TriangleArea);
    Circlestr="\n圆形数量："+QString("%1").arg(CircleNums)+"面积："+QString("%1").arg(CircleArea);
    Linestr="\n直线数量："+QString("%1").arg(LineNums)+"长度："+QString("%1").arg(Linelength);
    Ellipsestr="\n椭圆数量："+QString("%1").arg(EllipseNums)+"面积："+QString("%1").arg(EllipseArea);
    Polygonstr="\n梯形数量："+QString("%1").arg(PolygonNums)+"面积："+QString("%1").arg(PolygonArea);
    Str=Rectstr+Trianglestr+Circlestr+Linestr+Ellipsestr+Polygonstr;
    //统计对话框：各类几何的数量、颜色
    connect(ui->action_Statistics,&QAction::triggered,[=](){
    QMessageBox::about(this,"统计信息",Str);

    });
}

void MainWindow::on_action_Buffer_triggered()
{




}
