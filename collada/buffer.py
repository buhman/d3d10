from collections import defaultdict
from itertools import chain, islice

from collada import parse
from collada import types
from collada.util import find_semantics

def linearize_offset_table(by_offset, p_stride):
    for offset in range(p_stride):
        for input, source in by_offset[offset]:
            yield offset, input, source

mesh_semantic_names = ["NORMAL", "TEXCOORD"]

def build_offset_table(collada, triangles, p_stride):
    vertex_input, = find_semantics(triangles.inputs, "VERTEX")
    vertices = collada.lookup(vertex_input.source, types.Vertices)
    position_input, = find_semantics(vertices.inputs, "POSITION")
    position_source = collada.lookup(position_input.source, types.SourceCore)
    assert type(position_source.array_element) is types.FloatArray

    by_offset = defaultdict(list)
    by_offset[vertex_input.offset].append((vertex_input, position_source))

    for semantic_name in mesh_semantic_names:
        for input in find_semantics(triangles.inputs, semantic_name):
            source = collada.lookup(input.source, types.SourceCore)
            assert type(source.array_element) is types.FloatArray
            by_offset[input.offset].append((input, source))

    assert len(triangles.p) == triangles.count * 3 * p_stride

    used_offsets = [i for i in range(p_stride) if i in by_offset]
    linearized_table = linearize_offset_table(by_offset, p_stride)
    return list(linearized_table), used_offsets

def mesh_vertex_index_buffer(collada, mesh):
    assert len(mesh.primitive_elements) == 1
    triangles, = mesh.primitive_elements
    assert type(triangles) is types.Triangles

    max_offset = max(i.offset for i in triangles.inputs)
    p_stride = max_offset + 1
    offset_table, used_offsets = build_offset_table(collada, triangles, p_stride)

    ######################################################################
    # generate the index and vertex buffers
    ######################################################################

    vertex_buffer_stride = sum(
        source.technique_common.accessor.stride
        for offset, input, source in offset_table
    )
    vertex_index_table = []
    index_table = {}
    next_output_index = 0
    index_buffer = []
    vertex_buffer = []

    for vertex_ix in range(triangles.count * 3):
        index_table_key = tuple(triangles.p[vertex_ix * p_stride + offset] for offset in used_offsets)
        if index_table_key in index_table:
            index_buffer.append(index_table[index_table_key])
            continue

        index_table[index_table_key] = next_output_index
        index_buffer.append(next_output_index)
        next_output_index += 1

        # emit vertex attributes for new output index in vertex buffer
        for offset, input, source in offset_table:
            p_index = triangles.p[vertex_ix * p_stride + offset]
            if input.semantic == "VERTEX":
                vertex_index_table.append(p_index)

            source_stride = source.technique_common.accessor.stride
            source_index = p_index * source_stride
            array_slice = source.array_element.floats[source_index:source_index+source_stride]
            vertex_buffer.extend(array_slice)

    assert len(index_buffer) == triangles.count * 3
    assert len(vertex_buffer) == len(index_table) * vertex_buffer_stride

    input_source_table = [(input, source) for offset, input, source in offset_table]

    # vertex_index_table: input/collada vertex indices in the order written to the index buffer
    # input_source_table: (input, source) in the order written to the vertex buffer
    return vertex_buffer, index_buffer, vertex_index_table, input_source_table

def skin_vertex_buffer(collada, skin, vertex_index_table):
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
            weight = weights_source.array_element.floats[weight_index]
            influences.append((joint_index, weight))
            v_offset += v_stride
        assert len(influences) <= 4, len(influences)
        vertex_influences.append(influences)

    ######################################################################
    # generate the joint/weight buffer
    ######################################################################

    vertex_buffer = []
    # vertex_index_table: input/collada vertex indices in the order written to the index buffer
    for vertex_index in vertex_index_table:
        influences = vertex_influences[vertex_index]
        def emit(column):
            for i in range(4):
                if i >= len(influences):
                    vertex_buffer.append(0)
                else:
                    vertex_buffer.append(influences[i][column])
        emit(0) # emit joint int4
        emit(1) # emit weight float4
    return vertex_buffer

if __name__ == "__main__":
    import sys
    collada = parse.parse_collada_file(sys.argv[1])

    mesh = collada.library_geometries[0].geometries[0].geometric_element
    assert type(mesh) is types.Mesh
    vertex_buffer_pnt, index_buffer, vertex_index_table, input_source_table = mesh_vertex_index_buffer(collada, mesh)

    skin = collada.library_controllers[0].controllers[0].control_element
    assert type(skin) is types.Skin
    vertex_buffer_jw = skin_vertex_buffer(collada, skin, vertex_index_table)
