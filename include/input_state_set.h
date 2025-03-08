#pragma once

#include "cglm/cglm.h"
#define INPUT_STATE___VEC2(var_name) vec2 var_name{ 0.0f, 0.0f }
#define INPUT_STATE__IVEC2(var_name) ivec2 var_name{ 0, 0 }
#define INPUT_STATE__FLOAT(var_name) float_t var_name{ 0.0f }
#define INPUT_STATE___BOOL(var_name) bool var_name{ false }


namespace input_handling
{

struct Input_state_set
{
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
        INPUT_STATE__IVEC2(cursor_position);
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
        INPUT_STATE___BOOL(camera_move);
    } level_editor;
};

}  // namespace input_handling


#undef INPUT_STATE___VEC2
#undef INPUT_STATE__IVEC2
#undef INPUT_STATE__FLOAT
#undef INPUT_STATE___BOOL
