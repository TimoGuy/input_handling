#pragma once

#include "input_state_set.h"


namespace input_handling
{

void set_num_state_sets(uint32_t count);
Input_state_set& get_state_set(uint32_t idx);
void clear_all_delta_inputs();

}  // namespace input_handling
