#include <assimp/LogStream.hpp>

namespace arch {

class SpdlogStream : public Assimp::LogStream {
public:
    void write(const char *msg) override;
};

class AssimpInitializer {
public:
    static void init();
private:
    static void init_logger();
};

}