#include "channel.h"
#include "event_loop.h"
#include <sys/poll.h>

namespace tohsaka {
const int channel::none_event = 0;
const int channel::read_event = POLLIN | POLLPRI;
const int channel::write_event = POLLOUT;

channel::channel(event_loop *loop, int fd)
    : _loop(loop), _fd(fd), _events(0), _revents(0), _index(-1) {}
void channel::update() { _loop->update_channel(this); }

void channel::handle_event() {
    if (_revents & POLLNVAL) {
    }
    if (_revents & (POLLERR | POLLNVAL)) {
        if (_error_callback) {
            _error_callback();
        }
    }
    if (_revents & (POLLIN | POLLPRI | POLLHUP)) {
        if (_read_callback) {
            _read_callback();
        }
    }
    if (_revents & (POLLOUT)) {
        if (_write_callback) {
            _write_callback();
        }
    }
}
} // namespace tohsaka