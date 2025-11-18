#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <string>

class LabException : public std::exception {
    std::string msg;
public:
    LabException(const std::string& m) : msg(m) {}
    const char* what() const noexcept override { return msg.c_str(); }
};

#endif // EXCEPTIONS_H
