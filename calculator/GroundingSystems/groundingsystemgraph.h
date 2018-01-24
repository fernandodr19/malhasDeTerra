#ifndef GROUNDINGSYSTEMGRAPH_H
#define GROUNDINGSYSTEMGRAPH_H

#include <graphicsview.h>
#include <const.h>

class GroundingSystem;

class GroundingSystemGraph : public GraphicsView
{
    Q_OBJECT

public:
    GroundingSystemGraph(QWidget *parent = 0);

    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    void setGroundingSystem(const GroundingSystemPtr& groundingSystem);

protected:
    virtual void preMenuExec(QMenu& menu, QMouseEvent *event);
    QString getExportName();

private:
    void updateSurfaceVoltage();

    QGraphicsRectItem *m_surfaceVoltageItem = nullptr;
    GroundingSystemPtr m_groundingSystem = nullptr;
    QVector<QRectF> m_surfaceVoltageRects;
    double m_precision;

private slots:
    void surfaceVoltage(QPointF position);

signals:
    void surfaceVoltageChanged(double voltage);

};

#endif // GROUNDINGSYSTEMGRAPH_H
