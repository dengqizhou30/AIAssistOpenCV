https://github.com/opencv/opencv/pull/17384
https://github.com/google/automl/tree/master/efficientdet#2-pretrained-efficientdet-checkpoints

https://github.com/opencv/opencv/issues/16879

https://github.com/opencv/opencv/wiki/TensorFlow-Object-Detection-API
https://docs.opencv.org/master/d4/d2f/tf_det_tutorial_dnn_conversion.html

https://hub.tensorflow.google.cn/google/imagenet/efficientnet_v2_imagenet21k_ft1k_b3/classification/2
https://github.com/google/automl/tree/master/efficientnetv2


当前opencv使用tensorflow2.x模型说明：
1、需要先把tensorflow2.x格式的模型文件，转换为frozen graph格式的模型文件；
2、基于frozen graph格式的模型文件生成配置文件；



1、把tensorflow2.x格式的模型文件，转换为frozen graph格式的模型文件：
运行tf2.x环境，使用转换工具convertTF2Model2.py转换模型：

启动tf2.X容器：
docker pull tensorflow/tensorflow:latest-jupyter
docker run --name tf2 -it -v /ai:/ai tensorflow/tensorflow:latest-jupyter /bin/bash
docker exec -i -t  tf2 /bin/bash

修改convertTF2Model2.py里面如下内容，并把这些文件拷贝到tf2.x模型所在目录下：
修改get_concrete_function函数里面的input名称和shape形状。

在容器里面执行模型转换脚本：
cd /ai/b3-v2/
python convertTF2Model2.py

在容器里测试模型加载：
pip install opencv-python-headless
python testModel.py


2、基于frozen graph格式的模型文件生成配置文件；
目前opencv的这个工具python文件，只支持tf1.x环境：

启动tf1.X容器：
docker pull tensorflow/tensorflow:1.15.5
docker run --name tf1 -it -v /ai:/ai tensorflow/tensorflow:1.15.5 /bin/bash
docker exec -i -t  tf1 /bin/bash

pip install opencv-python-headless
cd /ai/b3-v2/
python /ai/opencv-master/samples/dnn/tf_text_graph_efficientdet.py --input ./frozen_graph.pb --output ./b3-v2.pbtxt

python testModel.py

python /ai/opencv-master/samples/dnn/tf_text_graph_efficientdet.py --input /ai/b3/frozen_graph.pb --output /ai/b3/b3.pbtxt

docker exec -i -t  53de77c926fa /bin/bash


python
import argparse
import re
from math import sqrt
import cv2 as cv
import tensorflow as tf
from tensorflow.tools.graph_transforms import TransformGraph



pip install argparse
pip install re
pip install math
pip install tf_text_graph_common




https://github.com/opencv/opencv/issues/16879




https://github.com/opencv/opencv/wiki/TensorFlow-Object-Detection-API
https://docs.opencv.org/master/d4/d2f/tf_det_tutorial_dnn_conversion.html




https://hub.docker.com/r/openvino/workbench
https://software.intel.com/content/www/cn/zh/develop/tools/openvino-toolkit.html
https://hub.tensorflow.google.cn/


docker pull openvino/workbench

docker run -p 5665:5665 --name workbench --privileged -v /dev/bus/usb:/dev/bus/usb -v /dev/dri:/dev/dri -e PORT=5665 -e PROXY_HOST_ADDRESS=0.0.0.0 -it openvino/workbench:latest

docker run -p 5665:5665 --name workbench --privileged -v /dev/bus/usb:/dev/bus/usb -v /dev/dri:/dev/dri -e PORT=5665 -e PROXY_HOST_ADDRESS=0.0.0.0 -it openvino/workbench:latest

docker start workbench


http://127.0.0.1:5665/

efficientnet
EfficientNet V2
https://hub.tensorflow.google.cn/google/imagenet/efficientnet_v2_imagenet21k_ft1k_b3/classification/2
https://github.com/google/automl/tree/master/efficientnetv2



docker pull tensorflow/tensorflow:1.15.5

docker run -it -v /data/ai:/ai tensorflow/tensorflow:1.15.5 /bin/bash

python /ai/opencv-master/samples/dnn/tf_text_graph_efficientdet.py --input /ai/b0/tfhub_module.pb --output /ai/b0/b0.pbtxt

python tf_text_graph_ssd.py --input frozen_inference_graph.pb --config pipeline.config --output ssd_mobilenet_v3.pbtxt


https://docs.opencv.org/master/d4/d2f/tf_det_tutorial_dnn_conversion.html

