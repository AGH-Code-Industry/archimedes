//
// Created by tosiek on 24.02.23.
//

#include <exception>
#include <string>

#ifndef ARCHIMEDES_CONFIG_EXCEPTION_H
#define ARCHIMEDES_CONFIG_EXCEPTION_H

namespace arch {
class ConfigException : public std::exception {
public:
    explicit ConfigException(const std::string& message = "");

    [[nodiscard]] const char* what() const noexcept override;

private:
    std::string _message;
};
}


#endif //ARCHIMEDES_CONFIG_EXCEPTION_H
