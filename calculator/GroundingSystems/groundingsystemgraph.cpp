#include "groundingsystemgraph.h"
#include <groundingsystem.h>
#include <QMouseEvent>
#include <QGraphicsItem>
#include <QMenu>
#include <QShortcut>
#include <QFont>

GroundingSystemGraph::GroundingSystemGraph(QWidget *parent) :
    GraphicsView(parent)
{
    m_precision = 0.5;

    QShortcut *dShortcut = new QShortcut(this);
    dShortcut->setKey(QKeySequence(Qt::Key_V));
    connect(dShortcut, &QShortcut::activated, [this]() {
        surfaceVoltage(mapToScene(mapFromGlobal(QCursor::pos())));
    });
}

void GroundingSystemGraph::mouseMoveEvent(QMouseEvent *event)
{
    QPoint mousePos = event->pos();
    QPointF position = mapToScene(mousePos);

    if(m_surfaceVoltageItem) {
        QRectF rect = m_surfaceVoltageItem->rect();
        rect.setWidth(position.x() - rect.topLeft().x());
        rect.setHeight(position.y() - rect.topLeft().y());
        m_surfaceVoltageItem->setRect(rect);
    }

    GraphicsView::mouseMoveEvent(event);

    if(m_groundingSystem) {
        double x = std::round(position.x() / m_precision) * m_precision;
        double y = std::round(-position.y() / m_precision) * m_precision;
        double voltage = m_groundingSystem->getSurfaceVoltage(Vector3Dd(x, y));
        emit surfaceVoltageChanged(voltage);
    }
}

void GroundingSystemGraph::mouseReleaseEvent(QMouseEvent *event)
{
    if(m_surfaceVoltageItem) {
        m_surfaceVoltageRects.push_back(m_surfaceVoltageItem->rect().normalized());
        m_scene.removeItem(m_surfaceVoltageItem);
        delete m_surfaceVoltageItem;
        m_surfaceVoltageItem = nullptr;
        updateSurfaceVoltage();
    }
    GraphicsView::mouseReleaseEvent(event);
}

void GroundingSystemGraph::setGroundingSystem(const GroundingSystemPtr& groundingSystem)
{
    m_groundingSystem = groundingSystem;
    m_surfaceVoltageRects.clear();
    m_scene.clear();

    for(const Conductor& conductor : groundingSystem->getConductors()) {
        if(conductor.p0.z() == conductor.p1.z()) { // Horizontal conductor
            m_scene.addLine(conductor.p0.x(), -conductor.p0.y(), conductor.p1.x(), -conductor.p1.y(), QPen(Qt::black, 0));
        }
        else { // Rod
            double r = 0.6;
            double r2 = r / 2.;
            QGraphicsEllipseItem *item = m_scene.addEllipse(-r2, -r2, r, r, QPen(Qt::blue, 0));
            item->setToolTip(tr("Length") + QString(": %1 m").arg(conductor.p0.distanceTo(conductor.p1)));
            item->setPos(conductor.p0.x(), -conductor.p0.y());
        }
    }

    int profileIndex = 1;
    for(const SurfaceVoltageProfile<Vector3Dd>& profile : groundingSystem->getSurfaceVoltageProfiles()) {

        Vector3Dd dir = (profile.pf - profile.pi).normalized();
        Vector3Dd posA = profile.pi - dir * profile.stepSearchMargin;
        Vector3Dd posB = profile.pf + dir * profile.stepSearchMargin;

        m_scene.addLine(posA.x(), -posA.y(), profile.pi.x(), -profile.pi.y(), QPen(Qt::red, 0, Qt::DashLine));
        m_scene.addLine(profile.pf.x(), -profile.pf.y(), posB.x(), -posB.y(), QPen(Qt::red, 0, Qt::DashLine));

        QGraphicsLineItem *line = m_scene.addLine(profile.pi.x(), -profile.pi.y(), profile.pf.x(), -profile.pf.y(), QPen(Qt::red, 0));
        line->setToolTip(QString::number(profileIndex));

        QFont textFont("Arial");
        textFont.setPointSizeF(6);
        ++profileIndex;

        double r = 0.5;
        QGraphicsEllipseItem *touch = m_scene.addEllipse(-r/2., -r/2., r, r, QPen(Qt::black, 0), QBrush(Qt::red));
        touch->setPos(QPointF(profile.maxTouchVoltagePos.x(), -profile.maxTouchVoltagePos.y()));

        QGraphicsEllipseItem *step1 = m_scene.addEllipse(-r/2., -r/2., r, r, QPen(Qt::black, 0), QBrush(Qt::blue));
        step1->setPos(QPointF(profile.maxStepVoltagePos1.x(), -profile.maxStepVoltagePos1.y()));

        QGraphicsEllipseItem *step2 = m_scene.addEllipse(-r/2., -r/2., r, r, QPen(Qt::black, 0), QBrush(Qt::blue));
        step2->setPos(QPointF(profile.maxStepVoltagePos2.x(), -profile.maxStepVoltagePos2.y()));
    }

    int margin = 5;
    QRectF boundingRect = m_scene.itemsBoundingRect();
    boundingRect.adjust(-margin, -margin, margin, margin);
    m_scene.setSceneRect(boundingRect);

    fitInView(boundingRect, Qt::KeepAspectRatio);
    centerOn(boundingRect.center());
    updateSurfaceVoltage();
}

