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

std::shared_ptr<NotificationCenter> NotificationCenter::_defaultCenter = NULL;

NotificationCenter::observer_const_itr_t NotificationCenter::addObserver(std::function<void()> method, const std::string& name)
{
    std::lock_guard<std::mutex> lock(_mutex);
    NotificationObserver n;
    n.callback = method;
    _observers[name].push_back(n);
    return --_observers[name].end();
}

void NotificationCenter::removeObserver(const std::string& name, std::list<NotificationObserver>::const_iterator& observer)
{
    std::lock_guard<std::mutex> lock(_mutex);
    if (_observers.find(name) != _observers.end())
    {
        _observers.at(name).erase(observer);
    }
}

void NotificationCenter::removeAllObservers(const std::string& name)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _observers.erase(name);
}

bool NotificationCenter::postNotification(const std::string& notification) const
{
    std::lock_guard<std::mutex> lock(_mutex);
    if (_observers.find(notification) != _observers.end())
    {
        const std::list<NotificationObserver>& notiList = _observers.at(notification);
        for (std::list<NotificationObserver>::const_iterator i = notiList.begin(); i != notiList.end(); i++)
        {
            i->callback();
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
        for (std::list<NotificationObserver>::const_iterator i = notiList.begin(); i != notiList.end(); i++)
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

NotificationCenter::notification_const_itr_t NotificationCenter::getNotificationIterator(const std::string& notification) const
{
    std::map<std::string, std::list<NotificationObserver> >::const_iterator retVal;
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