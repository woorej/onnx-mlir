#!/bin/bash 

echo "Resuming in arm environment"
echo -e "Current path : ${pwd}"

if [[ $# -ne 4 ]] ; then
  echo "Usage: $0 <path/to/onnx-mlir> <path/to/llvm_project/> <path/to/onnx_file> <path/to/cpp_main_file> "
  echo
  echo "onnx model is supported."
  echo
  echo "All Path should be absolute path."
  echo "All middle file result will be saved in the same directory in onnxfile_path."
  exit 1
fi

onnx_mlir=$1
llvm_project=$2
onnx_file=$3 
cpp_file=$4
parsed_mlir=$(dirname $onnx_file)/parsed.mlir
assembly_file_name=$(dirname $onnx_file)/assemble.s
llvm_file=$(dirname $onnx_file)/middle_llvm.ll
result_file=$(dirname $onnx_file)/result.so
llc="$llvm_project"/build/bin/llc
llvm_include_path="$llvm_project"/llvm
end_file=$(dirname $onnx_file)/result
cpp_compiler=/usr/bin/c++
echo "parsed_mlir=$parsed_mlir"
echo "assembly_file=$assembly_file_name"
echo "llvm_file=$llvm_file"
echo "result_file=$result_file"
echo "llc=$llc"
echo "cpp_file=$cpp_file"
echo "onnx_mlir=$onnx_mlir"
echo "llvm_project=$llvm_project"
echo "llvm_include_path=$llvm_include_path"
echo "cpp_compiler=$cpp_compiler"
echo
echo


echo "Installing package for pulling docker image"
#sudo apt-get update
#sudo apt-get install -y qemu-user-static

echo "Extract llvm dialect result from onnx-docker"
sudo "$onnx_mlir"/docker/onnx-mlir.py --O2 --EmitLLVMIR "$onnx_file"
sudo chmod 777 "$onnx_file".mlir 

echo "Parse and delete device"
python3 remove_targetdevice.py --dir "$onnx_file".mlir

echo "mlir-translate to llvm ir"
echo "$llvm_file"
"$llvm_project"/build/bin/mlir-translate --mlir-to-llvmir "$parsed_mlir" > "$llvm_file"

echo -e "Generating a assembly file name: ${assembly_file_name}"
"$llc" "$llvm_file" -o "$assembly_file_name" 

echo -e "Generating so file name: ${result_file}"
clang -shared "$assembly_file_name" -o "$result_file"

echo "${onnx_mlir}/include/"
echo "${llvm_include_path}/include/"



#$cpp_compiler" --std=c++17 -O3 "$cpp_file" "$result_file" -o output -I "$onnx_mlir"/include/ -I "$llvm_include_path"/include/ -L "$onnx_mlir"/src/Runtime/*.cpp


# need to add function declaration part. 
# need to add llvmir parser to track down target architecture and delete it.
