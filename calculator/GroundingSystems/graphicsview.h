#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>
#include <QMenu>
#include <graphicsitem.h>
//#include <thirdparty/dxflib/dl_dxf.h>

class GraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    enum GraphicsItemProperty {
        ItemNotExportable = Qt::UserRole + 100,
    };

    GraphicsView(QWidget *parent = 0);

    void drawBackground(QPainter *painter, const QRectF& rect);
    void drawForeground(QPainter *painter, const QRectF& rect);
    void drawGrid(QPainter *painter, const QRectF& rect);
    void drawCoordinates(QPainter *painter, const QRectF& rect);
    void drawArrow(QPainter *painter, const QPointF& from, const QPointF& to);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    double getDistance();
    QPair<QPointF, QPointF> getDistancePoints();
    bool exportImage(const QString& filename, QRectF sceneRect = QRectF());
//    bool exportDxf(const QString& filename);
//    void exportDxfItem(DL_WriterA *dw, DL_Dxf& dxf, QGraphicsItem *item, const QString& referenceZone);
    bool isGeographical() const { return m_isGeographical; }

    void setGridVisible(bool visible) { m_gridVisible = visible; }
    void setCoordinatesVisible(bool visible) { m_coordinatesVisible = visible; }
    void setFlipX(bool flip) { m_flipX = flip; }
    void setFlipY(bool flip) { m_flipY = flip; }
    void setIsGeographical(bool isGeographical) { m_isGeographical = isGeographical; }

protected slots:
    void distance(QPointF position);

protected:
    virtual void preMenuExec(QMenu&, QMouseEvent *) {}
    virtual QString getExportName() { return "untitled"; }

    QGraphicsScene m_scene;
    double m_scale = 1;
    double m_scaleFactor = 1.25;
    bool m_gridVisible = true;
    bool m_coordinatesVisible = false;
    bool m_flipX = false;
    bool m_flipY = false;
    bool m_isGeographical = false;
    GraphicsLineItem *m_distanceItem = nullptr;

signals:
    void positionChanged(QPointF position);
    void scaleChanged(double scale);
};

#endif // GRAPHICSVIEW_H
