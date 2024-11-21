#include <GLFW/glfw3.h>
#include <string>
#include <unordered_map>

#include "core/orderbook.h"

namespace OrderbookApp {
  using namespace OrderbookCore;

  struct ApplicationSpecification {
    std::string Name = "Orderbook App";
    uint32_t Width = 1200;
    uint32_t Height = 800;
  };

  class Application {
    public:
      Application(const ApplicationSpecification& app_spec = ApplicationSpecification());
      ~Application();
      static Application& Get();
      void Run();
      void Close();

    private:
      void Init();
      void Shutdown();

      ApplicationSpecification m_app_spec;
      GLFWwindow* m_window_handle { nullptr };
      bool m_running { false };
      std::unordered_map<const char*, Orderbook*> m_orderbook_map;
      static inline const char* m_symbols[] { "apple", "netflix", "google", "meta", "morgan stanley" };
  };
}
