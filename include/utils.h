#pragma once

namespace tohsaka {
class nocopyable {
  public:
    nocopyable(const nocopyable &) = delete;
    nocopyable(const nocopyable &&) = delete;
    nocopyable &operator=(const nocopyable &) = delete;

  protected:
    nocopyable() = default;
    ~nocopyable() = default;
};
} // namespace tohsaka