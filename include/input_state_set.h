#pragma once

#include "cglm/cglm.h"


namespace input_handling
{

struct Input_state_set
{
#define INPUT_STATE___VEC2(var_name) vec2 var_name{ 0.0f, 0.0f }
#define INPUT_STATE__IVEC2(var_name) ivec2 var_name{ 0, 0 }
#define INPUT_STATE__FLOAT(var_name) float_t var_name{ 0.0f }
#define INPUT_STATE___BOOL(var_name) bool var_name{ false }

    struct Gameplay
    {
        INPUT_STATE___VEC2(camera_delta);
        INPUT_STATE___VEC2(movement);
        INPUT_STATE___BOOL(jump);
        INPUT_STATE___BOOL(dodge_sprint);

        INPUT_STATE___BOOL(switch_weapon);
        INPUT_STATE___BOOL(attack);
        INPUT_STATE___BOOL(deflect_block);

        INPUT_STATE___BOOL(toggle_lock);
        INPUT_STATE___BOOL(interact);
    } gameplay;

    struct UI
    {
        INPUT_STATE___VEC2(cursor_position);
        INPUT_STATE__FLOAT(scroll_delta);
        INPUT_STATE__IVEC2(navigate_movement);
        INPUT_STATE___BOOL(confirm);
        INPUT_STATE___BOOL(cancel);
    } ui;

    struct Level_editor
    {
        INPUT_STATE___VEC2(camera_delta);
        INPUT_STATE___VEC2(movement);
        INPUT_STATE__FLOAT(move_world_y_axis);
        INPUT_STATE___BOOL(lshift_modifier);
        INPUT_STATE___BOOL(lctrl_modifier);
        INPUT_STATE___BOOL(camera_move);
    } level_editor;

    void clear()
    {
        glm_vec2_zero(gameplay.camera_delta);
        ui.scroll_delta = 0.0f;
        glm_vec2_zero(level_editor.camera_delta);
    }

#undef INPUT_STATE___VEC2
#undef INPUT_STATE__IVEC2
#undef INPUT_STATE__FLOAT
#undef INPUT_STATE___BOOL
};  // struct Input_state_set

enum class Key_mouse_control
{
    // Gameplay.

    // INPUT_STATE___VEC2(camera_delta);  @HARDCODE.

    GP_MOVEMENT_LEFT = 0,
    GP_MOVEMENT_RIGHT,
    GP_MOVEMENT_UP,
    GP_MOVEMENT_DOWN,

    GP_JUMP_BOOL,
    GP_DODGE_SPRINT_BOOL,
    GP_SWITCH_WEAPON_BOOL,
    GP_ATTACK_BOOL,
    GP_DEFLECT_BLOCK_BOOL,
    GP_TOGGLE_LOCK_BOOL,
    GP_INTERACT_BOOL,

    // UI.

    // INPUT_STATE__IVEC2(cursor_position);  @HARDCODE.

    // INPUT_STATE__FLOAT(scroll_delta);  @HARDCODE.

    UI_NAVIGATE_MOVEMENT_LEFT,
    UI_NAVIGATE_MOVEMENT_RIGHT,
    UI_NAVIGATE_MOVEMENT_UP,
    UI_NAVIGATE_MOVEMENT_DOWN,

    UI_CONFIRM_BOOL,
    UI_CANCEL_BOOL,

    // Level editor.

    // INPUT_STATE___VEC2(camera_delta);  @HARDCODE.

    LE_MOVEMENT_LEFT,
    LE_MOVEMENT_RIGHT,
    LE_MOVEMENT_UP,
    LE_MOVEMENT_DOWN,

    LE_MOVE_WORLD_Y_AXIS_UP,
    LE_MOVE_WORLD_Y_AXIS_DOWN,

    LE_LSHIFT_MODIFIER_BOOL,
    LE_LCTRL_MODIFIER_BOOL,
    LE_CAMERA_MOVE_BOOL,

    // Num controls.
    NUM_CONTROLS
};  // enum class Key_mouse_control

}  // namespace input_handling
