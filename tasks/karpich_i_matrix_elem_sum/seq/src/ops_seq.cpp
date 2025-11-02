#include "karpich_i_matrix_elem_sum/seq/include/ops_seq.hpp"

#include <numeric>
#include <vector>

#include "karpich_i_matrix_elem_sum/common/include/common.hpp"
#include "util/include/util.hpp"

namespace karpich_i_matrix_elem_sum {
KarpichIMatrixElemSumSEQ::KarpichIMatrixElemSumSEQ(const InType &in) {
  SetTypeOfTask(GetStaticTypeOfTask());
  GetInput() = in;
  GetOutput() = 0;
}

bool KarpichIMatrixElemSumSEQ::ValidationImpl() {
  int n = std::get<0>(GetInput());
  int m = std::get<1>(GetInput());
  std::vector<int> val = std::get<2>(GetInput());

  return (n > 0) && (m > 0) && (val.size() == (n + m));
}

bool KarpichIMatrixElemSumSEQ::PreProcessingImpl() {
  return true;
}

bool KarpichIMatrixElemSumSEQ::RunImpl() {
  // if (GetInput() == 0) {
  //   return false;
  // }
  int n = std::get<0>(GetInput());
  int m = std::get<1>(GetInput());
  std::vector<int> val = std::get<2>(GetInput());
  if((n > 0) && (m > 0) && (val.size() == (n + m))) {
    return false;
  }

  long sum = 0;
  for(int i = 0; i < val.size(); i++) {
    sum += val[i];
  }
  GetOutput() = sum;
  return true;
}

bool KarpichIMatrixElemSumSEQ::PostProcessingImpl() {
  return true;
}

}  // namespace karpich_i_matrix_elem_sum
