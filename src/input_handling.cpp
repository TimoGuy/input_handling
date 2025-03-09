#include "input_handling.h"

#include <cinttypes>
#include <vector>


namespace input_handling
{

std::vector<Input_state_set> s_all_input_state_sets;

}  // namespace input_handling


void input_handling::set_num_state_sets(uint32_t count)
{
    s_all_input_state_sets.clear();
    s_all_input_state_sets.resize(count, {});
}

input_handling::Input_state_set& input_handling::get_state_set(uint32_t idx)
{
    return s_all_input_state_sets[idx];
}

void input_handling::clear_all_delta_inputs()
{
    for (auto& state_set : s_all_input_state_sets)
        state_set.clear();
}
