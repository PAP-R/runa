#pragma once

#include <any>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <print>
#include <set>
#include <typeinfo>

#include <SDL3/SDL.h>

#include "AppState.hpp"
#include "ArgMap.hpp"
#include "Log.hpp"

class Node {
  protected:
    static inline std::shared_ptr<Node>                  _root = nullptr;
    typedef std::function<std::shared_ptr<Node>(ArgMap)> CreateFunction;
    static inline std::map<std::string, CreateFunction>  _createFunctions;
    typedef std::function<void(ArgMap)>                  UpdateFunction;
    static inline std::map<std::string, UpdateFunction>  _updateFunctions;
    typedef std::function<void(SDL_Event *, ArgMap)>     EventFunction;
    static inline std::map<std::string, EventFunction>   _eventFunctions;

    static inline std::set<std::shared_ptr<Node>> _nodeSet;

    Node                           *_parent = nullptr;
    std::set<std::shared_ptr<Node>> _children;

    std::string _name;

    bool _disabled = false;

    virtual void _update(ArgMap args = {}) {}

    void update(ArgMap args = {}) {
        if (_disabled) return;

        for (auto &c : _children) {
            c->update(args);
        }

        _update(add_args(args));
    }

    virtual void _event(SDL_Event *event, ArgMap args = {}) {}
    void         event(SDL_Event *event, ArgMap args = {}) {
        if (_disabled) return;

        for (auto &c : _children) {
            c->event(event, args);
        }

        _event(event, add_args(args));
    }

    Node() {
        _name = "Node";
        std::println("Creating {}", to_string());
    }

  public:
    static void add_type(const std::string &name, std::optional<CreateFunction> create_function = std::nullopt, std::optional<UpdateFunction> update_function = std::nullopt, std::optional<EventFunction> event_function = std::nullopt) {
        if (!_createFunctions.contains(name) && create_function) {
            _createFunctions.insert(std::make_pair(name, create_function.value()));
        } else {
            Log::println(Log::WARNING, "Node type {} already exists", name);
        }

        if (!_updateFunctions.contains(name) && update_function) {
            _updateFunctions.insert(std::make_pair(name, update_function.value()));
        } else {
            Log::println(Log::WARNING, "Update function for {} already exists", name);
        }

        if (!_eventFunctions.contains(name) && event_function) {
            _eventFunctions.insert(std::make_pair(name, event_function.value()));
        } else {
            Log::println(Log::WARNING, "Event function for {} already exists", name);
        }
    }

    static void remove_type(const std::string &name) {
        _createFunctions.erase(name);
        _updateFunctions.erase(name);
        _eventFunctions.erase(name);
    }

    static void terminate() {
        _root.reset();

        _createFunctions.clear();
        _updateFunctions.clear();
        _eventFunctions.clear();
        _nodeSet.clear();
    }

    static std::shared_ptr<Node> root() {
        if (_root == nullptr) {
            _root        = create();
            _root->_name = "Root";
        }

        return _root;
    }

    static void update(AppState *appstate, ArgMap args = {}) {
        args["AppState"] = appstate;
        for (auto &f : _updateFunctions) {
            f.second(args);
        }
        root()->update(args);
    }

    static void event(SDL_Event *event, AppState *appstate, ArgMap args = {}) {
        args["AppState"] = appstate;

        for (auto &f : _eventFunctions) {
            f.second(event, args);
        }

        root()->event(event, args);
    }

    virtual ~Node() {
        std::println("Destroying {}", to_string());
    }

    void add_child(std::shared_ptr<Node> node) {
        if (node && node->_parent == nullptr) {
            _children.insert(node);
            node->_parent = this;
        }
    }

    void remove_child(Node *node) {
        if (node) {
            Log::println(Log::INFO, "Removing {} from {} Child", node->to_string(), to_string());
            node->_parent = nullptr;
            std::erase_if(_children, [node](auto &n) { return node == n.get(); });
        }
    }

    void remove() {

        if (_parent) _parent->remove_child(this);

        std::erase_if(_nodeSet, [this](auto &n) { return this == n.get(); });
    }

    std::shared_ptr<Node> create_child(const std::string &name, AppState *appstate, ArgMap args = {}) {
        if (!_createFunctions.contains(name)) {
            Log::println(Log::WARNING, "Failed to find Node type [{}]", name);
            for (auto &t : _createFunctions) {
                Log::println(Log::WARNING, "\t[{}]", t.first);
            }
            return nullptr;
        }

        args["AppState"]           = appstate;

        std::shared_ptr<Node> node = _createFunctions[name](args);

        add_child(node);

        return node;
    }

    void enable() {
        this->_disabled = false;
    }

    void disable() {
        this->_disabled = true;
    }

    static inline std::shared_ptr<Node> create(const std::string &name, AppState *appstate, ArgMap args = {}) {
        return root()->create_child(name, appstate, args);
    };

    static inline std::shared_ptr<Node> create() {
        return std::shared_ptr<Node>(new Node());
    }

    virtual ArgMap add_args(ArgMap args) {
        return args;
    }

    virtual std::string to_string() {
        return std::format("{}::{}{}{}", typeid(this).name(), _name, _disabled ? " (Disabled)" : "", _parent == nullptr ? " (Orphan)" : "");
    }

    static void print(const std::string &addition = "") {
        Log::println(Log::INFO, "Nodes: {}", addition);
        for (auto &n : _nodeSet) {
            Log::println(Log::INFO, "\t{} : {}", n->to_string(), n.use_count());
        }
    }
};