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
#include "NotificationCenter.hpp"

std::shared_ptr<NotificationCenter> NotificationCenter::_defaultCenter = nullptr;

NotificationCenter::observer_const_itr_t NotificationCenter::addObserver(std::function<void()> method, const std::string& name)
{
    std::lock_guard<std::mutex> lock(_mutex);
    NotificationObserver n;
    n.callback = method;
    _observers[name].push_back(n);
    return --_observers[name].end();
}

NotificationCenter::observer_const_itr_t NotificationCenter::addObserver(std::function<void (void)> method, notification_itr_t &notification)
{
    std::lock_guard<std::mutex> lock(_mutex);
    NotificationCenter::observer_const_itr_t retVal = notification->second.end();
    if (notification != _observers.end())
    {
        NotificationObserver n;
        n.callback = method;
        notification->second.push_back(n);
        retVal = --notification->second.end();
    }
    return retVal;
}

void NotificationCenter::removeObserver(const std::string& name, observer_const_itr_t& observer)
{
    std::lock_guard<std::mutex> lock(_mutex);
    notification_itr_t i = _observers.find(name);
    if (i != _observers.end())
    {
        i->second.erase(observer);
    }
}

void NotificationCenter::removeObserver(notification_itr_t& notification, observer_const_itr_t& observer)
{
    std::lock_guard<std::mutex> lock(_mutex);
    if (notification != _observers.end())
    {
        notification->second.erase(observer);
    }
}

void NotificationCenter::removeAllObservers(const std::string& name)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _observers.erase(name);
}

void NotificationCenter::removeAllObservers(notification_itr_t& notification)
{
    std::lock_guard<std::mutex> lock(_mutex);
    if (notification != _observers.end())
    {
        _observers.erase(notification);
    }
}

bool NotificationCenter::postNotification(const std::string& notification) const
{
    std::lock_guard<std::mutex> lock(_mutex);
    notification_const_itr_t i = _observers.find(notification);
    if (i != _observers.end())
    {
        const std::list<NotificationObserver>& notiList = i->second;
        for (observer_const_itr_t ia = notiList.begin(); ia != notiList.end(); ia++)
        {
            ia->callback();
        }
        return true;
    }
    else
    {
        printf("WARNING: Notification \"%s\" does not exist.\n", notification.data());
        return false;
    }
}

bool NotificationCenter::postNotification(NotificationCenter::notification_const_itr_t& notification) const
{
    std::lock_guard<std::mutex> lock(_mutex);
    if (notification != _observers.end())
    {
        const std::list<NotificationObserver>& notiList = notification->second;
        for (observer_const_itr_t i = notiList.begin(); i != notiList.end(); i++)
        {
            i->callback();
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
    if (_observers.find(notification) != _observers.end())
    {
        retVal = _observers.find(notification);
    }
    
    return retVal;
}

std::shared_ptr<NotificationCenter> NotificationCenter::defaultNotificationCenter()
{
    static std::mutex mutex;
    std::lock_guard<std::mutex> lock(mutex);
    if (!_defaultCenter)
    {
        _defaultCenter = std::shared_ptr<NotificationCenter>(new NotificationCenter);
    }
    
    return _defaultCenter;
}
