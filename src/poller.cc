#include "poller.h"
#include "channel.h"
#include <cassert>
#include <sys/poll.h>

namespace tohsaka {
poller::poller(event_loop *loop) : _owner_loop(loop) {}

int poller::poll(int timeout_ms, channel_list *active_channels) {
    int num_events = ::poll(_pollfds.data(), _pollfds.size(), timeout_ms);
    if (num_events > 0) {
        fill_active_channels(num_events, active_channels);
    } else if (num_events == 0) {

    } else {
    }
    return 0;
}
void poller::fill_active_channels(int num_events,
                                  channel_list *active_channels) const {
    for (const auto &pfd : _pollfds) {
        if(num_events<=0){break;}
        if (pfd.fd > 0) {
            --num_events;
            auto ch = _channels.find(pfd.fd);
            assert(ch != _channels.end());
            auto channel = ch->second;
            assert(channel->fd() == pfd.fd);
            channel->set_revents(pfd.revents);
            active_channels->push_back(channel);
        }
    }
}

void poller::update_channel(channel *ch) {
    assert_in_loop_thread();
    if (ch->index() < 0) {
        // 新增一个 channel
        assert(_channels.find(ch->fd()) == _channels.end());
        pollfd pfd;
        pfd.fd = ch->fd();
        pfd.events = static_cast<short>(ch->events());
        pfd.revents = 0;
        _pollfds.push_back(pfd);
        int idx = static_cast<short>(_pollfds.size() - 1);
        ch->set_index(idx);
        _channels[pfd.fd] = ch;
    } else {
        // 更新存在的 channel
        assert(_channels.find(ch->fd()) != _channels.end());
        assert(_channels[ch->fd()] == ch);
        int idx = ch->index();
        assert(0 <= idx && idx < static_cast<int>(_pollfds.size()));
        pollfd &pfd = _pollfds[idx];
        assert(pfd.fd == ch->fd() || pfd.fd != -1);
        pfd.events = static_cast<int>(ch->events());
        pfd.revents = 0;
        if (ch->is_none_event()) {
            pfd.fd = -1;
        }
    }
}
poller::~poller() {}
} // namespace tohsaka