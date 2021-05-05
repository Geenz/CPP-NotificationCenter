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
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
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

class Foo {
    
public:
	unsigned int func(struct point* aPoint)
    {
        printf("Hello std::bind!\n");
		aPoint->x = 11;
		aPoint->y = 23;
		return 0;
    }
};

void runNotification()
{
	auto i1 = notification_center::default_notification_center().add_observer([=](const std::any&) -> unsigned int
	{
		printf("Received notification %d!\n", 1);
		return 0;
	}, "Poster");
    auto i2 = notification_center::default_notification_center().add_observer([=](const std::any&) -> bool
    {
	    printf("Received notification %d!\n", 2);
	    return true;
    }, "Poster");
    auto i3 = notification_center::default_notification_center().add_observer([=](const std::any&) -> unsigned int
    {
	    printf("Received notification %d!\n", 3);
	    return 0;
    }, "Poster");
    auto i4 = notification_center::default_notification_center().add_observer([=](const std::any&) -> unsigned int
    {
	    printf("Received notification %d!\n", 4);
	    return 0;
    }, "Poster");
    auto i5 = notification_center::default_notification_center().add_observer([=](const std::any&) -> unsigned int
    {
	    printf("Received notification %d!\n", 5);
	    return 0;
    }, "Poster");
    notification_center::default_notification_center().add_observer([=](const std::any&) -> unsigned int
    {
	    printf("Received notification %d!\n", 6);
	    return 0;
    }, "Poster");
    notification_center::default_notification_center().add_observer([=](const std::any&) -> unsigned int
    {
	    printf("Received notification %d!\n", 7);
	    return 0;
    }, "Poster");
    notification_center::default_notification_center().add_observer([=](const std::any&) -> unsigned int
    {
	    printf("Received notification %d!\n", 8);
	    return 0;
    }, "Poster");
    
    notification_center::default_notification_center().post_notification("Poster");
    
    printf("============\n");
    
    notification_center::default_notification_center().remove_observer("Poster", i1);
    
    notification_center::default_notification_center().post_notification("Poster");
    
    printf("============\n");
    
    notification_center::default_notification_center().remove_observer("Poster", i2);
    
    notification_center::default_notification_center().post_notification("Poster");
    
    printf("============\n");
    
    notification_center::default_notification_center().remove_observer("Poster", i3);
    
    notification_center::default_notification_center().post_notification("Poster");
    
    printf("============\n");
    
    notification_center::default_notification_center().remove_observer("Poster", i4);
    
    notification_center::default_notification_center().post_notification("Poster");
    
    printf("============\n");
    
    notification_center::default_notification_center().remove_observer("Poster", i5);
    
    notification_center::default_notification_center().post_notification("Poster");
    
    printf("============\n");
    
    notification_center::default_notification_center().remove_all_observers("Poster");
    
    notification_center::default_notification_center().post_notification("Poster");
    
    printf("============\n");
    
    notification_center::default_notification_center().add_observer([=](const std::any&) -> unsigned int {printf("Called by iterator!\n"); return 0; }, "Second Poster");
    notification_center::notification_const_itr_t itr = notification_center::default_notification_center().get_notification_iterator("Second Poster");
    notification_center::default_notification_center().post_notification(itr);
    printf("============\n");
    
    Foo myFoo;
    
	struct point aPoint {1,1};
	printf("Point x.value = %d\n", aPoint.x);
	printf("Point y.value = %d\n", aPoint.y);
    notification_center::default_notification_center().add_observer(std::bind(&Foo::func, myFoo, &aPoint), "Second Poster");

    notification_center::default_notification_center().post_notification("Second Poster");
	printf("Point x.value = %d\n", aPoint.x);
	printf("Point y.value = %d\n", aPoint.y);
}

int main(int argc, const char * argv[])
{
    runNotification();
    
    return 0;
}

