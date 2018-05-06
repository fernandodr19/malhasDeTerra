#include "graphicsview.h"
#include <QMouseEvent>
#include <QShortcut>
#include <QGraphicsLineItem>
#include <tmath.h>
//#include <util/geolocation.h>
//#include <widgets/graphicsarcitem.h>
//#include <widgets/graphicsarrowitem.h>
//#include <widgets/graphicscrossitem.h>
//#include <widgets/graphicsedgeitem.h>
//#include <widgets/graphicsflooritem.h>
//#include <widgets/graphicsgrounditem.h>
//#include <widgets/graphicsdimensionradial.h>
//#include <widgets/graphicsdimensionangular.h>
//#include <core/project.h>
#include <QFileDialog>
#include <QMenu>
#include <QMessageBox>
#include <QTextDocument>
#include <QDebug>
#include <array>

GraphicsView::GraphicsView(QWidget *parent) :
    QGraphicsView(parent)
{
    setFrameShape(QFrame::NoFrame);
    setDragMode(ScrollHandDrag);
    setInteractive(true);
    setMouseTracking(true);
    setScene(&m_scene);
    setRenderHint(QPainter::Antialiasing);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    viewport()->setCursor(Qt::ArrowCursor);

    QShortcut *dShortcut = new QShortcut(this);
    dShortcut->setKey(QKeySequence(Qt::Key_D));
    connect(dShortcut, &QShortcut::activated, [this]() {
        distance(mapToScene(mapFromGlobal(QCursor::pos())));
    });
}

void GraphicsView::drawBackground(QPainter *painter, const QRectF& rect)
{
    if(m_gridVisible)
        drawGrid(painter, rect);

    // debug draw bounding box
    //painter->setPen(QPen(Qt::black, 0));
    //painter->drawRect(m_scene.sceneRect());
    //painter->drawRect(m_scene.itemsBoundingRect());
}

void GraphicsView::drawForeground(QPainter *painter, const QRectF& rect)
{
    if(m_coordinatesVisible)
        drawCoordinates(painter, rect);
}

void GraphicsView::drawGrid(QPainter *painter, const QRectF& rect)
{
    qreal left, top;
    if(isGeographical()) {
        left = rect.left() - std::fmod(rect.left(), m_scale);
        top = rect.top() - std::fmod(rect.top(), m_scale);
    }
    else {
        left = int(rect.left()) - (int(rect.left()) % (int)m_scale);
        top = int(rect.top()) - (int(rect.top()) % (int)m_scale);
    }

    QVarLengthArray<QLineF, 100> lines;

    for(qreal x = left; x < rect.right(); x += m_scale)
        lines.append(QLineF(x, rect.top(), x, rect.bottom()));
    for(qreal y = top; y < rect.bottom(); y += m_scale)
        lines.append(QLineF(rect.left(), y, rect.right(), y));

    if(lines.size() > 1000)
        lines.clear(); // something went wrong

    painter->save();
    painter->setPen(QPen(Qt::lightGray, 0));
    painter->drawLines(lines.data(), lines.size());
    painter->restore();
}

void GraphicsView::drawCoordinates(QPainter *painter, const QRectF& rect)
{
    qreal size = std::max(rect.width(), rect.height()) * 0.1;

    QPointF fromX, toX;
    if(m_flipX)
        toX = fromX - QPointF(size, 0);
    else
        toX = fromX + QPointF(size, 0);
    drawArrow(painter, fromX, toX);

    QPointF fromY, toY;
    if(m_flipY)
        toY = fromY - QPointF(0, size);
    else
        toY = fromY + QPointF(0, size);
    drawArrow(painter, fromY, toY);

    QFont font = painter->font();
    font.setPointSizeF(1);

    painter->save();
    painter->setFont(font);
    painter->scale(size * 0.25, size * 0.25);
    painter->drawText(toX / (size * 0.25), "X");
    painter->drawText(toY / (size * 0.25), "Y");
    painter->restore();
}

void GraphicsView::drawArrow(QPainter *painter, const QPointF& from, const QPointF& to)
{
    QVarLengthArray<QLineF, 3> arrow;

    QLineF line(from, to);
    arrow.push_back(line);

    double angle = std::atan2(line.dx(), line.dy()) - math::pi / 2.;
    const double arrowSize = line.length() * 0.2;
    QPointF arrowP1 = to + QPointF(std::sin(angle - math::pi / 3), std::cos(angle - math::pi / 3)) * arrowSize;
    QPointF arrowP2 = to + QPointF(std::sin(angle - math::pi + math::pi / 3), std::cos(angle - math::pi + math::pi / 3)) * arrowSize;

    arrow.push_back(QLineF(line.p2(), arrowP1));
    arrow.push_back(QLineF(line.p2(), arrowP2));

    painter->save();
    painter->setPen(QPen(Qt::black, line.length() * 0.04));
    painter->drawLines(arrow.data(), arrow.size());
    painter->restore();
}

void GraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    QPoint mousePos = event->pos();
    QPointF position = mapToScene(mousePos);

    if(m_distanceItem) {
        QLineF line = m_distanceItem->line();
        line.setP2(position);
        m_distanceItem->setLine(line);
    }

    position.setY(-position.y());
    emit positionChanged(position);

    QGraphicsView::mouseMoveEvent(event);
}

void GraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    if(m_distanceItem) {
        m_scene.removeItem(m_distanceItem);
        delete m_distanceItem;
        m_distanceItem = nullptr;
    }

    if(event->button() == Qt::RightButton) {
        QMenu menu;

        if(QGraphicsItem *item = itemAt(event->pos())) {
            QAction *hideAction = menu.addAction(tr("Hide item"));
            connect(hideAction, &QAction::triggered, [item]() { item->setVisible(false); });
        }

        QAction *toggleGridAction = menu.addAction(m_gridVisible ? tr("Hide grid") : tr("Show grid"));
        connect(toggleGridAction, &QAction::triggered, [this]() {
            m_gridVisible = !m_gridVisible;
            viewport()->update();
        });

        menu.addAction(tr("Zoom expand"), this, SLOT(zoomExpand()));
        menu.addAction(tr("Export as image"), this, SLOT(exportAsImage()));
        menu.addAction(tr("Export as DXF"), this, SLOT(exportAsDxf()));

        QPointF position = mapToScene(event->pos());
        QAction *distanceAction = menu.addAction(tr("Distance"));
        distanceAction->setShortcut(QKeySequence(Qt::Key_D));
        connect(distanceAction, &QAction::triggered, [this, position]() { distance(position); });

        preMenuExec(menu, event);
        menu.exec(QCursor::pos());
    }

    QGraphicsView::mouseReleaseEvent(event);
    viewport()->setCursor(Qt::ArrowCursor);
}

double GraphicsView::getDistance()
{
    if(m_distanceItem)
        return m_distanceItem->line().length();
    return -1;
}

QPair<QPointF, QPointF> GraphicsView::getDistancePoints()
{
    if(m_distanceItem)
        return {m_distanceItem->line().p1(), m_distanceItem->line().p2()};
    return {QPointF(), QPointF()};
}
#include <QCoreApplication>
bool GraphicsView::exportImage(const QString& filename, QRectF sceneRect)
{
    if(sceneRect.isNull())
        sceneRect = m_scene.itemsBoundingRect();

    QSize imageSize(683, 512);

    if(sceneRect.width() > sceneRect.height())
        imageSize.setHeight(imageSize.width() * sceneRect.height() / sceneRect.width());
    else if(sceneRect.width() < sceneRect.height())
        imageSize.setWidth(imageSize.height() * sceneRect.width() / sceneRect.height());

    QImage image(imageSize, QImage::Format_ARGB32);
    QPainter painter(&image);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.fillRect(0, 0, image.width(), image.height(), Qt::transparent);
    painter.setRenderHint(QPainter::Antialiasing);
    m_scene.render(&painter, QRectF(), sceneRect);
//    return image.save(filename);

    QDir prev = QCoreApplication::applicationDirPath();
    prev.cd("..");
    prev.cd("..");
    QImage graphLegend(prev.path() + "/assets/images/legend_graph.png");
    QImage imageWithLegend =
      QImage(QSize(image.width() + graphLegend.height() + 100, image.height() - 50),
             QImage::Format_ARGB32_Premultiplied);
    QPainter painter2(&imageWithLegend);

    painter2.setCompositionMode(QPainter::CompositionMode_Source);
    painter2.fillRect(imageWithLegend.rect(), Qt::white);

    painter2.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter2.drawImage(0,0, image);

    painter2.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter2.drawImage(image.width() - 70, image.height()/2 - graphLegend.height()/2, graphLegend);

    painter2.end();

    return imageWithLegend.save(filename);
}

void GraphicsView::distance(QPointF position)
{
    if(m_distanceItem) {
        m_scene.removeItem(m_distanceItem);
        delete m_distanceItem;
    }
    m_distanceItem = new GraphicsLineItem();
    m_distanceItem->setLine(position.x(), position.y(), position.x(), position.y());
    m_distanceItem->setPen(QPen(Qt::black, 0));
    m_distanceItem->setDestroyedCallback([this]() { m_distanceItem = nullptr; });
    m_scene.addItem(m_distanceItem);
}
