//
// Created by tosiek on 22.02.23.
//

#include <exception>

#ifndef ARCHIMEDES_INIT_EXCEPTION_H
#define ARCHIMEDES_INIT_EXCEPTION_H

namespace arch {
    class InitException : public std::exception {
    public:
        explicit InitException(const std::string&& message = "") : _message(message){};

        const char* what();

    private:
        const std::string _message;
    };
}


#endif //ARCHIMEDES_INIT_EXCEPTION_H
