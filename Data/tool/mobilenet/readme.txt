https://github.com/opencv/opencv/pull/16760
https://github.com/tensorflow/models/tree/master/research/object_detection
https://github.com/tensorflow/models/blob/master/research/object_detection/g3doc/tf1_detection_zoo.md
https://github.com/tensorflow/models/blob/master/research/object_detection/g3doc/tf2_detection_zoo.md


https://github.com/opencv/opencv/wiki/TensorFlow-Object-Detection-API
https://docs.opencv.org/master/d4/d2f/tf_det_tutorial_dnn_conversion.html



https://github.com/tensorflow/models/blob/master/research/object_detection/g3doc/tf1_detection_zoo.md


mobilenet还可以在官方下载tf1版本的模型，可以直接用模型生成opencv配置文件：


启动tf1.X容器：
docker pull tensorflow/tensorflow:1.15.5
docker run --name tf1 -it -v /ai:/ai tensorflow/tensorflow:1.15.5 /bin/bash

docker start tf1
docker exec -i -t  tf1 /bin/bash

使用opencv提供的工具，
生成模型配置文件，

cd /ai/ssd_mobilenet_v3_large_coco_2020_01_14/
python /ai/opencv-master/samples/dnn/tf_text_graph_ssd.py --input frozen_inference_graph.pb --config pipeline.config --output ssd_mobilenet_v3_large.pbtxt

cd /ai/ssd_mobilenet_v3_small_coco_2020_01_14
python /ai/opencv-master/samples/dnn/tf_text_graph_ssd.py --input frozen_inference_graph.pb --config pipeline.config --output ssd_mobilenet_v3_small.pbtxt
