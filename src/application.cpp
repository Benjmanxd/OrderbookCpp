#include "application.h"

static OrderbookApp::Application* s_Instance = nullptr;

namespace OrderbookApp {
  Application::Application(const ApplicationSpecification& app_spec) : m_app_spec(app_spec) {

  }
}
