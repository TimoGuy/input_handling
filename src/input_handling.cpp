#include "input_handling.h"

#include <atomic>
#include <array>
#include <cinttypes>
#include <vector>
#include "input_ifc.h"


namespace input_handling
{

// This +0 is the read buffer, this +1 is the write buffer.
std::atomic_uint8_t s_buffer_offset{ 0 };
constexpr uint8_t k_num_buffers{ 3 };
std::array<std::vector<Input_state_set>, k_num_buffers> s_all_input_state_sets;

}  // namespace input_handling


void input_handling::set_num_state_sets(uint32_t count)
{
    s_buffer_offset = 0;
    for (auto& state_set : s_all_input_state_sets)
    {
        state_set.clear();
        state_set.resize(count, {});
    }
}

uint32_t input_handling::get_num_state_sets()
{
    return static_cast<uint32_t>(s_all_input_state_sets[0].size());
}

input_handling::Input_state_set& input_handling::get_state_set_writing_handle(uint32_t idx)
{
    return s_all_input_state_sets[(s_buffer_offset + 1) % k_num_buffers][idx];
}

const input_handling::Input_state_set& input_handling::get_state_set_reading_handle(uint32_t idx)
{
    return s_all_input_state_sets[s_buffer_offset % k_num_buffers][idx];
}

// From `input_ifc.h`.
void input_handling::end_reporting_swap_input_buffers()
{
    uint32_t num_sets{ get_num_state_sets() };

    // Copy writing buffer to next buffer.
    for (uint32_t i = 0; i < num_sets; i++)
    {
        auto next_writing_buffer_idx{
            (s_buffer_offset + 2) % k_num_buffers };

        s_all_input_state_sets[next_writing_buffer_idx][i] =
            get_state_set_writing_handle(i);
    }

    // Move writing buffer to reading buffer.
    s_buffer_offset++;

    // Clear next writing buffer.
    for (uint32_t i = 0; i < num_sets; i++)
    {
        get_state_set_writing_handle(i).clear();
    }
}
