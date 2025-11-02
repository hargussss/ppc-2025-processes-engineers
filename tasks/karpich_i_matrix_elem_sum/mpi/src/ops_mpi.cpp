#include "karpich_i_matrix_elem_sum/mpi/include/ops_mpi.hpp"

#include <mpi.h>

#include <numeric>
#include <vector>

#include "karpich_i_matrix_elem_sum/common/include/common.hpp"
#include "util/include/util.hpp"

namespace karpich_i_matrix_elem_sum {

KarpichIMatrixElemSumMPI::KarpichIMatrixElemSumMPI(const InType &in) {
  SetTypeOfTask(GetStaticTypeOfTask());
  GetInput() = in;
  GetOutput() = 0;
}

bool KarpichIMatrixElemSumMPI::ValidationImpl() {
  int n = std::get<0>(GetInput());
  int m = std::get<1>(GetInput());
  std::vector<int> val = std::get<2>(GetInput());

  return (n > 0) && (m > 0) && (val.size() == (n + m));
}

bool KarpichIMatrixElemSumMPI::PreProcessingImpl() {
  return true;
}

bool KarpichIMatrixElemSumMPI::RunImpl() {
  int n = std::get<0>(GetInput());
  int m = std::get<1>(GetInput());
  std::vector<int> val = std::get<2>(GetInput());
  if((n > 0) && (m > 0) && (val.size() == (n + m))) {
    return false;
  }
  int rank = 0
  int mpi_size = 0
  MPI_Comm_rank(&rank, MPI_COMM_WORLD);
  MPI_Comm_soze(&mpi_size, MPI_COMM_WORLD);
  
  std::size_t iter = val.size() / mpi_size;
  std::size_t start = iter * rank;
  std::size_t end = iter * (rank+1);

  if(rank == npi_size - 1) {
    end = val.size();
  }

  long sum = 0;
  for(int i = start; i < end; i++) {
    sum += val[i];
  }

  const long send_sum = sum;
  MPI_Reduce(&send_sum, sum, 1, MPI_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
  MPI_Bcast(&sum, 1, MPI_LONG, 0, MPI_COMM_WOWLD);

  GetOutput() = sum;

  return true;
}

bool KarpichIMatrixElemSumMPI::PostProcessingImpl() {
  GetOutput() -= GetInput();
  return GetOutput() > 0;
}

}  // namespace karpich_i_matrix_elem_sum
