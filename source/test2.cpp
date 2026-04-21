#include "Log.hpp"
#include "Node.hpp"
#include "PluginHeader.hpp"

struct Test : public Node {
    int number = 5;

    Test() {
        _name = "Test2";
    }

    ~Test() {
        std::println("Tschüss dies war ein test2 {}", this->number);
    }

    void _update(AppState *appstate) override {
        Log::println("Dies ist ein update2");
    }

    void _event(AppState *appstate, SDL_Event *event) override {
        Log::println("Dies ist ein event");
    }

    static std::shared_ptr<Test> create() {
        return std::make_shared<Test>();
    }
};

RUNA_API_HEADER("test2", 0, 0, {})

RUNA_API_INIT(
    printf("Initializing test\n");
    Node::add_type("test2", Test::create);)
