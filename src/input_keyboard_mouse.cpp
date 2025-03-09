#include "input_ifc.h"

#include <unordered_map>
#include "input_handling.h"
#include "input_state_set.h"


namespace input_handling
{

static std::unordered_multimap<int32_t, Key_mouse_control> s_key_binding_map;
static std::unordered_multimap<int32_t, Key_mouse_control> s_mousebutton_binding_map;
static vec2 s_prev_mouse_position{ 0.0f, 0.0f };

// Internal helpers.
void process_key_mouse_control(Key_mouse_control control, bool is_pressed);

}  // namespace input_handling


void input_handling::set_key_binding_map(std::unordered_multimap<int32_t, Key_mouse_control>&& key_binding_map)
{
    s_key_binding_map.clear();
    s_key_binding_map = std::move(key_binding_map);
}

void input_handling::set_mousebutton_binding_map(std::unordered_multimap<int32_t, Key_mouse_control>&& mousebutton_binding_map)
{
    s_mousebutton_binding_map.clear();
    s_mousebutton_binding_map = std::move(mousebutton_binding_map);
}

void input_handling::report_keyboard_input_change(int32_t key_code, bool is_pressed)
{
    auto range{ s_key_binding_map.equal_range(key_code) };
    for (auto it = range.first; it != range.second; it++)
        process_key_mouse_control(it->second, is_pressed);
}

void input_handling::report_mouse_button_input_change(int32_t button_code, bool is_pressed)
{
    auto range{ s_mousebutton_binding_map.equal_range(button_code) };
    for (auto it = range.first; it != range.second; it++)
        process_key_mouse_control(it->second, is_pressed);
}

void input_handling::report_mouse_position_change(vec2 position)
{
    vec2 delta;
    glm_vec2_sub(position, s_prev_mouse_position, delta);
    glm_vec2_copy(position, s_prev_mouse_position);

    auto& set{ get_state_set_writing_handle(0) };
    glm_vec2_copy(delta, set.gameplay.camera_delta);
    glm_vec2_copy(position, set.ui.cursor_position);
    glm_vec2_copy(delta, set.level_editor.camera_delta);
}

void input_handling::report_mouse_scroll_input_change(float_t y_delta)
{
    get_state_set_writing_handle(0).ui.scroll_delta = y_delta;
}

// Internal helpers.
void input_handling::process_key_mouse_control(Key_mouse_control control, bool is_pressed)
{
    auto& set{ get_state_set_writing_handle(0) };
    switch (control)
    {
    case Key_mouse_control::GP_MOVEMENT_LEFT:
        set.gameplay.movement[0] = (is_pressed ? -1.0f : 0.0f);
        break;

    case Key_mouse_control::GP_MOVEMENT_RIGHT:
        set.gameplay.movement[0] = (is_pressed ? 1.0f : 0.0f);
        break;

    case Key_mouse_control::GP_MOVEMENT_UP:
        set.gameplay.movement[1] = (is_pressed ? 1.0f : 0.0f);
        break;

    case Key_mouse_control::GP_MOVEMENT_DOWN:
        set.gameplay.movement[1] = (is_pressed ? -1.0f : 0.0f);
        break;

    case Key_mouse_control::GP_JUMP_BOOL:
        set.gameplay.jump = is_pressed;
        break;

    case Key_mouse_control::GP_DODGE_SPRINT_BOOL:
        set.gameplay.dodge_sprint = is_pressed;
        break;

    case Key_mouse_control::GP_SWITCH_WEAPON_BOOL:
        set.gameplay.switch_weapon = is_pressed;
        break;

    case Key_mouse_control::GP_ATTACK_BOOL:
        set.gameplay.attack = is_pressed;
        break;

    case Key_mouse_control::GP_DEFLECT_BLOCK_BOOL:
        set.gameplay.deflect_block = is_pressed;
        break;

    case Key_mouse_control::GP_TOGGLE_LOCK_BOOL:
        set.gameplay.toggle_lock = is_pressed;
        break;

    case Key_mouse_control::GP_INTERACT_BOOL:
        set.gameplay.interact = is_pressed;
        break;

    case Key_mouse_control::UI_NAVIGATE_MOVEMENT_LEFT:
        set.ui.navigate_movement[0] = (is_pressed ? -1 : 0);
        break;

    case Key_mouse_control::UI_NAVIGATE_MOVEMENT_RIGHT:
        set.ui.navigate_movement[0] = (is_pressed ? 1 : 0);
        break;

    case Key_mouse_control::UI_NAVIGATE_MOVEMENT_UP:
        set.ui.navigate_movement[1] = (is_pressed ? 1 : 0);
        break;

    case Key_mouse_control::UI_NAVIGATE_MOVEMENT_DOWN:
        set.ui.navigate_movement[1] = (is_pressed ? -1 : 0);
        break;

    case Key_mouse_control::UI_CONFIRM_BOOL:
        set.ui.confirm = is_pressed;
        break;

    case Key_mouse_control::UI_CANCEL_BOOL:
        set.ui.cancel = is_pressed;
        break;

    case Key_mouse_control::LE_MOVEMENT_LEFT:
        set.level_editor.movement[0] = (is_pressed ? -1.0f : 0.0f);
        break;

    case Key_mouse_control::LE_MOVEMENT_RIGHT:
        set.level_editor.movement[0] = (is_pressed ? 1.0f : 0.0f);
        break;

    case Key_mouse_control::LE_MOVEMENT_UP:
        set.level_editor.movement[1] = (is_pressed ? 1.0f : 0.0f);
        break;

    case Key_mouse_control::LE_MOVEMENT_DOWN:
        set.level_editor.movement[1] = (is_pressed ? -1.0f : 0.0f);
        break;

    case Key_mouse_control::LE_MOVE_WORLD_Y_AXIS_UP:
        set.level_editor.move_world_y_axis = (is_pressed ? 1.0f : 0.0f);
        break;

    case Key_mouse_control::LE_MOVE_WORLD_Y_AXIS_DOWN:
        set.level_editor.move_world_y_axis = (is_pressed ? -1.0f : 0.0f);
        break;

    case Key_mouse_control::LE_LSHIFT_MODIFIER_BOOL:
        set.level_editor.lshift_modifier = is_pressed;
        break;

    case Key_mouse_control::LE_LCTRL_MODIFIER_BOOL:
        set.level_editor.lctrl_modifier = is_pressed;
        break;

    case Key_mouse_control::LE_CAMERA_MOVE_BOOL:
        set.level_editor.camera_move = is_pressed;
        break;

    default:
        assert(false);  // Control is unsupported.
        break;
    }
}
