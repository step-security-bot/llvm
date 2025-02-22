//==------- slm_scatter_dg2_pvc.cpp - DPC++ ESIMD on-device test--------==//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===------------------------------------------------------------------===//
// REQUIRES: gpu-intel-pvc || gpu-intel-dg2
// RUN: %{build} -fsycl-device-code-split=per_kernel -o %t.out
// RUN: %{run} %t.out

// The test verifies esimd::slm_scatter() functions accepting
// optional compile-time esimd::properties.
// The slm_scatter() calls in this test use DG2 or PVC features.

#include "Inputs/scatter.hpp"

int main() {
  auto Q = queue{gpu_selector_v};
  esimd_test::printTestLabel(Q);

  constexpr auto TestFeatures = TestFeatures::PVC;
  bool Passed = true;

  Passed &= testSLM<int8_t, TestFeatures>(Q);
  Passed &= testSLM<int16_t, TestFeatures>(Q);
  if (Q.get_device().has(sycl::aspect::fp16))
    Passed &= testSLM<sycl::half, TestFeatures>(Q);
  Passed &= testSLM<uint32_t, TestFeatures>(Q);
  Passed &= testSLM<float, TestFeatures>(Q);
  Passed &= testSLM<uint64_t, TestFeatures>(Q);
  if (Q.get_device().has(sycl::aspect::fp64))
    Passed &= testSLM<double, TestFeatures>(Q);

  std::cout << (Passed ? "Passed\n" : "FAILED\n");
  return Passed ? 0 : 1;
}
