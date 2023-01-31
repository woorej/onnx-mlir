#from PIL import Image
import numpy as np
import cv2
import time

image_path = './parrot.jpg'

image = cv2.imread(image_path, 0)  # image read
image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
image_data = cv2.resize(image, (256, 256), interpolation=cv2.INTER_LINEAR).astype(np.float32)  # cv2.INTER_LINEAR
image_data = image_data[16:240, 16:240, :].copy()

image_data = image_data.transpose([2, 0, 1])  # C, H, W
mean = np.array([0.079, 0.05, 0]) + 0.406  # 0.485, 0.456, 0.406
std = np.array([0.005, 0, 0.001]) + 0.224  # 0.229, 0.224, 0.225

for channel in range(image_data.shape[0]):
    image_data[channel, :, :] = (image_data[channel, :, :] / 255 - mean[channel]) / std[channel]  # RGB

#image_data = np.expand_dims(image_data, axis=0)


image_data = np.ascontiguousarray(image_data)
print(image_data.shape)


with open('mobiledata.bin','wb') as f:
    f.write(np.ascontiguousarray(image_data, dtype=np.float32).data)


with open("mobiledata.bin","rb") as f2 :
    array = np.fromfile(f2, dtype='float32')
    print(array.shape)


