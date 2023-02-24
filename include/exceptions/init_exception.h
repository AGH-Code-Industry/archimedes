//
// Created by tosiek on 22.02.23.
//

#include <exception>
#include <string>

#ifndef ARCHIMEDES_INIT_EXCEPTION_H
#define ARCHIMEDES_INIT_EXCEPTION_H

namespace arch {
    class InitException : public std::exception {
    public:
        explicit InitException(const std::string& message = "");

        [[nodiscard]] const char* what() const noexcept override;

    private:
        std::string _message;
    };
}


#endif //ARCHIMEDES_INIT_EXCEPTION_H
