#ifndef MYGRAPHICSSCENE_H
#define MYGRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>
#include <QPen>
#include <QQueue>

#include <ctime>

class MyGraphicsScene : public QGraphicsScene
{
public:
    enum class DrawingStage {
        Pen, Rubber, Spray, Pouring, Line, Rectangle, Ellipse, Triangle
    };

    MyGraphicsScene(QGraphicsView* view = nullptr);

    // Events:
    void mousePressEvent(QGraphicsSceneMouseEvent  *mouseEvent) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QGraphicsSceneMouseEvent  *mouseEvent) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent  *mouseEvent) Q_DECL_OVERRIDE;

    void setDrawingStage(DrawingStage stage);
    void setPenSize(int size);
    void setPenColor(QColor color);

private:
    QGraphicsView* _parentView;

    DrawingStage _currentDrawingStage;
    QPen _currentPen;

    qreal _lastMouseX;
    qreal _lastMouseY;

    QGraphicsItem *_lastItem;
    QGraphicsItem *_lastItem2; // For triangle
    QGraphicsItem *_lastItem3; // For triangle

};

#endif // MYGRAPHICSSCENE_H
