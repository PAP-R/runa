#pragma once

#include <functional>
#include <map>
#include <memory>
#include <print>
#include <set>
#include <typeinfo>

#include <SDL3/SDL.h>

#include "AppState.hpp"
#include "Log.hpp"

class Node {
  protected:
    static inline std::shared_ptr<Node>                                                   _root = nullptr;
    static inline std::map<std::string, std::function<std::shared_ptr<Node>(AppState *)>> _createFunctions;

    Node                           *_parent = nullptr;
    std::set<std::shared_ptr<Node>> _children;

    std::string _name;

    bool _inactive = false;

    virtual void _update(AppState *appstate) {}
    virtual void _event(AppState *appstate, SDL_Event *event) {}

    Node() {
        _name = "Node";
        std::println("Creating {}", to_string());
    }

  public:
    static void add_type(const std::string &name, std::function<std::shared_ptr<Node>(AppState *)> create_function) {
        if (!_createFunctions.contains(name)) {
            _createFunctions.insert(std::make_pair(name, create_function));
            Log::println("Added Node type [{}]", name);
            for (auto &t : _createFunctions) {
                Log::println("\t[{}]", t.first);
            }
        } else {
            Log::println("Node type {} already exists", name);
        }
    }

    static void terminate() {
        _createFunctions.clear();
        _root.reset();
    }

    static std::shared_ptr<Node> root() {
        if (_root == nullptr) {
            _root = create();
        }

        return _root;
    }

    virtual ~Node() {
        std::println("Destroying {}", to_string());
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
        if (!_createFunctions.contains(name)) {
            Log::println("Failed to find Node type [{}]", name);
            for (auto &t : _createFunctions) {
                Log::println("\t[{}]", t.first);
            }
            return nullptr;
        }
        std::shared_ptr<Node> node = _createFunctions[name](args...);

        add_child(node);

        return node;
    }

    void enable() {
        this->_inactive = false;
    }

    void disable() {
        this->_inactive = true;
    }

    template <class... Args>
    static inline std::shared_ptr<Node> create(const std::string &name, Args... args) {
        return root()->create_child(name, args...);
    };

    static inline std::shared_ptr<Node> create() {
        return std::shared_ptr<Node>(new Node());
    }

    void update(AppState *appstate) {
        if (_inactive) return;

        for (auto &c : _children) {
            c->update(appstate);
        }

        _update(appstate);
    }

    void event(AppState *appstate, SDL_Event *event) {
        if (_inactive) return;

        for (auto &c : _children) {
            c->event(appstate, event);
        }

        _event(appstate, event);
    }

    virtual std::string to_string() {
        return std::format("{}::{}", typeid(this).name(), _name);
    }
};