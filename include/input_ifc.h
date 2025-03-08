#pragma once

#include "cglm/cglm.h"


namespace input_handling
{

void report_keyboard_input_change(int32_t key_code, bool is_pressed);
void report_mouse_position_change(vec2 position);
void report_mouse_button_input_change(int32_t key_code, bool is_pressed);
void report_mouse_scroll_input_change(float_t y_delta);
void report_gamepad_input_change();

}  // namespace input_handling
