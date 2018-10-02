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
    std::lock_guard<std::mutex> aLock(mMutex);
    NotificationObserver aNotificationObserver;
    aNotificationObserver.mCallback = aMethod;
    mObservers[aName].push_back(aNotificationObserver);
    return --mObservers[aName].end();
}

NotificationCenter::observer_const_itr_t NotificationCenter::addObserver(std::function<unsigned int(std::any)> aMethod, notification_itr_t &aNotification)
{
    std::lock_guard<std::mutex> aLock(mMutex);
    NotificationCenter::observer_const_itr_t aReturnValue = aNotification->second.end();
    if (aNotification != mObservers.end())
    {
        NotificationObserver aNotificationObserver;
        aNotificationObserver.mCallback = aMethod;
        aNotification->second.push_back(aNotificationObserver);
        aReturnValue = --aNotification->second.end();
    }
    return aReturnValue;
}

void NotificationCenter::removeObserver(const std::string& aName, observer_const_itr_t& aObserver)
{
    std::lock_guard<std::mutex> aLock(mMutex);
    notification_itr_t aNotificationIterator = mObservers.find(aName);
    if (aNotificationIterator != mObservers.end())
    {
        aNotificationIterator->second.erase(aObserver);
    }
}

void NotificationCenter::removeObserver(notification_itr_t& aNotification, observer_const_itr_t& aObserver)
{
    std::lock_guard<std::mutex> aLock(mMutex);
    if (aNotification != mObservers.end())
    {
        aNotification->second.erase(aObserver);
    }
}

void NotificationCenter::removeAllObservers(const std::string& aName)
{
    std::lock_guard<std::mutex> aLock(mMutex);
    mObservers.erase(aName);
}

void NotificationCenter::removeAllObservers(notification_itr_t& aNotification)
{
    std::lock_guard<std::mutex> aLock(mMutex);
    if (aNotification != mObservers.end())
    {
        mObservers.erase(aNotification);
    }
}

bool NotificationCenter::postNotification(const std::string& aNotification, std::any aPayload) const
{
    std::lock_guard<std::mutex> aLock(mMutex);
    notification_const_itr_t aNotificationIterator = mObservers.find(aNotification);
    if (aNotificationIterator != mObservers.end())
    {
        const std::list<NotificationObserver>& aNotificationList = aNotificationIterator->second;
        for (observer_const_itr_t aIterator = aNotificationList.begin(); aIterator != aNotificationList.end(); aIterator++)
        {
            aIterator->mCallback(aPayload);
        }
        return true;
    }
    else
    {
        printf("WARNING: Notification \"%s\" does not exist.\n", aNotification.data());
        return false;
    }
}

bool NotificationCenter::postNotification(NotificationCenter::notification_const_itr_t& aNotification, std::any aPayload) const
{
    std::lock_guard<std::mutex> aLock(mMutex);
    if (aNotification != mObservers.end())
    {
        const std::list<NotificationObserver>& aNotificationList = aNotification->second;
        for (observer_const_itr_t aIterator = aNotificationList.begin(); aIterator != aNotificationList.end(); aIterator++)
        {
            aIterator->mCallback(aPayload);
        }
        return true;
    }
    else
    {
        printf("WARNING: Notification \"%s\" does not exist.\n", aNotification->first.data());
        return false;
    }
}

NotificationCenter::notification_itr_t NotificationCenter::getNotificationIterator(const std::string& aNotification)
{
    notification_itr_t aReturnValue;
    if (mObservers.find(aNotification) != mObservers.end())
    {
        aReturnValue = mObservers.find(aNotification);
    }
    
    return aReturnValue;
}

NotificationCenter& NotificationCenter::defaultNotificationCenter()
{
	// Guaranteed to be destroyed. Instantiated on first use.
	static NotificationCenter aNotificationcenter;
    
    return aNotificationcenter;
}
