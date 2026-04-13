#pragma once

#include <functional>
#include <map>
#include <memory>
#include <print>
#include <set>
#include <typeinfo>

#include <SDL3/SDL.h>

class Node {
  protected:
    static inline std::shared_ptr<Node>                                         _root = nullptr;
    static inline std::map<std::string, std::function<std::shared_ptr<Node>()>> _creationFunctions;

    Node                           *_parent = nullptr;
    std::set<std::shared_ptr<Node>> _children;

    std::string _name;

    bool _inactive = false;

    virtual void _update() {}
    virtual void _event(SDL_Event *event) {}

    Node() {
        _name = "Node";
        std::println("Creating {}", to_string());
    }

  public:
    virtual int test(int new_number) { return 0; };

    static void load(const std::string &name) {
        auto obj = SDL_LoadObject(name.c_str());

        if (obj) {
            auto create = (Node * (*)()) SDL_LoadFunction(obj, "create");
            if (create) {
                _creationFunctions.insert(std::make_pair(name, [create]() { return std::shared_ptr<Node>(create()); }));
            } else {
                std::println("Failed to find function: {}", SDL_GetError());
            }
        } else {
            std::println("Failed to find library: {}", SDL_GetError());
        }
    }

    static std::shared_ptr<Node> root() {
        if (_root == nullptr) {
            _root = create();
        }

        return _root;
    }

    void add_child(std::shared_ptr<Node> node) {
        if (node && node->_parent == nullptr) {
            _children.emplace(node);
            node->_parent = this;
        }
    }

    void remove_child(std::shared_ptr<Node> node) {
        if (node) {
            _children.erase(node);
            node->_parent = nullptr;
        }
    }

    template <class... Args>
    std::shared_ptr<Node> create_child(const std::string &name, Args... args) {
        if (!_creationFunctions.contains(name)) return nullptr;
        std::shared_ptr<Node> node = _creationFunctions[name](args...);

        add_child(node);

        return node;
    }

    template <class... Args>
    static inline std::shared_ptr<Node> create(const std::string &name, Args... args) {
        return root()->create_child(name, args...);
    };

    static inline std::shared_ptr<Node> create() {
        return std::shared_ptr<Node>(new Node());
    }

    void update() {
        if (_inactive) return;

        for (auto &c : _children) {
            c->update();
        }

        _update();
    }

    void event(SDL_Event *event) {
        if (_inactive) return;

        for (auto &c : _children) {
            c->event(event);
        }

        _event(event);
    }

    virtual ~Node() {
        std::println("Destroying {}", to_string());
    }

    virtual std::string to_string() {
        return std::format("{}::{}", typeid(this).name(), _name);
    }
};