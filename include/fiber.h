#pragma once
#include <functional>
#include <memory>
#include <thread>
#include <ucontext.h>

class fiber : public std::enable_shared_from_this<fiber> {
  public:
    using ptr = std::shared_ptr<fiber>;

  private:
};