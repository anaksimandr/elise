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

const QLatin1String	kShutdown_service		=	QLatin1String(__Core_Shutdown_service);
const QLatin1String	kChangeProfile_service	=	QLatin1String(__Core_ChangeProfile_service);
const QLatin1String	kDBWriteSetting_service	=	QLatin1String(__DB_WriteSetting_service);
const QLatin1String	kDBReadSetting_service	=	QLatin1String(__DB_ReadSetting_service);
const QLatin1String	kDBDellSetting_service	=	QLatin1String(__DB_DellSetting_service);
//const QLatin1String	kClistShow_service	=	QLatin1String(__CList_Show_service);

////////////////////////////////////////////////////////////////////////////////////////////////////
//-- HOOKS --///////////////////////////////////////////////////////////////////////////////////////

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

	THookEvent* newEvent;

	newEvent = new THookEvent;
	newEvent->qsetSubscribers = NULL;
	newEvent->qmutexHook = new QMutex();
	qmapHooks_[*name] = newEvent;

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

	THookEvent* p;
	p = qmapHooks_[*name];

	//-- Destroy all hooks to this event
	if (p->qsetSubscribers != NULL)
		delete p->qsetSubscribers;

	delete p->qmutexHook;
	qmapHooks_.remove(*name);
	delete p;
	qmutexHooks_.unlock();
	return 0;
}

int Core::notifyEventHooks(const QLatin1String* name, uintptr_t wParam, uintptr_t lParam )
{
	//qmutexHooks.lock();
	if (!qmapHooks_.contains(*name))
		return -1;

	int returnErr = 0;
	THookEvent* p = qmapHooks_[*name];
	//qmutexHooks.unlock();

	p->qmutexHook->lock();

	//-- NOTE: We've got the critical section while all this lot are called.
	if (p->qsetSubscribers != NULL) {
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

	THookEvent* p = qmapHooks_[*name];

	//-- If this hook is first then create the Set
	if (p->qsetSubscribers == NULL)
		p->qsetSubscribers = new QSet<EliseHook>();

	p->qsetSubscribers->insert(hookProc);

	qmutexHooks_.unlock();
	return 0;
}

int Core::unhookEvent(const QLatin1String* name, EliseHook hookProc)
{
	if (name == NULL || name->size() < 1)
		return -2;

	qmutexHooks_.lock();
	//-- If the event name is wrong - return
	if (!qmapHooks_.contains(*name)) {
		qmutexHooks_.unlock();
		return -1;
	}

	THookEvent* p = qmapHooks_[*name];
	//-- If there is no subscribers or num is wrong - return
	if (p->qsetSubscribers == NULL || !p->qsetSubscribers->contains(hookProc)) {
		qmutexHooks_.unlock();
		return -3;
	}

	p->qsetSubscribers->remove(hookProc);
	//-- If there is no more subscribers - destroy map and reset subscriberCount
	if (p->qsetSubscribers->isEmpty()) {
		delete p->qsetSubscribers;
		p->qsetSubscribers = NULL;
	}

	qmutexHooks_.unlock();
	return 0;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//-- SERVICES --////////////////////////////////////////////////////////////////////////////////////

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

	TService* newSer;

	newSer = new TService;
	newSer->type = 0;
	newSer->pfnService = serviceProc;

	qmapServices_[*name] = newSer;
	qmutexServices_.unlock();
	return 0;
}

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

intptr_t Core::callService(const QLatin1String* name, uintptr_t wParam, uintptr_t lParam)
{
	if (name->size() < 1)
		return -2;

	qmutexServices_.lock();
	//-- Check service.
	if (!qmapServices_.contains(*name)) {
		qmutexServices_.unlock();
		return -1;
	}

	TService* ser = qmapServices_.value(*name);

	qmutexServices_.unlock();
	switch(ser->type)
	{
		case 1:
			//return ((ELISESERVICEWIDG)pfnServiceWidg)(wParam,lParam,fnParam);
		//case 2:  return ((ELISESERVICEWIDGJ)pfnService)(object,wParam,lParam);
		//case 3:  return ((ELISESERVICEWIDG)pfnService)(object,wParam,lParam,fnParam);
		default:
			return ser->pfnService(wParam,lParam);
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
			ser->pfnService = NULL;
	}
	delete ser;
	qmapServices_.remove(*name);
	qmutexServices_.unlock();
	return 0;
}
