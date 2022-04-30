# changed input resolution from 1x28x28 to 1x28x28x1 (NHWC)
# Used the following script to convert SavedModel to a frozen graph:
# https://github.com/opencv/opencv/issues/16879
# https://hub.tensorflow.google.cn/google/imagenet/efficientnet_v2_imagenet21k_ft1k_b3/classification/2

import numpy as np
import cv2 as cv

print(cv.__file__)

net = cv.dnn.readNet('./frozen_graph.pb')
inp = np.random.standard_normal([1, 3, 300, 300]).astype(np.float32)
net.setInput(inp)
out = net.forward()
print(out.shape)