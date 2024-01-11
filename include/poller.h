#pragma once
#include "event_loop.h"
#include "utils.h"
#include <map>
#include <vector>

struct pollfd;
namespace tohsaka {
class channel;

class poller : nocopyable {
  public:
    using channel_list = std::vector<channel *>;
    poller(event_loop *loop);
    ~poller();
    int poll(int timeout_ms, channel_list *active_channels);
    void update_channel(channel *ch);
    void assert_in_loop_thread() { _owner_loop->assert_in_loop_thread(); }

  private:
    void fill_active_channels(int num_events,
                              channel_list *active_channels) const;
    using poll_fd_list = std::vector<pollfd>;
    using channel_map = std::map<int, channel *>;

    event_loop *_owner_loop;
    poll_fd_list _pollfds;
    channel_map _channels;
};
} // namespace tohsaka