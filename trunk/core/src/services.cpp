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

const QLatin1String	kShutdown_service		=	QLatin1String("System/Shutdown");
const QLatin1String	kChangeProfile_service	=	QLatin1String("System/ChangeAcc");
const QLatin1String	kDBWriteSetting_service	=	QLatin1String("DB/WriteSetting");
const QLatin1String	kDBReadSetting_service	=	QLatin1String("DB/ReadSetting");
const QLatin1String	kDBDellSetting_service	=	QLatin1String("DB/DeleteSetting");

////////////////////////////////////////////////////////////////////////////////////////////////////
//-- HOOKS --///////////////////////////////////////////////////////////////////////////////////////

int Core::createHookableEvent(const QLatin1String* name)
{
	//if (name->isEmpty())
	//	return -1;

	if (name->size() < 1)
		return -2;

	qmutexHooks_.lock();
	//-- If event already exists - return
	if (qmapHooks_.contains(*name)) {
		qmutexHooks_.unlock();
		return -1;
	}

	THookEvent* newEvent;

	//newEvent = (THookEvent*)malloc(sizeof(THookEvent));
	newEvent = new THookEvent;
	newEvent->subscriberCount = 0;
	newEvent->qmapSubscribers = NULL;
	//newEvent->pfnHook = hokableSig;
	newEvent->qmutexHook = new QMutex();
	qmapHooks_[*name] = newEvent;

	qmutexHooks_.unlock();
	return 0;
}

int Core::destroyHookableEvent(const QLatin1String* name)
{
	//if (name->isEmpty())
	//	return -1;

	if (name->size() < 1)
		return -2;

	qmutexHooks_.lock();
	//if ( pLastHook == ( THook* )hEvent )
	//	pLastHook = NULL;
	if (!qmapHooks_.contains(*name)) {
		qmutexHooks_.unlock();
		return -1;
	}

	THookEvent* p;
	p = qmapHooks_[*name];

	//-- Destroy all hooks to this event
	if (p->qmapSubscribers != NULL) {
		QMapIterator<int, THookSubscriber*> iter(*p->qmapSubscribers);
		THookSubscriber* s;
		while (iter.hasNext()) {
			iter.next();
			s = iter.value();
			s->num = 0;
			s->type = 0;
			s->pfnHook = NULL;
			delete s;
		}
		p->qmapSubscribers->~QMap();
		p->qmapSubscribers = NULL;
		p->subscriberCount = 0;
	}

	p->qmutexHook->~QMutex();
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
	if (p->qmapSubscribers != NULL) {
		QMapIterator<int, THookSubscriber*> iter(*p->qmapSubscribers);
		THookSubscriber* s;
		while (iter.hasNext()) {
			iter.next();
			s = iter.value();
			switch (s->type) {
				case 1:
					returnErr = s->pfnHook(wParam, lParam);
					break;
				case 2:
					//returnVal = s->pfnHookParam( wParam, lParam, s->lParam );
					break;
				case 3:
					//returnVal = s->pfnHookObj( s->object, wParam, lParam );
					break;
				case 4:
					//returnVal = s->pfnHookObjParam( s->object, wParam, lParam, s->lParam );
					break;
				case 5:
					//returnVal = SendMessage( s->hwnd, s->message, wParam, lParam );
					break;
				default:
					continue;
			}
			if (returnErr)
				break;
		} //while
	} else {
		//-- Call the default hook if any  because no other hooks
		//	returnVal = p->pfnHook( wParam, lParam );
	}

	p->qmutexHook->unlock();
	return returnErr;
}

int Core::hookEvent(const QLatin1String* name, EliseHook hookProc)
{	
	//if (name->isEmpty())
	//	return -2;

	if (name->size() < 1)
		return -2;

	qmutexHooks_.lock();
	if (!qmapHooks_.contains(*name)) {
		qmutexHooks_.unlock();
		return -1;
	}

	THookEvent* p;
	THookSubscriber* newSubscr;

	p = qmapHooks_[*name];

	//-- Create new subscriber
	//newSubscr = (THookSubscriber*)malloc(sizeof(THookSubscriber));
	newSubscr = new THookSubscriber;
	newSubscr->num = p->subscriberCount;
	newSubscr->type = 1;
	newSubscr->pfnHook = hookProc;

	//-- If this hook is first then create the map
	if(p->qmapSubscribers == NULL)
	{
		p->qmapSubscribers = new QMap<int, THookSubscriber*>();
	}

	p->qmapSubscribers->insert(newSubscr->num, newSubscr);

	qmutexHooks_.unlock();
	return p->subscriberCount++;
}

