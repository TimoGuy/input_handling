#include "input_ifc.h"

#include <unordered_map>
#include <vector>
#include "input_handling.h"
#include "input_state_set.h"


namespace input_handling
{

static std::unordered_multimap<int32_t, Key_mouse_control> s_key_binding_map;
static std::unordered_multimap<int32_t, Key_mouse_control> s_mousebutton_binding_map;
static vec2 s_prev_mouse_position{ 0.0f, 0.0f };

struct Key_mouse_axis
{
    Key_mouse_control neg_direction;
    Key_mouse_control pos_direction;
    bool neg_pressed{ false };
    bool pos_pressed{ false };

    template<typename T>
    T calculate_value()
    {
        // @NOTE: This is the best branchless way I could think of.
        static_assert(false == 0x00);
        static_assert(true == 0xff);  // @TODO: START HERE!!!!!! For the branchless calc this needs to be true
        constexpr int8_t neg_value{ -1 };
        constexpr int8_t pos_value{ 1 };
        return (neg_value & neg_pressed) + (pos_value & pos_pressed);
    }
};
static std::vector<Key_mouse_axis> s_all_input_axes;
static std::unordered_map<Key_mouse_control, bool*> s_control_to_axis_control_map;

enum
{
    GP_MOVEMENT_HORIZONTAL = 0,
    GP_MOVEMENT_VERTICAL,
    UI_NAVIGATE_MOVEMENT_HORIZONTAL,
    UI_NAVIGATE_MOVEMENT_VERTICAL,
    LE_MOVEMENT_HORIZONTAL,
    LE_MOVEMENT_VERTICAL,
    LE_MOVE_WORLD_Y_AXIS,

    NUM_INPUT_AXES
};

// Internal helpers.
void process_key_mouse_control(Key_mouse_control control, bool is_pressed);

}  // namespace input_handling


void input_handling::init_key_mouse_adapter()
{
    s_all_input_axes.resize(NUM_INPUT_AXES);
    s_all_input_axes[0] = { Key_mouse_control::GP_MOVEMENT_LEFT,          Key_mouse_control::GP_MOVEMENT_RIGHT          };
    s_all_input_axes[1] = { Key_mouse_control::GP_MOVEMENT_DOWN,          Key_mouse_control::GP_MOVEMENT_UP             };
    s_all_input_axes[2] = { Key_mouse_control::UI_NAVIGATE_MOVEMENT_LEFT, Key_mouse_control::UI_NAVIGATE_MOVEMENT_RIGHT };
    s_all_input_axes[3] = { Key_mouse_control::UI_NAVIGATE_MOVEMENT_DOWN, Key_mouse_control::UI_NAVIGATE_MOVEMENT_UP    };
    s_all_input_axes[4] = { Key_mouse_control::LE_MOVEMENT_LEFT,          Key_mouse_control::LE_MOVEMENT_RIGHT          };
    s_all_input_axes[5] = { Key_mouse_control::LE_MOVEMENT_DOWN,          Key_mouse_control::LE_MOVEMENT_UP             };
    s_all_input_axes[6] = { Key_mouse_control::LE_MOVE_WORLD_Y_AXIS_DOWN, Key_mouse_control::LE_MOVE_WORLD_Y_AXIS_UP    };

    s_control_to_axis_control_map = {
        // 0
        { Key_mouse_control::GP_MOVEMENT_LEFT,           &s_all_input_axes[0].neg_pressed },
        { Key_mouse_control::GP_MOVEMENT_RIGHT,          &s_all_input_axes[0].pos_pressed },
        // 1
        { Key_mouse_control::GP_MOVEMENT_DOWN,           &s_all_input_axes[1].neg_pressed },
        { Key_mouse_control::GP_MOVEMENT_UP,             &s_all_input_axes[1].pos_pressed },
        // 2
        { Key_mouse_control::UI_NAVIGATE_MOVEMENT_LEFT,  &s_all_input_axes[2].neg_pressed },
        { Key_mouse_control::UI_NAVIGATE_MOVEMENT_RIGHT, &s_all_input_axes[2].pos_pressed },
        // 3
        { Key_mouse_control::UI_NAVIGATE_MOVEMENT_DOWN,  &s_all_input_axes[3].neg_pressed },
        { Key_mouse_control::UI_NAVIGATE_MOVEMENT_UP,    &s_all_input_axes[3].pos_pressed },
        // 4
        { Key_mouse_control::LE_MOVEMENT_LEFT,           &s_all_input_axes[4].neg_pressed },
        { Key_mouse_control::LE_MOVEMENT_RIGHT,          &s_all_input_axes[4].pos_pressed },
        // 5
        { Key_mouse_control::LE_MOVEMENT_DOWN,           &s_all_input_axes[5].neg_pressed },
        { Key_mouse_control::LE_MOVEMENT_UP,             &s_all_input_axes[5].pos_pressed },
        // 6
        { Key_mouse_control::LE_MOVE_WORLD_Y_AXIS_DOWN,  &s_all_input_axes[6].neg_pressed },
        { Key_mouse_control::LE_MOVE_WORLD_Y_AXIS_UP,    &s_all_input_axes[6].pos_pressed },
    };
}

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
    glm_vec2_add(delta, set.gameplay.camera_delta, set.gameplay.camera_delta);
    glm_vec2_copy(position, set.ui.cursor_position);
    glm_vec2_add(delta, set.level_editor.camera_delta, set.level_editor.camera_delta);
}

