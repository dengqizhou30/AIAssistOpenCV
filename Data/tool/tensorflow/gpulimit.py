import tensorflow.compat.v1 as tf

def create_serialized_options(fraction, growth):
    config = tf.ConfigProto()
    config.gpu_options.per_process_gpu_memory_fraction = fraction
    config.gpu_options.allow_growth = growth
    serialized = config.SerializeToString()
    return '{' + ','.join(list(map(hex, serialized))) + '}'

# Example with 30% and enable memory growth
# {0x32,0xb,0x9,0x33,0x33,0x33,0x33,0x33,0x33,0xd3,0x3f,0x20,0x1}
print(create_serialized_options(fraction=0.3, growth=True))