
#include <iostream>
#include <vector>

#include "OnnxMlirRuntime.h"
#include <stdlib.h>
#include <time.h>
// Declare the inference entry point.
extern "C" OMTensorList *run_main_graph(OMTensorList *);

//static float img_data[] = {};

int main() {

  FILE *fp;
  float *array;
  int i, size;
  
  fp = fopen("mobiledata.bin","rb");
  if(!fp) {
    printf("Failed to load\n");
  }

  fseek(fp, 0,SEEK_END);
  size = ftell(fp) / sizeof(float);
  rewind(fp);

  // Allocate memory for the array
  array = (float*)malloc(size * sizeof(float));
  if (!array) {
    printf("Failed to allocate memory\n");
    return 1;
  }

  // Read the binary data into the array
  if (fread(array, sizeof(float), size, fp) != size) {
    printf("Failed to read binary data\n");
    return 1;
  }

  // Close the binary file
  fclose(fp);
  // 784 - mnist
  // 150528 - mobilenet

  static float result[150528] = {};

  // Print the values of the array
  for (i = 0; i < size; i++) {
    result[i] = array[i];
    // printf("%.10f ", array[i]);
  }

  printf("%d\n", sizeof(result) / sizeof(float));

  printf("\n");
  //img_data = result;

  // Create an input tensor list of 1 tensor.
  int inputNum = 1;
  OMTensor **inputTensors = (OMTensor **)malloc(inputNum * sizeof(OMTensor *));
  // The first input is of tensor<1x1x28x28xf32>.
  int64_t rank = 4;
  int64_t shape[] = {1, 3, 224, 224};
  OMTensor *tensor = omTensorCreate(result, shape, rank, ONNX_TYPE_FLOAT);
  // Create a tensor list.
  inputTensors[0] = tensor;
  OMTensorList *tensorListIn = omTensorListCreate(inputTensors, inputNum);
  
  //time check
  clock_t start;
  double time_end;
  start = clock();
  // Compute outputs.
  OMTensorList *tensorListOut = run_main_graph(tensorListIn);
  std::cout << "time consumption: "<<double(clock() - start) / CLOCKS_PER_SEC << std::endl;
  
  // Extract the output. The model defines one output of type tensor<1x10xf32>.
  OMTensor *y = omTensorListGetOmtByIndex(tensorListOut, 0);
  float *prediction = (float *)omTensorGetDataPtr(y);
  std::cout << sizeof(prediction)/ sizeof(float);
  // Analyze the output.
  int digit = -1;
  float prob = 0.;
  for (int i = 0; i < 1000; i++) {
    printf("prediction[%d] = %f\n", i, prediction[i]);
    if (prediction[i] > prob) {
      digit = i;
      prob = prediction[i];
    }
  }

  printf("The digit is %d\n", digit);
  return 0;
}
