from collections import defaultdict
from itertools import chain, islice

from collada import parse
from collada import types

from prettyprinter import pprint, install_extras
install_extras(include=["dataclasses"])

def resolve_input_source(collada, source_uri):
    return element

def find_semantics(inputs, semantic):
    return [i for i in inputs if i.semantic == semantic]

def mesh_vertex_buffer(collada, mesh):
    semantic_names = ["NORMAL", "TEXCOORD"]

    assert len(mesh.primitive_elements) == 1
    triangles, = mesh.primitive_elements
    assert type(triangles) is types.Triangles

    vertex_input, = find_semantics(triangles.inputs, "VERTEX")
    vertices = collada.lookup(vertex_input.source, types.Vertices)
    position_input, = find_semantics(vertices.inputs, "POSITION")
    position_source = collada.lookup(position_input.source, types.SourceCore)
    assert type(position_source.array_element) is types.FloatArray

    by_offset = defaultdict(list)
    by_offset[vertex_input.offset].append((vertex_input, position_source))

    for semantic_name in semantic_names:
        for input in find_semantics(triangles.inputs, semantic_name):
            source = collada.lookup(input.source, types.SourceCore)
            assert type(source.array_element) is types.FloatArray
            by_offset[input.offset].append((input, source))

    max_offset = max(i.offset for i in triangles.inputs)
    p_stride = max_offset + 1
    assert len(triangles.p) == triangles.count * 3 * p_stride

    ######################################################################
    # generate the index/vertex buffer
    ######################################################################
    vertex_buffer_stride = sum(
        source.technique_common.accessor.stride
        for input, source in chain.from_iterable(by_offset.values())
    )
    vertex_table = []
    index_table = {}
    next_output_index = 0
    index_buffer = []
    vertex_buffer = []

    used_offsets = [offset for offset in range(p_stride) if offset in by_offset]

    for vertex_ix in range(triangles.count * 3):
        index_table_key = tuple(triangles.p[vertex_ix * p_stride + offset] for offset in used_offsets)
        if index_table_key in index_table:
            index_buffer.append(index_table[index_table_key])
            continue

        index_table[index_table_key] = next_output_index
        index_buffer.append(next_output_index)
        next_output_index += 1

        # emit vertex attributes for new output index in vertex buffer
        for offset in used_offsets:
            p_index = triangles.p[vertex_ix * p_stride + offset]
            if offset == vertex_input.offset:
                vertex_table.append(p_index)
            for input, source in by_offset[offset]:
                source_stride = source.technique_common.accessor.stride
                source_index = p_index * source_stride
                array_slice = source.array_element.floats[source_index:source_index+source_stride]
                vertex_buffer.extend(array_slice)

    """
    print("{")
    for i in range(triangles.count):
        print(", ".join(str(index_buffer[i * 3 + j]) for j in range(3)), end=",\n")
    print("}")
    print("{")
    for i in range(len(index_table)):
        print(", ".join(str(vertex_buffer[i * vertex_buffer_stride + j])
                        for j in range(vertex_buffer_stride)), end=",\n")
    print("}")
    """
    # vertex table:
    # list indices: (output/direct3d) vertex indices
    # list values: (input/collada) vertex indices
    return vertex_table

def filter_tiny(fs, epsilon=0.00001):
    return [f if abs(f) > epsilon else 0 for f in fs]

def matrix_transpose(fs):
    return (
        fs[0], fs[4], fs[8],  fs[12],
        fs[1], fs[5], fs[9],  fs[13],
        fs[2], fs[6], fs[10], fs[14],
        fs[3], fs[7], fs[11], fs[15],
    )

def matrix_print(fs):
    for i, f in enumerate(fs):
        print(f"{f:5.01f}f", end=", ")
        if i % 4 == 3:
            print()

def skin_vertex_buffer(collada, skin, vertex_table):
    inverse_bind_matrix_input, = find_semantics(skin.joints.inputs, "INV_BIND_MATRIX")
    inverse_bind_matrix_source = collada.lookup(inverse_bind_matrix_input.source, types.SourceCore)
    stride = inverse_bind_matrix_source.technique_common.accessor.stride
    count = inverse_bind_matrix_source.technique_common.accessor.count
    array = inverse_bind_matrix_source.array_element
    assert type(inverse_bind_matrix_source.array_element) == types.FloatArray
    assert stride == 16
    assert array.count == count * stride

    # enable to improve inverse bind matrix human-readability
    #floats = filter_tiny(array.floats)

    inverse_bind_matrices = []
    print("static const float inverse_bind_matrices[] = {")
    for i in range(count):
        offset = stride * i
        matrix = matrix_transpose(floats[offset:offset+stride])
        matrix_print(matrix)
        if i + 1 < count:
            print()
    print("};")

    ######################################################################
    # vertex weights
    ######################################################################
    max_offset = max(i.offset for i in skin.vertex_weights.inputs)
    weights_input, = find_semantics(skin.vertex_weights.inputs, "WEIGHT")
    weights_source = collada.lookup(weights_input.source, types.SourceCore)
    joints_input, = find_semantics(skin.vertex_weights.inputs, "JOINT")
    joints_source = collada.lookup(joints_input.source, types.SourceCore)
    assert weights_source.technique_common.accessor.stride == 1
    assert joints_source.technique_common.accessor.stride == 1

    vertex_weights = defaultdict(int)

    v_stride = max_offset + 1
    v_offset = 0
    vertex_influences = []
    for vcount in skin.vertex_weights.vcount:
        influences = []
        for vi in range(vcount):
            joint_index = skin.vertex_weights.v[v_offset + joints_input.offset]
            weight_index = skin.vertex_weights.v[v_offset + weights_input.offset]
            pprint(weights_source)
            weight = weights_source.array_element.floats[weight_index]
            influences.append((joint_index, weight))
            v_offset += v_stride
        vertex_influences.append(influences)

    vertex_buffer = []
    for vertex_index in vertex_table:
        influences = vertex_influences[vertex_index]
        def emit(column):
            for i in range(4):
                if i >= len(influences):
                    vertex_buffer.append(0)
                else:
                    vertex_buffer.append(influences[i][column])
        emit(0) # emit joint int4
        emit(1) # emit weight float4

    for i, v in enumerate(vertex_buffer):
        print(v, end=", ")
        if i % 8 == 7:
            print()

if __name__ == "__main__":
    import sys
    collada = parse.parse_collada_file(sys.argv[1])

    mesh = collada.library_geometries[0].geometries[0].geometric_element
    assert type(mesh) is types.Mesh
    vertex_table = mesh_vertex_buffer(collada, mesh)

    skin = collada.library_controllers[0].controllers[0].control_element
    assert type(skin) is types.Skin
    skin_vertex_buffer(collada, skin, vertex_table)
