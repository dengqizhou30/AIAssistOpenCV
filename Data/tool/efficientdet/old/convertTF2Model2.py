# changed input resolution from 1x28x28 to 1x28x28x1 (NHWC)
# Used the following script to convert SavedModel to a frozen graph:
# https://github.com/opencv/opencv/issues/16879
# https://hub.tensorflow.google.cn/google/imagenet/efficientnet_v2_imagenet21k_ft1k_b3/classification/2

import tensorflow as tf
print(tf.__version__)

from tensorflow.python.framework.convert_to_constants import convert_variables_to_constants_v2
from tensorflow.python.tools import optimize_for_inference_lib

loaded = tf.saved_model.load('./')
infer = loaded.signatures['serving_default']

f = tf.function(infer).get_concrete_function(input_1=tf.TensorSpec(shape=[None, 300, 300, 3], dtype=tf.float32))
f2 = convert_variables_to_constants_v2(f)
graph_def = f2.graph.as_graph_def()

# Remove NoOp nodes
for i in reversed(range(len(graph_def.node))):
    if graph_def.node[i].op == 'NoOp':
        del graph_def.node[i]

for node in graph_def.node:
    for i in reversed(range(len(node.input))):
        if node.input[i][0] == '^':
            del node.input[i]

# Remove a lot of Identity nodes
# graph_def = optimize_for_inference_lib.optimize_for_inference(graph_def,
#                                                               ['input_1'],
#                                                               ['StatefulPartitionedCall/StatefulPartitionedCall/sequential/dense_1/BiasAdd'],
#                                                               tf.float32.as_datatype_enum)

# Export frozen graph
with tf.io.gfile.GFile('./frozen_graph.pb', 'wb') as f:
   f.write(graph_def.SerializeToString())