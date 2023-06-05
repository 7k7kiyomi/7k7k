#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void initMainWindow();
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void on_mousePress(QPoint point);
    void on_mouse_Right_Press(QPoint point);//右键单击o
    void on_mouseDoubleClicked(QPoint point);
    void on_mouseMove(QPoint point);
    void on_keyPress(QKeyEvent* event);

    void on_actItem_Ellipse_triggered();

    void on_actItem_Rect_triggered();

    void on_actItem_Circle_triggered();

    void on_actItem_Triangle_triggered();

    void on_actItem_Polygon_triggered();

    void on_actItem_Line_triggered();

//    void on_actItem_Text_triggered();

    void on_actZoomIn_triggered();

    void on_actZoomOut_triggered();

    void on_actRotateLeft_triggered();

    void on_actRotateRight_triggered();

    void on_actRestore_triggered();

    void on_actEdit_Front_triggered();

    void on_actEdit_Back_triggered();

    void on_actGroup_triggered();

    void on_actGroupBreak_triggered();

    void on_actEdit_Delete_triggered();

//    void on_actionSave_triggered();

//    void on_actionPrint_triggered();

    void on_action_triggered();

    void on_actionStatistics_triggered();

    void on_action_Statistics_triggered();

    void on_action_Buffer_triggered();



private:
    Ui::MainWindow *ui;
    //图形项自定义数据
    static const int ItemId = 1;
    //图形项自定义数据
    static const int ItemDes = 2;
    //图形面积
    static const int Area=3;
    //图形编号:每新建一个图形对应一个编号
    int seqNum = 0;
    //图形的z轴顺序
    int frontZ = 0;
    int backZ = 0;

    //Status cur_status;  // 当前状态
public:
    //各类图形的初始数量
    int RectNums=0;
    int CircleNums=0;
    int TriangleNums=0;
    int EllipseNums=0;
    int PolygonNums=0;
    int LineNums=0;
    double RectArea=100*50;
    double CircleArea=3.14*50*50;
    double TriangleArea=0.5*120*40;
    double Linelength=200;
    double PolygonArea=0.5*(80+200)*80;
    double EllipseArea=3.14*100*60;


    QGraphicsScene *scene;
    QLabel *labViewCord;
    QLabel *labSceneCord;
    QLabel *labItemCord;
    QLabel *labItemInfo;
};
#endif // MAINWINDOW_H
