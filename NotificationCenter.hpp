/*
 *  NotificationCenter.hpp
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

#ifndef __Notification_Center_CPP__NotificationCenter__
#define __Notification_Center_CPP__NotificationCenter__

#include <iostream>
#include <map>
#include <functional>
#include <string>
#include <list>
#include <memory>
#include <thread>
#include <mutex>

struct NotificationObserver {
    std::function<void(void)> callback;
};

class NotificationCenter {
    static std::shared_ptr<NotificationCenter> _defaultCenter;
    std::map<std::string, std::list<NotificationObserver> > _observers;
    mutable std::mutex _mutex;
public:
    
    typedef std::map<std::string, std::list<NotificationObserver> >::const_iterator notification_const_itr_t;
    typedef std::map<std::string, std::list<NotificationObserver> >::iterator notification_itr_t;
    typedef std::list<NotificationObserver>::const_iterator observer_const_itr_t;
    typedef std::list<NotificationObserver>::iterator observer_itr_t;
    
    /**
     * This method adds a function callback as an observer to a named notification.
     * @param method the function callback.  Accepts void(void) methods or lambdas.
     * @param name the name of the notification you wish to observe.
     */
    observer_const_itr_t addObserver(std::function<void(void)> method, const std::string& name);
    
    /**
     * This method adds a function callback as an observer to a given notification.
     * @param method the function callback.  Accepts void(void) methods or lambdas.
     * @param name the name of the notification you wish to observe.
     */
    observer_const_itr_t addObserver(std::function<void(void)> method, notification_itr_t& notification);
    
    /**
     * This method removes an observer by iterator.
     * @param name the name of the notification you wish to remove a given observer from.
     * @param observer the iterator to the observer you wish to remove.
     */
    void removeObserver(const std::string& name, observer_const_itr_t& observer);
    
    /**
     * This method removes an observer by iterator.
     * @param notification the iterator of the notification you wish to remove a given observer from.
     * @param observer the iterator to the observer you wish to remove.
     */
    void removeObserver(notification_itr_t& notification, observer_const_itr_t& observer);
    
    /**
     * This method removes all observers from a given notification, removing the notification from being tracked outright.
     * @param name the name of the notification you wish to remove.
     */
    void removeAllObservers(const std::string& name);
    
    /**
     * This method removes all observers from a given notification, removing the notification from being tracked outright.
     * @param notification the iterator of the notification you wish to remove.
     */
    void removeAllObservers(notification_itr_t& notification);
    
    /**
     * This method posts a notification to a set of observers.
     * If successful, this function calls all callbacks associated with that notification and return true.  If no such notification exists, this function will print a warning to the console and return false.
     * @param name the name of the notification you wish to post.
     */
    bool postNotification(const std::string& notification) const;
    
    /**
     * This method posts a notification to a set of observers.
     * If successful, this function calls all callbacks associated with that notification and return true.  If no such notification exists, this function will print a warning to the console and return false.
     * @param name the name of the notification you wish to post.
     */
    bool postNotification(notification_const_itr_t& notification) const;
    
    /**
     * This method retrieves a notification iterator for a named notification.
     * The returned iterator may be used with the overloaded variants of postNotification, removeAllObservers, removeObserver, and addObserver to avoid string lookups.
     * @param name the name of the notification you wish to post.
     */
    notification_itr_t getNotificationIterator(const std::string& notification);
    
    /**
     * This method returns the default global notification center.  You may alternatively create your own notification center without using the default notification center.
     */
    static std::shared_ptr<NotificationCenter> defaultNotificationCenter();
};

#endif /* defined(__Notification_Center_CPP__NotificationCenter__) */