void GroundingSystemGraph::preMenuExec(QMenu& menu, QMouseEvent *event)
{
    menu.addSeparator();

    QPointF position = mapToScene(event->pos());
    QAction *voltageAction = menu.addAction(tr("Calculate surface voltage"));
    connect(voltageAction, &QAction::triggered, [this, position]() { surfaceVoltage(position); });
}

QString GroundingSystemGraph::getExportName()
{
    return m_groundingSystem->getName();
}

void GroundingSystemGraph::updateSurfaceVoltage()
{
    double vMin = 0;
    double vMax = 0;

    double precision2 = m_precision / 2.;

    double first = true;
    for(const QRectF& rect : m_surfaceVoltageRects) {
        for(double x = rect.left(); x <= rect.right(); x += m_precision) {
            x = std::round(x / m_precision) * m_precision;
            for(double y = rect.top(); y <= rect.bottom(); y += m_precision) {
                y = std::round(y / m_precision) * m_precision;
                double v = m_groundingSystem->getSurfaceVoltage(Vector3Dd(x, -y, 0));
                if(first) {
                    vMin = v;
                    vMax = v;
                    first = false;
                }
                else {
                    vMin = std::min(v, vMin);
                    vMax = std::max(v, vMax);
                }
            }
        }
    }

    if(vMax == vMin)
        return;

    for(const QRectF& rect : m_surfaceVoltageRects) {
        for(double x = rect.left(); x <= rect.right(); x += m_precision) {
            x = std::round(x / m_precision) * m_precision;
            for(double y = rect.top(); y <= rect.bottom(); y += m_precision) {
                y = std::round(y / m_precision) * m_precision;

                double v = m_groundingSystem->getSurfaceVoltage(Vector3Dd(x, -y, 0));
                double intensity = (v - vMin) / (vMax - vMin);
                QColor color = QColor(255 * (1-intensity), 255, 255 * (1-intensity));
                QGraphicsItem *item = m_scene.addRect(x - precision2, y - precision2, m_precision, m_precision, QPen(color, 0), QBrush(color));
                item->setZValue(-1);
            }
        }
    }
}

void GroundingSystemGraph::surfaceVoltage(QPointF position)
{
    if(m_surfaceVoltageItem) {
        m_scene.removeItem(m_surfaceVoltageItem);
        delete m_surfaceVoltageItem;
    }
    m_surfaceVoltageItem = new QGraphicsRectItem();
    m_surfaceVoltageItem->setRect(position.x(), position.y(), 0, 0);
    m_surfaceVoltageItem->setPen(QPen(Qt::black, 0));
    m_scene.addItem(m_surfaceVoltageItem);
}
