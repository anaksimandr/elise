#ifndef SKINRESIZEHANDLER_H
#define SKINRESIZEHANDLER_H

#include "skinobject.h"

class SkinResizeHandler;
class SkinObject;

typedef struct {
	union
	{
		unsigned char chars[4];		//-- '0' - red, '1' - green, '2' - blue
		qint32 value;
	};
} EColor;


class SkinObject : public QObject
{
	Q_OBJECT

};

#endif // SKINRESIZEHANDLER_H
