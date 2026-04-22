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

    void _update(ArgMap args) override {
        std::println("Dies ist ein update");
    }

    static std::shared_ptr<Test> create(ArgMap args) {
        auto test = std::make_shared<Test>();

        _nodeSet.insert(test);

        return test;
    }
};

RUNA_API_INIT(
    printf("Initializing test\n");
    Node::add_type("test", Test::create);)