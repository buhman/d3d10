def find_semantics(inputs, semantic):
    return [i for i in inputs if i.semantic == semantic]

def filter_tiny(fs, epsilon=0.00001):
    return [f if abs(f) > epsilon else 0 for f in fs]

def matrix_transpose(fs):
    return (
        fs[0], fs[4], fs[8],  fs[12],
        fs[1], fs[5], fs[9],  fs[13],
        fs[2], fs[6], fs[10], fs[14],
        fs[3], fs[7], fs[11], fs[15],
    )
