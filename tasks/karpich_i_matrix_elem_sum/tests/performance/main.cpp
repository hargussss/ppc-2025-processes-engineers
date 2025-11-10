#include <gtest/gtest.h>

#include <random>

#include "karpich_i_matrix_elem_sum/common/include/common.hpp"
#include "karpich_i_matrix_elem_sum/mpi/include/ops_mpi.hpp"
#include "karpich_i_matrix_elem_sum/seq/include/ops_seq.hpp"
#include "util/include/perf_test_util.hpp"

namespace karpich_i_matrix_elem_sum {

class KarpichIMatrixElemSumPerfTest : public ppc::util::BaseRunPerfTests<InType, OutType> {
  std::size_t n = 10000;
  std::size_t m = 10000;
  long correct_test_output_data_;
  InType input_data_;

  void SetUp() override {
    input_data_ = std::make_tuple(n, m, GenMatrix(n, m, 777));
  }

  bool CheckTestOutputData(OutType &output_data) final {
    return output_data == correct_test_output_data_;
  }

  InType GetTestInputData() final {
    return input_data_;
  }

  std::vector<int> GenMatrix(std::size_t n, std::size_t m, int seed) {
    std::mt19937 gen(seed);
    std::uniform_int_distribution<> idis;
    std::vector<int> res(n * m);
    correct_test_output_data_ = 0;

    for (std::size_t i = 0; i < n * m; i++) {
      res[i] = idis(gen);
      correct_test_output_data_ += res[i];
    }
    return res;
  }
};

TEST_P(KarpichIMatrixElemSumPerfTest, RunPerfModes) {
  ExecuteTest(GetParam());
}

const auto kAllPerfTasks = ppc::util::MakeAllPerfTasks<InType, KarpichIMatrixElemSumMPI, KarpichIMatrixElemSumSEQ>(
    PPC_SETTINGS_karpich_i_matrix_elem_sum);

const auto kGtestValues = ppc::util::TupleToGTestValues(kAllPerfTasks);

const auto kPerfTestName = KarpichIMatrixElemSumPerfTest::CustomPerfTestName;

INSTANTIATE_TEST_SUITE_P(RunModeTests, KarpichIMatrixElemSumPerfTest, kGtestValues, kPerfTestName);
}  // namespace karpich_i_matrix_elem_sum
