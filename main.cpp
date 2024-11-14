#include <GLFW/glfw3.h>
#include <stdexcept>
#include <string>

#if defined(__linux__)
#define OS_LINUX
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#elif defined(_WIN32)
#define OS_WINDOWS
#elif defined (__APPLE__) && defined(__MACH__)
#define OS_MACOS
#endif

#include "imgui.h"
#include "imgui_util.h"
#include "orderbook.h"
#include "types.h"


int main(void) {
  if (!glfwInit())
    return 1;

  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
  GLFWwindow *window = glfwCreateWindow(1200, 800, "Orderbook Application", nullptr, nullptr);
  if (window == nullptr)
    return 1;
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1); // Enable vsync

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(window, true); // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
  ImGui_ImplOpenGL3_Init();

  Orderbook orderbook;

  while (!glfwWindowShouldClose(window)) {
    // Poll and handle events (inputs, window resize, etc.)
    // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
    // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
    // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the
    // keyboard data. Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
    glfwPollEvents();
    if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0) {
      ImGui_ImplGlfw_Sleep(10);
      continue;
    }

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    {
      ImGui::SetNextWindowPos(ImVec2(0, 0));
      ImGui::SetNextWindowSize(ImVec2(400, 400));
      ImGui::Begin("Add Order");

      static const char *current_order_side = nullptr;
      if (ImGui::BeginCombo("Side", current_order_side, ImGuiComboFlags_HeightRegular)) {
        for (int n = 0; n < IM_ARRAYSIZE(SideItems); ++n) {
          bool is_selected = (current_order_side == SideItems[n]);
          if (ImGui::Selectable(SideItems[n], is_selected))
            current_order_side = SideItems[n];
          if (is_selected)
            ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
      }
      ImGui::NewLine();

      static const char *current_order_type = nullptr;
      if (ImGui::BeginCombo("Order Type", current_order_type)) {
        for (int n = 0; n < IM_ARRAYSIZE(OrderTypeItems); ++n) {
          bool is_selected = (current_order_type == OrderTypeItems[n]);
          if (ImGui::Selectable(OrderTypeItems[n], is_selected))
            current_order_type = OrderTypeItems[n];
          if (is_selected)
            ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
      }
      ImGui::NewLine();

      static char current_order_quantity_input[10] = {'0', };
      ImGui::InputText("Order Quantity", current_order_quantity_input, sizeof(current_order_quantity_input), ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_EscapeClearsAll);
      ImGui::NewLine();

      static char current_order_price_input[10] = {'0', };
      ImGui::InputText("Order Price", current_order_price_input, sizeof(current_order_price_input), ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_EscapeClearsAll);
      ImGui::NewLine();

      if (ImGui::Button("Submit Order!")) {
        bool input_valid = current_order_side && current_order_type;
        try {
          int current_order_price = std::stoi(current_order_price_input);
          int current_order_quantity = std::stoi(current_order_quantity_input);
          if (!current_order_price || !current_order_quantity) input_valid = false;
        } catch (const std::invalid_argument& err) {
          input_valid = false;
        }

        if (!input_valid) {
          ImGui::SetNextWindowPos(ImVec2(400, 400));
          ImGui::SetNextWindowSize(ImVec2(200, 100));
          ImGui::OpenPopup("invalid_input");
        } else {
          orderbook.AddOrder(OrderFactory::CreateOrder(current_order_side, current_order_type, std::stoi(current_order_quantity_input), std::stoi(current_order_price_input)));
          current_order_side = nullptr;
          current_order_type = nullptr;
          snprintf(current_order_quantity_input, 0, "0");
          snprintf(current_order_price_input, 0, "0");
          ImGui::SetNextWindowPos(ImVec2(200, 200));
          ImGui::SetNextWindowSize(ImVec2(400, 300));
          ImGui::OpenPopup("order_received");
        }
      }

      if (ImGui::BeginPopupModal("invalid_input")) {
        TextCentered("Invalid Input!");
        if (ButtonCentered("Ok")) {
          ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
      }

      if (ImGui::BeginPopupModal("order_received")) {
        TextCentered("Order Received!");
        if (ButtonCentered("Ok")) {
          ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
      }
      ImGui::End();
    }

    {
      ImGui::SetNextWindowPos(ImVec2(400, 0));
      ImGui::SetNextWindowSize(ImVec2(800, 800));
      ImGui::Begin("Orderbook"); // Create a window called "Hello, world!" and append into it.

      ImGui::Text("Hi this part is for the orderbook preview");

      ImGui::SameLine();

      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

      if (ImGui::BeginTable("Orderbook Preview", 3)) {
        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::Text("Asks");
        ImGui::TableNextColumn();
        ImGui::Text("Price");
        ImGui::TableNextColumn();
        ImGui::Text("Bids");

        auto [asks, bids] = orderbook.GetLevelInfos();
        for (auto& ask_level : asks) {
          ImGui::TableNextRow();
          ImGui::TableNextColumn();
          ImGui::Text("%d", ask_level.m_quantity);
          ImGui::TableNextColumn();
          ImGui::Text("%d", ask_level.m_price);
          ImGui::TableNextColumn();
        }
        for (auto& bid_level : bids) {
          ImGui::TableNextRow();
          ImGui::TableNextColumn();
          ImGui::TableNextColumn();
          ImGui::Text("%d", bid_level.m_price);
          ImGui::TableNextColumn();
          ImGui::Text("%d", bid_level.m_quantity);
        }
        ImGui::EndTable();
      }
      ImGui::End();
    }

    // Rendering
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
      GLFWwindow *backup_current_context = glfwGetCurrentContext();
      ImGui::UpdatePlatformWindows();
      ImGui::RenderPlatformWindowsDefault();
      glfwMakeContextCurrent(backup_current_context);
    }

    glfwMakeContextCurrent(window);
    glfwSwapBuffers(window);
  }

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}
