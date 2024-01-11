#include "event_loop.h"

tohsaka::event_loop* g_loop;

void thread_func(){
    g_loop->loop();
}

int main(){
    tohsaka::event_loop loop;
    g_loop=&loop;
    std::thread t1{thread_func};
    t1.join();
    return 0;
}