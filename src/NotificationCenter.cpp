/*
 *  NotificationCenter.cpp
 *  Notification Center CPP
 *
 *  Created by Jonathan Goodman on 11/23/13.
 *  Copyright (c) 2013 Jonathan Goodman. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include "NotificationCenter.h"

std::shared_ptr<NotificationCenter> NotificationCenter::mDefaultCenter = nullptr;

NotificationCenter::observer_const_itr_t NotificationCenter::addObserver(std::function<unsigned int(std::any)> aMethod, const std::string& aName)
{
    std::lock_guard<std::mutex> lock(mMutex);
    NotificationObserver n;
    n.mCallback = aMethod;
    mObservers[aName].push_back(n);
    return --mObservers[aName].end();
}

NotificationCenter::observer_const_itr_t NotificationCenter::addObserver(std::function<unsigned int(std::any)> method, notification_itr_t &notification)
{
    std::lock_guard<std::mutex> lock(mMutex);
    NotificationCenter::observer_const_itr_t retVal = notification->second.end();
    if (notification != mObservers.end())
    {
        NotificationObserver n;
        n.mCallback = method;
        notification->second.push_back(n);
        retVal = --notification->second.end();
    }
    return retVal;
}

void NotificationCenter::removeObserver(const std::string& name, observer_const_itr_t& observer)
{
    std::lock_guard<std::mutex> lock(mMutex);
    notification_itr_t i = mObservers.find(name);
    if (i != mObservers.end())
    {
        i->second.erase(observer);
    }
}

void NotificationCenter::removeObserver(notification_itr_t& notification, observer_const_itr_t& observer)
{
    std::lock_guard<std::mutex> lock(mMutex);
    if (notification != mObservers.end())
    {
        notification->second.erase(observer);
    }
}

void NotificationCenter::removeAllObservers(const std::string& name)
{
    std::lock_guard<std::mutex> lock(mMutex);
    mObservers.erase(name);
}

void NotificationCenter::removeAllObservers(notification_itr_t& notification)
{
    std::lock_guard<std::mutex> lock(mMutex);
    if (notification != mObservers.end())
    {
        mObservers.erase(notification);
    }
}

bool NotificationCenter::postNotification(const std::string& notification, std::any aPayload) const
{
    std::lock_guard<std::mutex> lock(mMutex);
    notification_const_itr_t i = mObservers.find(notification);
    if (i != mObservers.end())
    {
        const std::list<NotificationObserver>& notiList = i->second;
        for (observer_const_itr_t ia = notiList.begin(); ia != notiList.end(); ia++)
        {
            ia->mCallback(aPayload);
        }
        return true;
    }
    else
    {
        printf("WARNING: Notification \"%s\" does not exist.\n", notification.data());
        return false;
    }
}

bool NotificationCenter::postNotification(NotificationCenter::notification_const_itr_t& notification, std::any aPayload) const
{
    std::lock_guard<std::mutex> lock(mMutex);
    if (notification != mObservers.end())
    {
        const std::list<NotificationObserver>& notiList = notification->second;
        for (observer_const_itr_t i = notiList.begin(); i != notiList.end(); i++)
        {
            i->mCallback(aPayload);
        }
        return true;
    }
    else
    {
        printf("WARNING: Notification \"%s\" does not exist.\n", notification->first.data());
        return false;
    }
}

NotificationCenter::notification_itr_t NotificationCenter::getNotificationIterator(const std::string& notification)
{
    notification_itr_t retVal;
    if (mObservers.find(notification) != mObservers.end())
    {
        retVal = mObservers.find(notification);
    }
    
    return retVal;
}

NotificationCenter& NotificationCenter::defaultNotificationCenter()
{
	// Guaranteed to be destroyed. Instantiated on first use.
	static NotificationCenter aNotificationcenter;
    
    return aNotificationcenter;
}
