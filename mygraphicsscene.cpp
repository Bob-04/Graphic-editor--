#include "mygraphicsscene.h"

MyGraphicsScene::MyGraphicsScene(QGraphicsView* view)
    : QGraphicsScene(1, 1, 1119, 589),
      _parentView(view),
      _currentDrawingStage(DrawingStage::Pen),
      _currentPen(Qt::black, 2),
      _lastMouseX(0), _lastMouseY(0),
      _lastItem(nullptr), _lastItem2(nullptr), _lastItem3(nullptr)
{
    setBackgroundBrush(Qt::white);
}


void MyGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    _lastMouseX = event->scenePos().x();
    _lastMouseY = event->scenePos().y();

    // Pouring the field using BFS
    if(_currentDrawingStage == DrawingStage::Pouring) {
        QImage img = _parentView->grab().toImage();

        QPoint curPoint(static_cast<int>(_lastMouseX), static_cast<int>(_lastMouseY));
        QRgb colorForCanghe = img.pixelColor(curPoint).rgb();
        QRgb penColor = _currentPen.color().rgb();

        if(penColor == colorForCanghe)
            return;

        bool marked[1119][589];
        for(int i=0; i<1119; i++)
            for(int j=0; j<589; j++)
                marked[i][j] = false;

        QQueue<QPoint> queue;
        queue.enqueue(QPoint(curPoint));

        while (!queue.empty()) {
            curPoint = queue.dequeue();

            if(marked[curPoint.x()][curPoint.y()])
                continue;
            marked[curPoint.x()][curPoint.y()] = true;

            //qDebug() << QString::number(curPoint.x()) + " " + QString::number(curPoint.y());
            img.setPixel(curPoint, penColor);

            // Add left point
            if(curPoint.x() > 0) {
                QPoint leftPoint(curPoint.x()-1, curPoint.y());
                if(colorForCanghe == img.pixelColor(leftPoint).rgb())
                    queue.enqueue(leftPoint);
            }
            // Add bottom point
            if(curPoint.y() < 588) {
                QPoint bottomPoint(curPoint.x(), curPoint.y()+1);
                if(colorForCanghe == img.pixelColor(bottomPoint).rgb())
                    queue.enqueue(bottomPoint);
            }
            // Add right point
            if(curPoint.x() < 1118) {
                QPoint rightPoint(curPoint.x()+1, curPoint.y());
                if(colorForCanghe == img.pixelColor(rightPoint).rgb())
                    queue.enqueue(rightPoint);
            }
            // Add top point
            if(curPoint.y() > 0) {
                QPoint topPoint(curPoint.x(), curPoint.y()-1);
                if(colorForCanghe == img.pixelColor(topPoint).rgb())
                    queue.enqueue(topPoint);
            }
        }

        this->addPixmap(QPixmap::fromImage(img));
    }
}

void MyGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{

    qreal curX = event->scenePos().x();
    qreal curY = event->scenePos().y();

    if(_lastItem != nullptr) {
        removeItem(_lastItem);
        delete _lastItem;
    }

    switch (_currentDrawingStage) {

    case DrawingStage::Pen:{
        addLine(_lastMouseX, _lastMouseY, curX, curY, _currentPen);
        _lastMouseX = curX;
        _lastMouseY = curY;
        break;
    }
    case DrawingStage::Rubber:{
        QPen rubberPen( Qt::white, _currentPen.width());
        int penWidth = _currentPen.width()/2;
        addLine(curX - penWidth, curY, curX + penWidth, curY, rubberPen);
        break;
    }
    case DrawingStage::Spray:{
        qreal penWidth = _currentPen.width();
        srand(time(NULL));
        for(int i=0; i<penWidth/2; ++i) {
            qreal pointX = curX - penWidth/2 + static_cast<qreal>(rand() % static_cast<int>(penWidth));
            qreal pointY = curY - penWidth/2 + static_cast<qreal>(rand() % static_cast<int>(penWidth));
            addEllipse(pointX, pointY, 1, 1, QPen(_currentPen.color(), 0));
        }
        break;
    }
    case DrawingStage::Line:{
        _lastItem = addLine(_lastMouseX, _lastMouseY, curX, curY, _currentPen);
        break;
    }
    case DrawingStage::Rectangle:{
        qreal rectX = curX>_lastMouseX ? _lastMouseX : curX;
        qreal rectY = curY>_lastMouseY ? _lastMouseY : curY;
        _lastItem = addRect(rectX, rectY, abs(curX-_lastMouseX), abs(curY-_lastMouseY), _currentPen);
        break;
    }
    case DrawingStage::Ellipse:{
        qreal ellipseX = curX>_lastMouseX ? _lastMouseX : curX;
        qreal ellipseY = curY>_lastMouseY ? _lastMouseY : curY;
        _lastItem = addEllipse(ellipseX, ellipseY, abs(curX-_lastMouseX), abs(curY-_lastMouseY), _currentPen);
        break;
    }
    case DrawingStage::Triangle:{
        if(_lastItem2 != nullptr) {
            removeItem(_lastItem2);
            delete _lastItem2;
        }
        if(_lastItem3 != nullptr) {
            removeItem(_lastItem3);
            delete _lastItem3;
        }
        _lastItem = addLine(_lastMouseX, _lastMouseY, (_lastMouseX+curX)/2, curY, _currentPen);
        _lastItem2 = addLine((_lastMouseX+curX)/2, curY, curX, _lastMouseY, _currentPen);
        _lastItem3 = addLine(curX, _lastMouseY, _lastMouseX, _lastMouseY, _currentPen);
        break;
    }
    case DrawingStage::Pouring:{
        break;
    }
    }
}

void MyGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    _lastItem = nullptr;
    _lastItem2 = nullptr;
    _lastItem3 = nullptr;
}

void MyGraphicsScene::setDrawingStage(DrawingStage stage)
{
    _currentDrawingStage = stage;
}

void MyGraphicsScene::setPenSize(int size)
{
    _currentPen.setWidth(size);
}

void MyGraphicsScene::setPenColor(QColor color)
{
    _currentPen.setColor(color);
}
