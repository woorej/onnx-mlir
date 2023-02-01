# How to use
https://www.notion.so/woojaejoo/Run-ONNX-Model-in-arm-architecture-12b7741d08244286bc85311a82876f45

# File Guide
## Convert image format to binary
`model_preprocess.py` : Generate binary file  
`read_and_check_binary_file_size.cpp` : Read a binary file and check   
`run.cpp` : Inferece only single image   
`test_mnist.cpp` : Inference all files for mnist  
`test_mobilenet.cpp` : inference all files for mobilenetv2  

# Run
you should have more than one image in your directroy  
support model option list
- mobilenetv2
- mnist
Example
```
$ python3 model_preprocess.py --model {model} --imagedir {image_dir}
$ python3 model_preprocess.py --model mnist --imagedir ./data
```

