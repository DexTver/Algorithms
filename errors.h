#include <exception>
#include <string>

class ShapeException : public std::exception {
protected:
    std::string msg;
public:
    explicit ShapeException(const std::string &message)
            : msg(message) {}

    const char *what() const noexcept override {
        return msg.c_str();
    }
};

class NegativeSizeException : public ShapeException {
public:
    explicit NegativeSizeException(const std::string &message)
            : ShapeException(message) {}
};

class OutOfScreenException : public ShapeException {
public:
    explicit OutOfScreenException(const std::string &message)
            : ShapeException(message) {}
};
