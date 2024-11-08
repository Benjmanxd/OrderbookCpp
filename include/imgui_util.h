#pragma once

#define TextCentered(text) \
  SetHorizontalCentered(text); \
  ImGui::Text(text);

#define ButtonCentered(text) \
  ( SetHorizontalCentered(text), ImGui::Button(text) )

void SetHorizontalCentered(const char*);
