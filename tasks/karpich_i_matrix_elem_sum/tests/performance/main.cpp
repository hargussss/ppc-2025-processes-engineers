#include <gtest/gtest.h>

#include "karpich_i_matrix_elem_sum/common/include/common.hpp"
#include "karpich_i_matrix_elem_sum/mpi/include/ops_mpi.hpp"
#include "karpich_i_matrix_elem_sum/seq/include/ops_seq.hpp"
#include "util/include/perf_test_util.hpp"

namespace karpich_i_matrix_elem_sum {

class KarpichIMatrixElemSumPerfTest : public ppc::util::BaseRunPerfTests<InType, OutType> {
  const int kSum_ = 4500;
  const std::string kTestFile = "test_matrix_10_10";
  InType input_data_;

  void SetUp() override {
    std::size_t n = 0;
    std::size_t m = 0;
    
    // Read image
    {
      std::string local = kTestFile + ".txt";
      std::string abs_path = ppc::util::GetAbsoluteTaskPath(PPC_ID_karpich_i_matrix_elem_sum, local);
      std::ifstream file(abs_path);
      if(file.is_open() == false) {
        throw std::runtime_error("Failed to open file: " + abs_path);
      }

      file >> n;
      file >> m;
      std::vector<int> val(n * m);
      for(std::size_t i = 0; i < val.size(); i++) {
        file >> val[i];
      }
      input_data_ = std::make_tuple(n, m, val);
    }
  }

  bool CheckTestOutputData(OutType &output_data) final {
    return output_data == kSum_;
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
