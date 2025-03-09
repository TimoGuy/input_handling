#pragma once

#include "input_state_set.h"


namespace input_handling
{

void set_num_state_sets(uint32_t count);
uint32_t get_num_state_sets();
Input_state_set& get_state_set_writing_handle(uint32_t idx);
const Input_state_set& get_state_set_reading_handle(uint32_t idx);

}  // namespace input_handling
