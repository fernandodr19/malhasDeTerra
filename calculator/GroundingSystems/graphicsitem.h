#ifndef GRAPHICSITEM_H
#define GRAPHICSITEM_H

#include <QGraphicsItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <functional>

template <class T>
class GraphicsItem : public T
{
public:
    GraphicsItem() {}
    ~GraphicsItem() {
        if(m_destroyedCallback)
            m_destroyedCallback();
    }

    QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
    {
        if(change == QGraphicsItem::ItemPositionHasChanged) {
            if(m_positionHasChangedCallback)
                m_positionHasChangedCallback();
        }
        return T::itemChange(change, value);
    }

    void setPositionHasChangedCallback(const std::function<void()>& f) { m_positionHasChangedCallback = f; }
    void setDestroyedCallback(const std::function<void()>& f) { m_destroyedCallback = f; }

private:
    std::function<void()> m_positionHasChangedCallback;
    std::function<void()> m_destroyedCallback;

};

class GraphicsEllipseItem : public GraphicsItem<QGraphicsEllipseItem> {};
class GraphicsLineItem : public GraphicsItem<QGraphicsLineItem> {};
class GraphicsRectItem : public GraphicsItem<QGraphicsRectItem> {};
class GraphicsTextItem : public GraphicsItem<QGraphicsTextItem> {};

#endif // GRAPHICSITEM_H
