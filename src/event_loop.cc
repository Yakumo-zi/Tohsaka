#include "event_loop.h"
#include "channel.h"
#include "poller.h"
#include <cassert>
#include <cstdlib>
#include <memory>
#include <sys/poll.h>
#include <thread>

namespace tohsaka {
thread_local event_loop *t_loop_in_this_thread = nullptr;

event_loop::event_loop()
    : _looping(false), _thread_id(std::this_thread::get_id()) {
    if (t_loop_in_this_thread) {
        exit(1);
    } else {
        t_loop_in_this_thread = this;
        _poller = std::make_unique<poller>(this);
    }
}

event_loop::~event_loop() {
    assert(!_looping);
    t_loop_in_this_thread = nullptr;
}
void event_loop::loop() {
    assert(!_looping);
    assert_in_loop_thread();
    _looping = true;
    _quit = false;
    while (!_quit) {
        _active_channels.clear();
        _poller->poll(1000, &_active_channels);
        for (auto it : _active_channels) {
            it->handle_event();
        }
    }
    _looping = false;
}
void event_loop::quit() { _quit = true; }
void event_loop::update_channel(channel *channel) {
    assert(channel->owner_loop() == this);
    assert_in_loop_thread();
    _poller->update_channel(channel);
}
event_loop *event_loop::get_event_loop_of_current_thread() {
    return t_loop_in_this_thread;
}
} // namespace tohsaka