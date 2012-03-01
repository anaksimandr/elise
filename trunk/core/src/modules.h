#ifndef MODULES_H
#define MODULES_H

#include "commonheaders.h"

int LoadDefaultModules();
int UnloadDefaultModules();

class PluginInterface
{
public:
	virtual ~PluginInterface() {}

	virtual QStringList brushes() const = 0;
	virtual QRect mousePress(const QString &brush, QPainter &painter,
							 const QPoint &pos) = 0;
	virtual QRect mouseMove(const QString &brush, QPainter &painter,
							const QPoint &oldPos, const QPoint &newPos) = 0;
	virtual QRect mouseRelease(const QString &brush, QPainter &painter,
							   const QPoint &pos) = 0;
};


#endif // MODULES_H
