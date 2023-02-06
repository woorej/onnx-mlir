# File Guide
`model_preprocess.py` : Generate binary file from images
`read_and_check_binary_file_size.cpp` : Read a binary file and check   
`run.cpp` : Inferece only single image   
`test_mnist.cpp` : Inference all files for mnist  
`test_mobilenet.cpp` : Inference all files for mobilenetv2  

# Convert image format to binary
You should have at least one image in the directory.
model option list below :
- mobilenetv2
- mnist  

Example
```
$ python3 model_preprocess.py --model {model} --imagedir {image_dir}
$ python3 model_preprocess.py --model mnist --imagedir ./data
```

# run shellscript 
Customize onnx-mlir to run in arm64  
This shellscript can run where llvm-project and onnx-mlir is cloned. Also, llvm-project must be built for usage.
```
$ run.sh <path/to/onnx-mlir> <path/to/llvm_project/> <path/to/onnx_file> <path/to/cpp_main_file>
```
Use the above run.sh method to run. 
This shellscript is ongoing since onnx-mlir is not fully available in arm64. 

## Short Explanation
1. run onnx-mlir and extract .mlir extension written in llvm dialect.
2. Convert into llvmir code by using mlir-translate 
3. use llc from llvm-project and clang to generate so file (pure onnx-mlir also generates so file, so we managed to generate so file too.)
4. compile with g++ with required include paths.
5. run the output generated from compiled result.

Important Thing : Current sh file is only handling before g++ compile(line 4)  
Users need to manually do line 4 and 5 until bug fix.
