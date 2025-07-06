#ifndef CUSTOMGRAPHICSVIEW_H
#define CUSTOMGRAPHICSVIEW_H

#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QLabel>
#include <QWidget>

class CustomGraphicsView : public QGraphicsView {
    Q_OBJECT
public:
    CustomGraphicsView(QGraphicsScene *scene, QWidget *parent = nullptr)
        : QGraphicsView(scene, parent) {
        // 创建一个 QLabel 来显示鼠标坐标
        label = new QLabel(this);
        label->setStyleSheet("QLabel { background-color : white; color : black; }");
        label->setFixedSize(120, 20);  // 设置标签大小
    }

protected:
    void mouseMoveEvent(QMouseEvent *event) override {
        // 获取鼠标在视图中的坐标
        QPoint viewPos = event->pos();

        // 将视图坐标转换为场景坐标
        QPointF scenePos = mapToScene(viewPos);

        // 更新 QLabel 显示内容
        label->setText(QString("View: [%1, %2]\nScene: [%3, %4]")
                           .arg(viewPos.x()).arg(viewPos.y())
                           .arg(scenePos.x()).arg(scenePos.y()));
        label->move(viewPos + QPoint(10, 10));  // 将标签移动到鼠标附近

        // 调用父类的 mouseMoveEvent
        QGraphicsView::mouseMoveEvent(event);
    }

private:
    QLabel *label;
};
#endif // CUSTOMGRAPHICSVIEW_H
