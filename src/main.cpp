/*
 *  main.cpp
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

 // ReSharper disable CppExpressionWithoutSideEffects
#include "NotificationCenter.h"

struct point
{
	int x, y;
};

class foo
{
public:
	// ReSharper disable once CppMemberFunctionMayBeStatic
	static unsigned int func(struct point* a_point, int a_value)
	{
		printf("Hello std::bind!\n");
		a_point->x = 11;
		a_point->y = 23;
		printf("Hello value %d\n", a_value);
		return 0;
	}
};

enum message
{
	poster,
	second_poster
};

void run_notification()
{
	auto lambda = [](std::any& any) -> std::any
	{
		try
		{
			auto message = std::any_cast<int>(any);
			printf("Received notification %d!\n", message++);
			any = std::make_any<int>(message);
			return 0;
		}
		catch (const std::bad_any_cast&)
		{
			printf("Bad any cast!\n");
			return 1;
		}
	};

	auto i1 = notification_center::default_notification_center().add_observer(
		poster,
		lambda);

	auto i2 = notification_center::default_notification_center().add_observer(
		poster,
		lambda);

	auto i3 = notification_center::default_notification_center().add_observer(
		poster,
		lambda);

	auto i4 = notification_center::default_notification_center().add_observer(
		poster,
		lambda);

	auto i5 = notification_center::default_notification_center().add_observer(
		poster,
		lambda);

	notification_center::default_notification_center().add_observer(
		poster,
		lambda);

	notification_center::default_notification_center().add_observer(
		poster,
		lambda);

	notification_center::default_notification_center().add_observer(
		poster,
		[=]<typename ...ARGS>(ARGS... args) -> unsigned int
		{
			printf("Received notification %d!\n", 8);
			return 0;
		});

	auto value = 1;
	auto payload = std::make_any<int>(value);
	notification_center::default_notification_center().post_notification(poster, payload);

	printf("============\n");

	notification_center::default_notification_center().remove_observer(i1);

	notification_center::default_notification_center().post_notification(poster);

	printf("============\n");

	notification_center::default_notification_center().remove_observer(i2);

	notification_center::default_notification_center().post_notification(poster);

	printf("============\n");

	notification_center::default_notification_center().remove_observer(i3);

	notification_center::default_notification_center().post_notification(poster);

	printf("============\n");

	notification_center::default_notification_center().remove_observer(i4);

	notification_center::default_notification_center().post_notification(poster);

	printf("============\n");

	notification_center::default_notification_center().remove_observer(i5);

	notification_center::default_notification_center().post_notification(poster);

	printf("============\n");

	notification_center::default_notification_center().remove_all_observers(poster);

	notification_center::default_notification_center().post_notification(poster);

	printf("============\n");

	notification_center::default_notification_center().add_observer(
		second_poster,
		[=](const std::any&) -> unsigned int
		{
			printf("Called by iterator!\n");
			return 0;
		});
	//auto itr = notification_center::default_notification_center().get_notification_iterator(second_poster);
	//notification_center::default_notification_center().post_notification(itr);
	printf("============\n");

	struct point a_point { 1, 1 };
	printf("Point x.value = %d\n", a_point.x);
	printf("Point y.value = %d\n", a_point.y);
	notification_center::default_notification_center().add_observer(
		second_poster,
		std::bind(&foo::func, &a_point, 1));  // NOLINT(modernize-avoid-bind)

	notification_center::default_notification_center().post_notification(second_poster);
	printf("Point x.value = %d\n", a_point.x);
	printf("Point y.value = %d\n", a_point.y);
}

int main()
{
	run_notification();

	return 0;
}