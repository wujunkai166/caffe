// The main caffe test code. Your test cpp code should include this hpp
// to allow a main function to be compiled into the binary.

#include "caffe/caffe.hpp"
#include "caffe/test/test_caffe_main.hpp"

#ifdef CPU_ONLY

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  ::google::InitGoogleLogging(argv[0]);

  // invoke the test.
  return RUN_ALL_TESTS();
}

#elif defined USE_OCL

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  ::google::InitGoogleLogging(argv[0]);

  caffe::Caffe::SetDevice(0);
  cout << "OpenCL - selected first GPU device : "
       << caffe::Caffe::cl_state().get_device() << endl;
  caffe::ClDeviceProperties cl_prop = caffe::Caffe::cl_state().get_properties();
  cout << cl_prop.name << endl;
  cout << cl_prop.vendor << endl;
  cout << cl_prop.version << endl;

  // invoke the test.
  int r = RUN_ALL_TESTS();

  // Call explicitly for OCL + FFT
  caffe::Caffe::TeardownDevice(0);

  return r;
}

#else

namespace caffe {
  cudaDeviceProp CAFFE_TEST_CUDA_PROP;
}

using caffe::CAFFE_TEST_CUDA_PROP;

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  caffe::GlobalInit(&argc, &argv);
  // Before starting testing, let's first print out a few cuda defice info.
  int device;
  cudaGetDeviceCount(&device);
  cout << "Cuda number of devices: " << device << endl;
  if (argc > 1) {
    // Use the given device
    device = atoi(argv[1]);
    cudaSetDevice(device);
    cout << "Setting to use device " << device << endl;
  } else if (CUDA_TEST_DEVICE >= 0) {
    // Use the device assigned in build configuration; but with a lower priority
    device = CUDA_TEST_DEVICE;
  }
  cudaGetDevice(&device);
  cout << "Current device id: " << device << endl;
  cudaGetDeviceProperties(&CAFFE_TEST_CUDA_PROP, device);
  // invoke the test.
  return RUN_ALL_TESTS();
}

#endif
