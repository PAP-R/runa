#include <set>
#include <string>

#include <SDL3/SDL.h>

#include "Node.hpp"
#include "PluginHeader.hpp"

class Window : public Node {
  protected:
    static inline std::set<std::shared_ptr<Window>> _window_set;

    SDL_GPUDevice *_device;
    SDL_Window    *_window;

  public:
    Window() {
        _name = "window";
    }

    ~Window() {
        SDL_ReleaseWindowFromGPUDevice(_device, _window);
        SDL_DestroyWindow(_window);
    }

    void _update(ArgMap args) override {
        auto            appstate = args.get<AppState *>("AppState");
        SDL_GPUTexture *texture;
        SDL_WaitAndAcquireGPUSwapchainTexture(appstate->command_buffer, _window, &texture, nullptr, nullptr);
    }

    static void destroy(SDL_Window *window) {
        {
            auto w_it = std::find_if(_window_set.begin(), _window_set.end(), [window](auto const &w) { return w->_window == window; });
            if (w_it != _window_set.end()) {
                auto w = *w_it;

                w->remove();

                Log::println(Log::INFO, "Removing {} from window set", w->to_string());
                _window_set.erase(w);
            }
        }
    }

    static std::shared_ptr<Window>
    create(ArgMap args) {
        auto window = std::make_shared<Window>();

        Log::println(Log::INFO, "{}:{} : {} : {}", __FILE__, __LINE__, window->to_string(), window.use_count());

        auto            appstate = args.get<AppState *>("AppState");
        auto            title    = args.get<std::string>("WindowTitle");
        int             width = args.get<int>("WindowWidth"), height = args.get<int>("WindowHeight");
        SDL_WindowFlags flags = args.get<SDL_WindowFlags>("WindowFlags");

        window->_device       = appstate->device;
        window->_window       = SDL_CreateWindow(title.c_str(), width, height, flags | SDL_WINDOW_VULKAN);
        if (!window->_window) {
            Log::println(Log::ERROR, "Failed to create window: {}", SDL_GetError());
            return nullptr;
        }

        SDL_ClaimWindowForGPUDevice(window->_device, window->_window);

        _window_set.insert(window);
        _nodeSet.insert(window);
        return window;
    }

    static void event(SDL_Event *event, ArgMap args = {}) {
        if (event->type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
            auto window = SDL_GetWindowFromEvent(event);
            if (!window) Log::println(Log::ERROR, "Failed to get window {}", SDL_GetError());

            Window::destroy(window);

            if (_window_set.empty()) {
                if (auto appstate = args.get<AppState *>("AppState")) {
                    appstate->should_quit = true;
                }
            }
        }
    }
};

RUNA_HEADER("window", 0, 0, {})

RUNA_INIT(
    Node::add_type("window", Window::create, std::nullopt, Window::event);)