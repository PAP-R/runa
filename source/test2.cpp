#include "Log.hpp"
#include "Node.hpp"
#include "PluginHeader.hpp"

struct Test : public Node {
    int number = 5;

    Test(ArgMap args) {
        _name = "Test2";
    }

    ~Test() {
        std::println("Tschüss dies war ein test2 {}", this->number);
    }

    void _update(ArgMap args) override {
        Log::println("Dies ist ein update2");
    }

    void _event(SDL_Event *event, ArgMap args) override {
        Log::println("Dies ist ein event");
    }

    static std::shared_ptr<Test> create(ArgMap args) {
        auto test = std::make_shared<Test>(args);

        _nodeSet.insert(test);

        return test;
    }
};

RUNA_API_HEADER("test2", 0, 0, {})

RUNA_API_INIT(
    printf("Initializing test\n");
    Node::add_type("test2", Test::create);)
