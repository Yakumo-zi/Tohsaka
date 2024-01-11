#pragma once
#include "utils.h"
#include <functional>

namespace tohsaka {
class event_loop;
class channel : nocopyable {
  public:
    using event_callback = std::function<void()>;
    channel(event_loop *loop, int fd);
    void handle_event();
    void set_read_callback(const event_callback &cb) { _read_callback = cb; }
    void set_write_callback(const event_callback &cb) { _write_callback = cb; }
    void set_error_callback(const event_callback &cb) { _error_callback = cb; }

    int fd() const { return _fd; };
    int events() { return _events; };
    void set_revents(int revt) { _revents = revt; };
    bool is_none_event() { return _events == none_event; }

    void enable_reading() {
        _events |= read_event;
        update();
    };
    void enable_writing() {
        _events |= write_event;
        update();
    };
    void disable_writing() {
        _events &= ~write_event;
        update();
    };
    void disable_all() {
        _events = none_event;
        update();
    };

    int index() { return _index; };
    void set_index(int index) { _index = index; };
    event_loop *owner_loop() { return _loop; };

  private:
    void update();

    static const int none_event;
    static const int read_event;
    static const int write_event;

    event_loop *_loop;
    const int _fd;
    int _events;
    int _revents;
    int _index;

    event_callback _read_callback;
    event_callback _write_callback;
    event_callback _error_callback;
};
} // namespace tohsaka