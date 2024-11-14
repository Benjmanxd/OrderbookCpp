#include <string>

#include "imgui.h"

namespace OrderbookApp {
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

    private:
      void Init();
      void Shutdown();

      ApplicationSpecification m_app_spec;
  };
}
