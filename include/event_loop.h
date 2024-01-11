#pragma once
#include "utils.h"
#include <assert.h>
#include <memory>
#include <thread>
#include <vector>
namespace tohsaka {
class channel;
class poller;
class event_loop : nocopyable {
  public:
    event_loop();
    ~event_loop();

    void loop();
    void assert_in_loop_thread() {
        if (!is_in_loop_thread()) {
            abort_not_in_loop_thread();
        }
    }

    bool is_in_loop_thread() const {
        return _thread_id == std::this_thread::get_id();
    }

    void update_channel(channel *channel);
    void quit();
    static event_loop *get_event_loop_of_current_thread();

  private:
    void abort_not_in_loop_thread() { exit(1); }
    using channel_list = std::vector<channel *>;
    bool _looping;
    bool _quit = false;
    std::thread::id _thread_id;
    std::unique_ptr<poller> _poller;
    channel_list _active_channels;
};
} // namespace tohsaka