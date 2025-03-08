#pragma once

#include "cglm/cglm.h"


namespace input_handling
{

struct Input_state_set
{
    struct Gameplay
    {
        vec2 camera_delta{ 0.0f, 0.0f };
        vec2 movement{ 0.0f, 0.0f };
        bool jump{ false };
        bool dodge_sprint{ false };
        
        bool switch_weapon{ false };
        bool attack{ false };
        bool deflect_block{ false };

        bool toggle_lock{ false };
        bool interact{ false };
    } gameplay;

    struct UI
    {
        ivec2 cursor_position{ 0, 0 };
        ivec2 navigate{ 0, 0 };
        bool confirm{ false };
        bool cancel{ false };
    } ui;

    struct Level_editor
    {
        float_t world_y_axis{ 0.0f };
        bool lshift_modifier{ false };
    } level_editor;
};

class Input_handling_ifc
{
public:
    virtual void report_input_change() = 0;
};

}  // namespace input_handling
