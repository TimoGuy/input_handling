#include "input_ifc.h"

#include <atomic>
#include "input_handling.h"


namespace input_handling
{

// For marking compiler sections to update memory barriers.
static std::atomic_uint8_t s_reporting_stage{ 0 };

}  // namespace input_handling


void input_handling::start_reporting_frame()
{
    s_reporting_stage = 1;
    clear_all_delta_inputs();
    s_reporting_stage = 2;
}

void input_handling::end_reporting_frame()
{
    s_reporting_stage = 0;
}
