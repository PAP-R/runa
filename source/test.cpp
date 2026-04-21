#include "Node.hpp"
#include "PluginHeader.hpp"

struct Test : public Node {
    int number = 5;

    Test() {
        _name = "Test";
    }

    ~Test() {
        std::println("Tschüss dies war ein test {}", this->number);
    }

    void _update(AppState *appstate) override {
        std::println("Dies ist ein update");
    }

    static std::shared_ptr<Test> create() {
        return std::make_shared<Test>();
    }
};

extern "C" {
RUNA_API void init() {
    printf("Initializing test\n");
    Node::add_type("test", Test::create);
}
}