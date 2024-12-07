#ifndef INFORMATION_NOT_PROVIDED_EXCEPTION_H
#define INFORMATION_NOT_PROVIDED_EXCEPTION_H

#include <exception>

class InformationNotProvidedException : public std::exception
{
  public:
    InformationNotProvidedException(const std::string& _msg) : msg(_msg) {}

    const char* what() const noexcept override { return msg.c_str(); }

  private:
    std::string msg;
};

#endif // INFORMATION_NOT_PROVIDED_EXCEPTION_H