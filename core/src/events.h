#ifndef ELISE_CORE_EVENTS_H_
#define ELISE_CORE_EVENTS_H_

#include <QEvent>

extern QEvent::Type g_shutdownEventType;
extern QEvent::Type g_changeProfileEventType;

class QShutdownEvent : public QEvent
{
public:
	QShutdownEvent() : QEvent(QShutdownEvent::type()) { }

	virtual ~QShutdownEvent() { }

	static QEvent::Type type()
	{
		/*if (g_shutdownEventType == QEvent::None)
		{
			int generatedType = QEvent::registerEventType();
			g_shutdownEventType = static_cast<QEvent::Type>(generatedType);
		}*/
		return g_shutdownEventType;
	}
};

class QChangeProfileEvent : public QEvent
{
public:
	QChangeProfileEvent() : QEvent(QChangeProfileEvent::type()) { }

	virtual ~QChangeProfileEvent() { }

	static QEvent::Type type()
	{
		return g_changeProfileEventType;
	}
};

#endif // ELISE_CORE_EVENTS_H_
