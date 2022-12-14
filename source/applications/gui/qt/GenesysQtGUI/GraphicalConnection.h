#ifndef GRAPHICALCONNECTION_H
#define GRAPHICALCONNECTION_H

#include <QGraphicsObject>

#include <QGraphicsItem>
#include <QGraphicsObject>
#include <QPen>
#include <QBrush>
#include "GraphicalComponentPort.h"
#include "../../../../kernel/simulator/ModelComponent.h"
#include "../../../../kernel/simulator/Plugin.h"
#include "../../../../kernel/simulator/ConnectionManager.h"

class GraphicalConnection : public QGraphicsObject {
public:
	GraphicalConnection(GraphicalComponentPort* sourceGraphicalPort, GraphicalComponentPort* destinationGraphicalPort, QColor color = Qt::black, QGraphicsItem *parent = nullptr);
	GraphicalConnection(const GraphicalConnection& orig);
	virtual ~GraphicalConnection();
public:
	QRectF boundingRect() const override;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
	Connection* getSource() const;
	Connection* getDestination() const;
	void updateDimensionsAndPosition();
protected: // virtual
	virtual bool sceneEvent(QEvent *event) override;
	//virtual void	hoverEnterEvent(QGraphicsSceneHoverEvent * event)
	//virtual void	hoverLeaveEvent(QGraphicsSceneHoverEvent * event)
	//virtual void	hoverMoveEvent(QGraphicsSceneHoverEvent * event)
	//virtual void	inputMethodEvent(QInputMethodEvent * event)
	//virtual QVariant	inputMethodQuery(Qt::InputMethodQuery query) const
	//virtual QVariant	itemChange(GraphicsItemChange change, const QVariant & value)
	//virtual void	keyPressEvent(QKeyEvent * event)
	//virtual void	keyReleaseEvent(QKeyEvent * event)
	//virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event);
	//virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
	//virtual void mousePressEvent(QGraphicsSceneMouseEvent * event);
	//virtual void	mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
private:
private:
	qreal _width;
	qreal _height;
	unsigned int _margin = 2;
	unsigned int _selWidth = 8;
	Connection* _sourceConnection;
	Connection* _destinationConnection;
	GraphicalComponentPort* _sourceGraphicalPort;
	GraphicalComponentPort* _destinationGraphicalPort;
	QColor _color;
};
#endif // GRAPHICALCONNECTION_H
