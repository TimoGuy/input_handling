#pragma once

#include <unordered_map>
#include "cglm/cglm.h"
#include "input_state_set.h"


namespace input_handling
{

void set_key_binding_map(std::unordered_multimap<int32_t, Key_mouse_control>&& key_binding_map);
void set_mousebutton_binding_map(std::unordered_multimap<int32_t, Key_mouse_control>&& mousebutton_binding_map);

void end_reporting_swap_input_buffers();

void report_keyboard_input_change(int32_t key_code, bool is_pressed);
void report_mouse_button_input_change(int32_t button_code, bool is_pressed);
void report_mouse_position_change(vec2 position);
void report_mouse_scroll_input_change(float_t y_delta);

void report_gamepad_input_change();

}  // namespace input_handling
