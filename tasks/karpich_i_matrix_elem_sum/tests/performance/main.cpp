#include <gtest/gtest.h>

#include "karpich_i_matrix_elem_sum/common/include/common.hpp"
#include "karpich_i_matrix_elem_sum/mpi/include/ops_mpi.hpp"
#include "karpich_i_matrix_elem_sum/seq/include/ops_seq.hpp"
#include "util/include/perf_test_util.hpp"

namespace karpich_i_matrix_elem_sum {

class KarpichIMatrixElemSumPerfTest : public ppc::util::BaseRunPerfTests<InType, OutType> {
  const int kCount_ = 100;
  InType input_data_{};

  void SetUp() override {
    input_data_ = kCount_;
  }

  bool CheckTestOutputData(OutType &output_data) final {
    return input_data_ == output_data;
  }

  InType GetTestInputData() final {
    return input_data_;
  }
};

TEST_P(KarpichIMatrixElemSumPerfTest, RunPerfModes) {
  ExecuteTest(GetParam());
}

const auto kAllPerfTasks =
    ppc::util::MakeAllPerfTasks<InType, KarpichIMatrixElemSumMPI, KarpichIMatrixElemSumSEQ>(PPC_SETTINGS_karpich_i_matrix_elem_sum);

const auto kGtestValues = ppc::util::TupleToGTestValues(kAllPerfTasks);

const auto kPerfTestName = KarpichIMatrixElemSumPerfTest::CustomPerfTestName;

INSTANTIATE_TEST_SUITE_P(RunModeTests, KarpichIMatrixElemSumPerfTest, kGtestValues, kPerfTestName);

}  // namespace karpich_i_matrix_elem_sum