void input_handling::report_mouse_scroll_input_change(float_t y_delta)
{
    get_state_set_writing_handle(0).ui.scroll_delta += y_delta;
}

// Internal helpers.
void input_handling::process_key_mouse_control(Key_mouse_control control, bool is_pressed)
{
    if (s_control_to_axis_control_map.find(control) != s_control_to_axis_control_map.end())
    {
        // Update control map axis.
        *s_control_to_axis_control_map.at(control) = is_pressed;
    }

    auto& set{ get_state_set_writing_handle(0) };
    switch (control)
    {
    case Key_mouse_control::GP_MOVEMENT_LEFT:
    case Key_mouse_control::GP_MOVEMENT_RIGHT:
        set.gameplay.movement[0] =
            s_all_input_axes[GP_MOVEMENT_HORIZONTAL].calculate_value<float_t>();
        break;

    case Key_mouse_control::GP_MOVEMENT_UP:
    case Key_mouse_control::GP_MOVEMENT_DOWN:
        set.gameplay.movement[1] =
            s_all_input_axes[GP_MOVEMENT_VERTICAL].calculate_value<float_t>();
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
    case Key_mouse_control::UI_NAVIGATE_MOVEMENT_RIGHT:
        set.ui.navigate_movement[0] =
            s_all_input_axes[UI_NAVIGATE_MOVEMENT_HORIZONTAL].calculate_value<int32_t>();
        break;

    case Key_mouse_control::UI_NAVIGATE_MOVEMENT_UP:
    case Key_mouse_control::UI_NAVIGATE_MOVEMENT_DOWN:
        set.ui.navigate_movement[1] =
            s_all_input_axes[UI_NAVIGATE_MOVEMENT_VERTICAL].calculate_value<int32_t>();
        break;

    case Key_mouse_control::UI_CONFIRM_BOOL:
        set.ui.confirm = is_pressed;
        break;

    case Key_mouse_control::UI_CANCEL_BOOL:
        set.ui.cancel = is_pressed;
        break;

    case Key_mouse_control::LE_MOVEMENT_LEFT:
    case Key_mouse_control::LE_MOVEMENT_RIGHT:
        set.ui.navigate_movement[0] =
            s_all_input_axes[LE_MOVEMENT_HORIZONTAL].calculate_value<float_t>();
        break;

    case Key_mouse_control::LE_MOVEMENT_UP:
    case Key_mouse_control::LE_MOVEMENT_DOWN:
        set.ui.navigate_movement[1] =
            s_all_input_axes[LE_MOVEMENT_VERTICAL].calculate_value<float_t>();
        break;

    case Key_mouse_control::LE_MOVE_WORLD_Y_AXIS_UP:
    case Key_mouse_control::LE_MOVE_WORLD_Y_AXIS_DOWN:
        set.level_editor.move_world_y_axis =
            s_all_input_axes[LE_MOVE_WORLD_Y_AXIS].calculate_value<float_t>();
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