int Core::unhookEvent(const THook hook)
{
	//if (hook.name == NULL || hook.name->isEmpty())
	//	return -1;

	if (hook.name == NULL || hook.name->size() < 1)
		return -2;

	qmutexHooks_.lock();
	//-- If the event name is wrong - return
	if (!qmapHooks_.contains(*hook.name)) {
		qmutexHooks_.unlock();
		return -1;
	}

	THookEvent* p = qmapHooks_[*hook.name];
	//-- If there is no subscribers or num is wrong - return
	if (p->qmapSubscribers == NULL || !p->qmapSubscribers->contains(hook.num)) {
		qmutexHooks_.unlock();
		return -3;
	}

	THookSubscriber* s = p->qmapSubscribers->value(hook.num);
	s->num = 0;
	s->type = 0;
	s->pfnHook = NULL;
	delete s;
	p->qmapSubscribers->remove(hook.num);
	//-- If there is no more subscribers - destroy map and reset subscriberCount
	if (p->qmapSubscribers->isEmpty()) {
		p->qmapSubscribers->~QMap();
		p->qmapSubscribers = NULL;
		p->subscriberCount = 0;
	}

	qmutexHooks_.unlock();
	return 0;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//-- SERVICES --////////////////////////////////////////////////////////////////////////////////////

//int Core::createServiceFunction(const QLatin1String* name, EliseService serviceProc, int type = 0)
int Core::createServiceFunction(const QLatin1String* name, EliseService serviceProc)
{	
	//if (name->isEmpty())
	//	return -1;
	if (name->size() < 1)
		return -2;
	qmutexServices_.lock();
	//-- Return 1 if service with this name already exists.
	if (qmapServices_.contains(*name)) {
		qmutexServices_.unlock();
		return -1;
	}

	TService* newSer;

	//newSer = (TService*)malloc(sizeof(TService));
	newSer = new TService;
	/*newSer->type = type;
	switch (type)
	{
		case 1:
			//newSer->pfnServiceWidg = (ELISESERVICEWIDG)serviceProc;
			break;
		default:
			newSer->pfnService = serviceProc;
			break;
	}*/
	newSer->type = 0;
	newSer->pfnService = serviceProc;

	qmapServices_[*name] = newSer;
	qmutexServices_.unlock();
	return 0;
}

//int createServiceFunction(const QLatin1String* name, EliseService serviceProc)
//{
//	return CreateServiceFunctionInt(name, serviceProc, 0);
//}

//int CreateServiceFunctionWidg(const QString *name, ELISESERVICEWIDG serviceProc)
//{
//	return CreateServiceFunctionInt(name, (ELISESERVICE)serviceProc, 1);
//}

int Core::serviceExists(const QLatin1String* name)
{
	//if (name->isEmpty())
	//	return -1;
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
	//if (name->isEmpty())
	//	return SERVICE_NOTFOUND;
	if (name->size() < 1)
		return -2;

	qmutexServices_.lock();
	//-- Check service. Return SERVICE_NOTFOUND if service with this name not exists
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
			//case 2:  return ((MIRANDASERVICEOBJ)pfnService)(object,wParam,lParam);
			//case 3:  return ((MIRANDASERVICEOBJPARAM)pfnService)(object,wParam,lParam,fnParam);
		default:
			return ser->pfnService(wParam,lParam);
			//}
	}
	//return qmapServices.value(*name)->pfnService(wParam, lParam);
}

int Core::destroyServiceFunction(const QLatin1String* name)
{
	//if (name->isEmpty())
	//	return -1;
	if (name->size() < 1)
		return -2;
	qmutexServices_.lock();
	//-- Return SERVICE_NOTFOUND if service with this name not exists.
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
