#pragma once

#include <string>
#include <tuple>
#include <vector>

#include "task/include/task.hpp"

namespace karpich_i_matrix_elem_sum {

using InType = std::tuple<std::size_t, std::size_t, std::vector<int>>;
using OutType = long;
using TestType = std::tuple<std::string, long>;
using BaseTask = ppc::task::Task<InType, OutType>;

}  // namespace karpich_i_matrix_elem_sum
