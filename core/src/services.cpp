/*  Elise IM, free crossplatform IM client
	Copyright (C) 2012  Elise project

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QMap>
#include <QMutex>
#include <QLatin1String>

#include "core.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
//-- HOOKS --///////////////////////////////////////////////////////////////////////////////////////

intptr_t Core::getAvailableEventsList()
{
	intptr_t result = -1;

	qmutexHooks_.lock();
	QList<QLatin1String>* list = new QList<QLatin1String>(qmapHooks_.keys());
	qmutexHooks_.unlock();

	if (list->count() > 0)
		result = reinterpret_cast<intptr_t>(list);
	else
		delete list;

	return result;
}

int Core::createHookableEvent(const QLatin1String* name)
{
	if (name->size() < 1)
		return -2;

	qmutexHooks_.lock();
	//-- If event already exists - return
	if (qmapHooks_.contains(*name)) {
		qmutexHooks_.unlock();
		return -1;
	}

	THookableEvent* newEvent = new THookableEvent;

	newEvent->qsetSubscribers = 0;
	newEvent->qmutexHook = new QMutex();
	qmapHooks_.insert(*name, newEvent);

	qmutexHooks_.unlock();
	return 0;
}

int Core::destroyHookableEvent(const QLatin1String* name)
{
	if (name->size() < 1)
		return -2;

	qmutexHooks_.lock();
	if (!qmapHooks_.contains(*name)) {
		qmutexHooks_.unlock();
		return -1;
	}

	THookableEvent* p = qmapHooks_.value(*name);

	//-- Destroy all hooks to this event
	if (p->qsetSubscribers != 0)
		delete p->qsetSubscribers;

	delete p->qmutexHook;
	qmapHooks_.remove(*name);
	delete p;
	qmutexHooks_.unlock();
	return 0;
}

int Core::notifyEventHooks(const QLatin1String* name, intptr_t wParam, intptr_t lParam )
{
	//qmutexHooks.lock();
	if (!qmapHooks_.contains(*name))
		return -1;

	int returnErr = 0;
	THookableEvent* p = qmapHooks_.value(*name);
	//qmutexHooks.unlock();

	p->qmutexHook->lock();

	//-- NOTE: We've got the critical section while all this lot are called.
	if (p->qsetSubscribers != 0) {
		QSet<EliseHook>::const_iterator iterSubs = p->qsetSubscribers->constBegin();
		QSet<EliseHook>::const_iterator iterEnd = p->qsetSubscribers->constEnd();
		while (iterSubs != iterEnd) {
			if (returnErr = (*iterSubs)(wParam, lParam))
				break;
			++iterSubs;
		}
	}

	p->qmutexHook->unlock();
	return returnErr;
}

int Core::hookEvent(const QLatin1String* name, EliseHook hookProc)
{
	if (name->size() < 1)
		return -2;

	qmutexHooks_.lock();
	if (!qmapHooks_.contains(*name)) {
		qmutexHooks_.unlock();
		return -1;
	}

	THookableEvent* p = qmapHooks_.value(*name);

	//-- If this hook is first then create the Set
	if (p->qsetSubscribers == 0)
		p->qsetSubscribers = new QSet<EliseHook>();

	p->qsetSubscribers->insert(hookProc);

	qmutexHooks_.unlock();
	return 0;
}

int Core::unhookEvent(const QLatin1String* name, EliseHook hookProc)
{
	if (name == 0 || name->size() < 1)
		return -2;

	qmutexHooks_.lock();
	//-- If the event name is wrong - return
	if (!qmapHooks_.contains(*name)) {
		qmutexHooks_.unlock();
		return -1;
	}

	THookableEvent* p = qmapHooks_.value(*name);
	//-- If there is no subscribers or num is wrong - return
	if (p->qsetSubscribers == 0 || !p->qsetSubscribers->contains(hookProc)) {
		qmutexHooks_.unlock();
		return -3;
	}

	p->qsetSubscribers->remove(hookProc);
	//-- If there is no more subscribers - destroy map and reset subscriberCount
	if (p->qsetSubscribers->isEmpty()) {
		delete p->qsetSubscribers;
		p->qsetSubscribers = 0;
	}

	qmutexHooks_.unlock();
	return 0;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//-- SERVICES --////////////////////////////////////////////////////////////////////////////////////

intptr_t Core::getAvailableServicesList()
{
	intptr_t result = -1;

	qmutexServices_.lock();
	QList<QLatin1String>* list = new QList<QLatin1String>(qmapServices_.keys());
	qmutexServices_.unlock();

	if (list->count() > 0)
		result = reinterpret_cast<intptr_t>(list);
	else
		delete list;

	return result;
}

int Core::createServiceFunction(const QLatin1String* name, EliseService serviceProc)
{
	if (name->size() < 1)
		return -2;
	qmutexServices_.lock();
	//-- Return 1 if service with this name already exists.
	if (qmapServices_.contains(*name)) {
		qmutexServices_.unlock();
		return -1;
	}

	TService* service = new TService;
	service->type = 0;
	service->pfnService = serviceProc;

	qmapServices_.insert(*name, service);
	qmutexServices_.unlock();
	return 0;
}

/*int Core::createServiceFunction(const QLatin1String* name, QEliseService serviceProc)
{
	TService* service;

	service = new TService;
	service->type = 1;
	service->pfnQService = serviceProc;
	return createService(name, service);
}*/

/*int Core::createService(const QLatin1String* name, TService* service)
{
	if (name->size() < 1)
		return -2;
	qmutexServices_.lock();
	//-- Return 1 if service with this name already exists.
	if (qmapServices_.contains(*name)) {
		qmutexServices_.unlock();
		return -1;
	}
	qmapServices_.insert(*name, service);
	qmutexServices_.unlock();
	return 0;
}*/

int Core::serviceExists(const QLatin1String* name)
{
	if (name->size() < 1)
		return -2;

	qmutexServices_.lock();
	bool is = qmapServices_.contains(*name);
	qmutexServices_.unlock();
	//-- If found then return 1, else 0
	return is;
}

intptr_t Core::callService(const QLatin1String* name, intptr_t wParam, intptr_t lParam)
{
	if (name->size() < 1)
		return -2;

	qmutexServices_.lock();
	//-- Check service.
	if (!qmapServices_.contains(*name)) {
		qmutexServices_.unlock();
		return -1;
	}

	TService* service = qmapServices_.value(*name);

	qmutexServices_.unlock();
	switch(service->type)
	{
		case 1:
			//return service->pfnQService();
			//break;
			//case 2:  return ((ELISESERVICEWIDGJ)pfnService)(object,wParam,lParam);
			//case 3:  return ((ELISESERVICEWIDG)pfnService)(object,wParam,lParam,fnParam);
		default:
			return service->pfnService(wParam,lParam);
			//}
	}
	//return qmapServices.value(*name)->pfnService(wParam, lParam);
}

int Core::destroyServiceFunction(const QLatin1String* name)
{
	if (name->size() < 1)
		return -2;
	qmutexServices_.lock();

	if (!qmapServices_.contains(*name)) {
		qmutexServices_.unlock();
		return -1;
	}

	TService* ser = qmapServices_.value(*name);
	ser->type = 0;
	switch (ser->type)
	{
		case 1:
			break;
		default:
			ser->pfnService = 0;
	}
	delete ser;
	qmapServices_.remove(*name);
	qmutexServices_.unlock();
	return 0;
}
