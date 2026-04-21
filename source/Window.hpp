#pragma once

#include <set>
#include <string>

#include <SDL3/SDL.h>

#include "Node.hpp"

class Window : public Node {
  protected:
    static inline std::set<std::shared_ptr<Window>> _window_set;

    SDL_GPUDevice *_device;
    SDL_Window    *_window;

    Window(AppState *appstate, const std::string &title, int width, int height, SDL_WindowFlags flags) {
        _device = appstate->device;
        _window = SDL_CreateWindow(title.c_str(), width, height, flags | SDL_WINDOW_VULKAN);
        SDL_ClaimWindowForGPUDevice(_device, _window);
    }

  public:
    ~Window() {
        SDL_ReleaseWindowFromGPUDevice(_device, _window);
        SDL_DestroyWindow(_window);
    }

    static void destroy(SDL_Window *window) {
        auto w = *std::find_if(_window_set.begin(), _window_set.end(), [window](auto const &w) { return w->_window == window; });
        w.reset();
        _window_set.erase(w);
    }

    static std::shared_ptr<Window>
    create(AppState *appstate, const std::string &title, int width, int height, SDL_WindowFlags flags = 0) {
        auto window = std::shared_ptr<Window>(new Window(appstate, title, width, height, flags));
        _window_set.insert(window);
        return window;
    }
};
