#include "karpich_i_matrix_elem_sum/mpi/include/ops_mpi.hpp"

#include <mpi.h>

#include <vector>

#include "karpich_i_matrix_elem_sum/common/include/common.hpp"

namespace karpich_i_matrix_elem_sum {

KarpichIMatrixElemSumMPI::KarpichIMatrixElemSumMPI(const InType &in) {
  SetTypeOfTask(GetStaticTypeOfTask());
  GetInput() = in;
  GetOutput() = 0;
}

bool KarpichIMatrixElemSumMPI::ValidationImpl() {
  std::size_t n = std::get<0>(GetInput());
  std::size_t m = std::get<1>(GetInput());
  std::vector<int> val = std::get<2>(GetInput());

  return (n > 0) && (m > 0) && (val.size() == (n * m));
}

bool KarpichIMatrixElemSumMPI::PreProcessingImpl() {
  return true;
}

bool KarpichIMatrixElemSumMPI::RunImpl() {
  std::size_t n = std::get<0>(GetInput());
  std::size_t m = std::get<1>(GetInput());
  std::vector<int> val = std::get<2>(GetInput());
  if (((n > 0) && (m > 0) && (val.size() == (n * m))) == false) {
    return false;
  }
  int rank = 0;
  int mpi_size = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);

  std::size_t iter = val.size() / mpi_size;
  std::size_t start = iter * rank;
  std::size_t end = iter * (rank + 1);

  if (rank == mpi_size - 1) {
    end = val.size();
  }

  long sum = 0;
  for (std::size_t i = start; i < end; i++) {
    sum += val[i];
  }

  const long send_sum = sum;
  MPI_Reduce(&send_sum, &sum, 1, MPI_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
  MPI_Bcast(&sum, 1, MPI_LONG, 0, MPI_COMM_WORLD);

  std::string out = std::to_string(rank) + ": " + std::to_string(start) + " " + std::to_string(end) + " " +
                    std::to_string(sum) + "\n";
  std::cout << out;
  GetOutput() = sum;

  return true;
}

bool KarpichIMatrixElemSumMPI::PostProcessingImpl() {
  return true;
}

}  // namespace karpich_i_matrix_elem_sum
