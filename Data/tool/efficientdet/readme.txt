https://github.com/opencv/opencv/pull/17384
https://github.com/google/automl/tree/master/efficientdet#2-pretrained-efficientdet-checkpoints

https://github.com/opencv/opencv/issues/16879

https://github.com/opencv/opencv/wiki/TensorFlow-Object-Detection-API
https://docs.opencv.org/master/d4/d2f/tf_det_tutorial_dnn_conversion.html

https://hub.tensorflow.google.cn/google/imagenet/efficientnet_v2_imagenet21k_ft1k_b3/classification/2
https://github.com/google/automl/tree/master/efficientnetv2






https://github.com/opencv/opencv/pull/17384
直接下载移动版的模型，使用脚本生成配置文件：
https://hub.tensorflow.google.cn/s?q=efficientdet
python3 tf_text_graph_efficientdet.py --input ./efficientdet-lite0/saved_model.pb --output efficientdet-lite0.pbtxt





使用efficientdet项目自己提供的工具,将从ckpt转换为frozen graph格式的模型：
目前opcv对tf2.x支持很差，在opencv改进前，不要浪费时间转换模型，一堆坑。
目前使用efficientdet ft1.x版本比较靠谱：


启动tf1.X容器：
docker pull tensorflow/tensorflow:1.15.5
docker run --name tf1 -it -v /ai:/ai tensorflow/tensorflow:1.15.5 /bin/bash

docker start tf1
docker exec -i -t  tf1 /bin/bash

使用efficientdet自带的工具，
在容器里面执行模型转换脚本：
cd /ai/automl-1.0/efficientdet/
pip install -r requirements.txt
cd /ai/d0/
python3 /ai/automl-1.0/efficientdet/model_inspect.py --runmode=saved_model --model_name=efficientdet-d0  --ckpt_path=efficientdet-d0 --saved_model_dir=savedmodeldir
python3 /ai/automl-1.0/efficientdet/model_inspect.py --hparams="image_size=224x224" --runmode=saved_model --model_name=efficientdet-d0  --ckpt_path=efficientdet-d0 --saved_model_dir=savedmodeldir
python3 /ai/automl-1.0/efficientdet/model_inspect.py --runmode=saved_model --model_name=efficientdet-lite0  --ckpt_path=efficientdet-lite0 --saved_model_dir=savedmodeldir

直接编写一个工具，
将工具efficientdetOptimize.py拷贝到模型目录：
在容器里针对opcv环境优化模型：
pip install tensorflow-model-optimization
cd /ai/d0/
python3 efficientdetOptimize.py

使用opencv提供的工具，
生成模型配置文件，
pip install opencv-python-headless
cd /ai/d0/
python /ai/opencv-master/samples/dnn/tf_text_graph_efficientdet.py --input ./d0.pb --output ./d0.pbtxt






