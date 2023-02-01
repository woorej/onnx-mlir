
#include <iostream>
#include <vector>
#include <string.h>
#include <dirent.h>


#include "OnnxMlirRuntime.h"
#include <stdlib.h>
#include <time.h>
// Declare the inference entry point.
extern "C" OMTensorList *run_main_graph(OMTensorList *);

#define MAX_FILES 100

//static float img_data[] = {};


int main() {

  DIR *dir;
  struct dirent *ent;
  char *files[MAX_FILES];
  int i = 0;
  FILE *fp;
  float *array;
  int t, size;

  char opd[100] = "./databin";

  if ((dir = opendir(opd)) != NULL) {
  /* print all the files and directories within directory */
  while ((ent = readdir (dir)) != NULL) {
      files[i] = (char *)malloc(strlen(ent->d_name) + 1);
      strcpy(files[i], ent->d_name);
      i++;
  }
  closedir (dir);
  } else {
  /* could not open directory */
  perror("");
  return EXIT_FAILURE;
  }

  // std::filesystem::path p = "foo.c";
  // std::cout << "Current path is " << std::filesystem::current_path() << '\n';
  // std::cout << "Absolute path for " << p << " is " << fs::absolute(p) << '\n';

  char filedir[100];
  strcpy(filedir, opd); // ./databin
  //printf("%s\n",filedir);

  for (int j = 0; j < i; j++) {
	  //printf("%s\n", files[j]); //parrot.jpg
	  if(strcmp(files[j],".")==0 || strcmp(files[j],"..")==0){
            continue;
          }
	  strcat(filedir, "/"); // ./databin/
	  strcat(filedir, files[j]); // ./databin/parrot.bin
	  printf("%s\n",filedir);

	  fp = fopen(filedir, "rb");
	  if (!fp) {
	      printf("Failed to open the binary file\n");
	      return 1;
	  }

	  // Determine the size of the binary file
	  fseek(fp, 0, SEEK_END);
	  size = ftell(fp) / sizeof(float); //same as array.shape
	  //printf("size: %d\n",int(size));

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

	  float result[size] = {};

	  int len;
	  // Print the values of the array
	  for (t = 0; t < size; t++) {
	      result[t] = array[t];
	      //len = sizeof(result[t]) / sizeof(float);
	      //printf("%.10f", result[t]);
	  }
	  printf("%d\n", sizeof(result) / sizeof(float));

	  strcpy(filedir, opd);

	  





	  // Create an input tensor list of 1 tensor.
	  int inputNum = 1;
	  OMTensor **inputTensors = (OMTensor **)malloc(inputNum * sizeof(OMTensor *));
	  // The first input is of tensor<1x1x28x28xf32>.
	  int64_t rank = 4;
	  int64_t shape[] = {1, 1, 28, 28};
	  OMTensor *tensor = omTensorCreate(result, shape, rank, ONNX_TYPE_FLOAT);
	  // Create a tensor list.
	  inputTensors[0] = tensor;
	  OMTensorList *tensorListIn = omTensorListCreate(inputTensors, inputNum);
	  
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
	  for (int i = 0; i < 10; i++) {
	    printf("prediction[%d] = %f\n", i, prediction[i]);
	    if (prediction[i] > prob) {
	      digit = i;
	      prob = prediction[i];
	    }
	  }

	  printf("The digit is %d\n", digit);
	  free(files[j]);
  }
  return 0;
}
