#include "app/application.h"

int main(void) {
  OrderbookApp::ApplicationSpecification spec;
  spec.Name = "Orderbook Application";

  OrderbookApp::Application app{spec};
  app.Run();
  app.Close();
}
