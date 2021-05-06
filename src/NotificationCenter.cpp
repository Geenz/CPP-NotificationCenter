/*
 *  NotificationCenter.cpp
 *  Notification Center CPP
 *
 *  Created by Jonathan Goodman on 11/23/13.
 *  Copyright (c) 2013 Jonathan Goodman. All rights reserved.
 *  Edited by Salvatore Rivieccio.
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
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <utility>

#include "NotificationCenter.h"

notification_center::notification_tuple_t notification_center::add_observer(
	const int a_name, std::function<std::any(std::any)> a_method)
{
	std::lock_guard a_lock(m_mutex_);
	notification_observer a_notification_observer;
	a_notification_observer.m_callback = std::move(a_method);
	m_observers_[a_name].push_back(a_notification_observer);
	return std::make_tuple(a_name, --m_observers_[a_name].end());
}

notification_center::observer_const_itr_t notification_center::add_observer(
	notification_itr_t& a_notification, std::function<std::any(std::any)> a_method)
{
	std::lock_guard a_lock(m_mutex_);
	observer_const_itr_t a_return_value = a_notification->second.end();
	if (a_notification != m_observers_.end())
	{
		notification_observer a_notification_observer;
		a_notification_observer.m_callback = std::move(a_method);
		a_notification->second.push_back(a_notification_observer);
		a_return_value = --a_notification->second.end();
	}
	return a_return_value;
}

void notification_center::remove_observer(notification_tuple_t& a_notification)
{
	std::lock_guard a_lock(m_mutex_);
	if (auto a_notification_iterator = m_observers_.find(std::get<0>(a_notification));
		a_notification_iterator != m_observers_.end())
	{
		a_notification_iterator->second.erase(std::get<1>(a_notification));
	}
}

void notification_center::remove_observer(notification_itr_t& a_notification, observer_const_itr_t& a_observer)
{
	std::lock_guard a_lock(m_mutex_);
	if (a_notification != m_observers_.end())
	{
		a_notification->second.erase(a_observer);
	}
}

void notification_center::remove_all_observers(const int a_name)
{
	std::lock_guard a_lock(m_mutex_);
	m_observers_.erase(a_name);
}

void notification_center::remove_all_observers(notification_itr_t& a_notification)
{
	std::lock_guard a_lock(m_mutex_);
	if (a_notification != m_observers_.end())
	{
		m_observers_.erase(a_notification);
	}
}

bool notification_center::post_notification(const int a_notification, const std::any& a_payload) const
{
	std::lock_guard a_lock(m_mutex_);
	if (const auto a_notification_iterator = m_observers_.find(a_notification);
		a_notification_iterator != m_observers_.end())
	{
		const auto& a_notification_list = a_notification_iterator->second;
		for (const auto& [callback] : a_notification_list)
		{
			// ReSharper disable once CppExpressionWithoutSideEffects
			callback(a_payload);
		}
		return true;
	}
	else
	{
		printf("WARNING: Notification \"%d\" does not exist.\n", a_notification);
		return false;
	}
}

bool notification_center::post_notification(notification_const_itr_t& a_notification, const std::any&
	a_payload) const
{
	std::lock_guard a_lock(m_mutex_);
	if (a_notification != m_observers_.end())
	{
		const auto& a_notification_list = a_notification->second;
		for (const auto& [callback] : a_notification_list)
		{
			// ReSharper disable once CppExpressionWithoutSideEffects
			callback(a_payload);
		}
		return true;
	}
	else
	{
		printf("WARNING: Notification \"%d\" does not exist.\n", a_notification->first);
		return false;
	}
}

notification_center::notification_itr_t notification_center::get_notification_iterator(const int a_notification)
{
	notification_itr_t a_return_value;
	if (m_observers_.find(a_notification) != m_observers_.end())
	{
		a_return_value = m_observers_.find(a_notification);
	}

	return a_return_value;
}

notification_center& notification_center::default_notification_center()
{
	// Guaranteed to be destroyed. Instantiated on first use.
	// ReSharper disable once CommentTypo
	static notification_center a_notification;  // NOLINT(clang-diagnostic-exit-time-destructors)

	return a_notification;
}