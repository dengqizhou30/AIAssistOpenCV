# https://github.com/opencv/opencv/pull/17384

import tensorflow as tf
from tensorflow.python.tools import optimize_for_inference_lib
from tensorflow.tools.graph_transforms import TransformGraph

pb_file = './savedmodeldir/efficientdet-d0_frozen.pb'
#pb_file = './savedmodeldir/efficientdet-lite0_frozen.pb'

graph_def = tf.compat.v1.GraphDef()

try:
    with tf.io.gfile.GFile(pb_file, 'rb') as f:
        graph_def.ParseFromString(f.read())
except:
    with tf.gfile.FastGFile(pb_file, 'rb') as f:
        graph_def.ParseFromString(f.read())

graph_def = optimize_for_inference_lib.optimize_for_inference(graph_def, ['image_arrays'], ['detections'], tf.uint8.as_datatype_enum)
graph_def = TransformGraph(graph_def, ['image_arrays'], ['detections'], ['fold_constants'])

with tf.gfile.FastGFile('d0.pb', 'wb') as f:
   f.write(graph_def.SerializeToString())
