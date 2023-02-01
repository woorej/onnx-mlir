import os
import argparse
import numpy as np
import cv2

class Mnist() :
    def __init__(self, image_dirs) :
        self.get_image = self.get_image(image_dirs)
        self.binlists = self.preprocess_image(self.get_image)
        self.checkfile(self.binlists)

    def preprocess_image(self, image_paths) :
        bin_dir = []
        for image_path in image_paths :
            image = cv2.imread(image_path, 0) #gray scale image read
            image_data = cv2.resize(image, (28, 28), interpolation=cv2.INTER_LINEAR).astype(np.float32) # cv2.INTER_LINEAR
            # binary file 로 이미지 저장
            path, name = os.path.split(image_path)
            name, _ = os.path.splitext(name)
            save_path = path.split('/')[:-1]
            save_path = '/'.join(save_path)
            save_path = save_path+'/'+'databin'
            try :
                if not os.path.exists(save_path) :
                    os.makedirs(save_path)
            except OSError :
                print (f'Error: Creating directory. {save_path}')
            bin_file_name = os.path.join(save_path, name) + '.bin'

            with open(bin_file_name, 'wb') as f :
                f.write(np.ascontiguousarray(image_data, dtype=np.float32).data)
                bin_dir.append(bin_file_name)    

        return bin_dir

    def checkfile(self, binlists) :
        for binlist in binlists :
            print(binlist)
            with open(binlist,'rb') as f :
                array = np.fromfile(f, dtpye=np.float32)
                print(array.shape)

    def get_image(self, image_dirs) :
        image_list = []
        for roots, dirs, files in os.walk(image_dirs) :
            for file in files :
                image_dir = os.path.join(roots, file)
                if image_dir.endswith("png") or image_dir.endswith("jpg") or image_dir.endswith("JPEG") :
                    image_list.append(image_dir)
        return image_list


class MobileNetv2() :
    def __init__(self, image_dirs) :
        self.get_image = self.get_image(image_dirs)
        self.binlists = self.preprocess_image(self.get_image)
        self.checkfile(self.binlists)

    def preprocess_image(self, image_paths) :
        bin_dir = []
        for image_path in image_paths :
            image = cv2.imread(image_path, 1) #image read
            image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
            image_data = cv2.resize(image, (256, 256), interpolation=cv2.INTER_LINEAR).astype(np.float32) # cv2.INTER_LINEAR
            image_data = image_data[16:240, 16:240, :].copy()

            image_data = image_data.transpose([2, 0, 1]) # C, H, W
            mean = np.array([0.079, 0.05, 0]) + 0.406 # 0.485, 0.456, 0.406
            std = np.array([0.005, 0, 0.001]) + 0.224 #0.229, 0.224, 0.225

            for channel in range(image_data.shape[0]): 
                image_data[channel, :, :] = (image_data[channel, :, :] / 255 - mean[channel]) / std[channel] #RGB

        #image_data = np.expand_dims(image_data, axis=0)
        # binary file 로 이미지 저장
            path, name = os.path.split(image_path)
            name, _ = os.path.splitext(name)
            save_path = path.split('/')[:-1]
            save_path = '/'.join(save_path)
            save_path = save_path+'/'+'databin'
            try :
                if not os.path.exists(save_path) :
                    os.makedirs(save_path)
            except OSError :
                print (f'Error: Creating directory. {save_path}')
            bin_file_name = os.path.join(save_path, name) + '.bin'

            with open(bin_file_name, 'wb') as f :
                f.write(np.ascontiguousarray(image_data, dtype=np.float32).data)
                bin_dir.append(bin_file_name)    

        return bin_dir
        
    def get_image(self, image_dirs) :
        image_list = []
        for roots, dirs, files in os.walk(image_dirs) :
            for file in files :
                image_dir = os.path.join(roots, file)
                if image_dir.endswith("png") or image_dir.endswith("jpg") or image_dir.endswith("JPEG") :
                    image_list.append(image_dir)
        #print(image_list)
        return image_list

    def checkfile(self, binlists) :
        for binlist in binlists :
            with open(binlist,'rb') as f :
                array = np.fromfile(f, dtype='float32')
                if len(array) != 150528 :
                    print(f'ImageFile Error : {binlist}')
                    return

# python3 model_process.py mnist ./data
# python3 model_process.py mobilenetv2 ./data

def main() :
    parser = argparse.ArgumentParser(description="supprot preprocessing model : mnist, mobilenetv2")
    parser.add_argument('--model', required=True, help="put the model name")
    parser.add_argument('--imagedir',required=True, help="put image path")

    args = parser.parse_args()
    model_name = args.model
    image_dirs = args.imagedir

    if not os.path.exists(image_dirs) :
        raise 'Directory not found'

    if model_name == 'mnist' :
        Mnist(image_dirs)
    if model_name == 'mobilenetv2' :
        MobileNetv2(image_dirs)

if __name__ == "__main__" :
    main()
