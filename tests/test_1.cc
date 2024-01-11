#include "event_loop.h"
void thread_func() {
    tohsaka::event_loop loop;
    loop.loop();
}
int main() {
    tohsaka::event_loop loop;
    std::thread t1{thread_func};
    t1.join();
    loop.loop();
    return 0;
}