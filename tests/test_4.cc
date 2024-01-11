#include "channel.h"
#include "event_loop.h"
#include <ctime>
#include <strings.h>
#include <sys/timerfd.h>
#include <unistd.h>

tohsaka::event_loop *g_loop;
void timeout() {
    printf("timeout\n");
    g_loop->quit();
}
int main() {
    tohsaka::event_loop loop;
    g_loop = &loop;

    int timerfd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
    tohsaka::channel channel(&loop, timerfd);

    channel.set_read_callback(timeout);
    channel.enable_reading();

    itimerspec howlong;
    bzero(&howlong, sizeof(howlong));
    howlong.it_value.tv_sec = 5;
    ::timerfd_settime(timerfd, 0, &howlong, nullptr);
    loop.loop();
    ::close(timerfd);
    return 0;
}
