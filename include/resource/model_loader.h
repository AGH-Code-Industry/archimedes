#include <graphics/model.h>
#include <string>

namespace arch {

class ModelLoadingException : public std::exception {
public:
    ModelLoadingException(const char *msg) : msg(msg) {}
    virtual const char *what() const noexcept override { return msg.c_str(); }
private:
    std::string msg;
};


class ModelLoader {
public:
    virtual Model load_model(const std::string &path) = 0;
protected:
    ModelLoader() = default;
};

}