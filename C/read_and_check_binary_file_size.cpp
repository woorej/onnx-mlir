#include <stdio.h>
#include <stdlib.h>


int main() {
  FILE *fp;
  float *array;
  int i, size;

  // Open the binary file for reading
  fp = fopen("data2.bin", "rb");
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

  // Print the values of the array
  for (i = 0; i < size; i++) {
    result[i] = array[i];
    //printf("%.10f ", array[i]);
  }

  printf("%d\n", sizeof(result) / sizeof(float));

  // Free the memory
  free(array);

  return 0;
}