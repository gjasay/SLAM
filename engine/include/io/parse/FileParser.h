#pragma once
#include <string>
#include <memory>
#include <stdexcept>

namespace slam::io {

class ParseError : public std::runtime_error {
public:
    explicit ParseError(const std::string& message) : std::runtime_error(message) {}
};

class FileParser {
public:
    virtual ~FileParser() = default;
    virtual void Parse(const std::string& filePath) = 0;
    virtual void Write(const std::string& filePath) = 0;
};

}
