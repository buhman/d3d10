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
    index_table = {}
    next_output_index = 0
    index_buffer = []
    vertex_buffer = []

    used_offsets = [offset for offset in range(p_stride) if offset in by_offset]

    for vertex_ix in range(triangles.count * 3):
        index_table_key = tuple(triangles.p[vertex_ix * p_stride + offset] for offset in used_offsets)
        print(index_table_key)
        if index_table_key in index_table:
            index_buffer.append(index_table[index_table_key])
            continue

        index_table[index_table_key] = next_output_index
        index_buffer.append(next_output_index)
        next_output_index += 1

        # emit vertex attributes for new output index in vertex buffer
        for offset in used_offsets:
            p_index = triangles.p[vertex_ix * p_stride + offset]
            for input, source in by_offset[offset]:
                print(input.semantic, end=" ")
                source_stride = source.technique_common.accessor.stride
                source_index = p_index * source_stride
                array_slice = source.array_element.floats[source_index:source_index+source_stride]
                print(array_slice, end=" ")
                vertex_buffer.extend(array_slice)
        print()

    print("{")
    for i in range(triangles.count):
        print(", ".join(str(index_buffer[i * 3 + j]) for j in range(3)), end=",\n")
    print("}")
    print("{")
    for i in range(len(index_table)):
        print(", ".join(str(vertex_buffer[i * vertex_buffer_stride + j])
                        for j in range(vertex_buffer_stride)), end=",\n")
    print("}")

if __name__ == "__main__":
    import sys
    collada = parse.parse_collada_file(sys.argv[1])

    mesh = collada.library_geometries[0].geometries[0].geometric_element
    assert type(mesh) is types.Mesh
    mesh_vertex_buffer(collada, mesh)
