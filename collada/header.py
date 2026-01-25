from typing import Dict, List

from dataclasses import dataclass
from itertools import islice
from io import BytesIO
import struct

from collada.util import matrix_transpose, find_semantics
from collada import parse
from collada import types
from collada.generate import renderer
from collada import buffer

from prettyprinter import pprint, install_extras
install_extras(include=["dataclasses"])

@dataclass
class State:
    # arbitrary binary data, including vertex buffers and index
    # buffers
    buf: BytesIO

    # geometry__indices:
    #   keys: collada <geometry> id
    #   values: the index the geometry was placed at in the C++ geometries[] array
    geometry__indices: Dict[str, int]

    # geometry__vertex_index_tables:
    #   keys: collada <geometry> id
    #   values: vertex_index_table (see buffer.py)
    geometry__vertex_index_tables: Dict[str, List[int]]

    def __init__(self):
        self.buf = BytesIO()
        self.geometry__indices = {}
        self.geometry__vertex_index_tables = {}

def render_matrix(fs):
    fsi = iter(fs)
    for i in range(4):
        s = ", ".join(f"{f}f" for f in islice(fsi, 4))
        yield f"{s},"

def render_inverse_bind_matrix(collada, skin):
    inverse_bind_matrix_input, = find_semantics(skin.joints.inputs, "INV_BIND_MATRIX")
    inverse_bind_matrix_source = collada.lookup(inverse_bind_matrix_input.source, types.SourceCore)
    stride = inverse_bind_matrix_source.technique_common.accessor.stride
    count = inverse_bind_matrix_source.technique_common.accessor.count
    array = inverse_bind_matrix_source.array_element
    assert type(inverse_bind_matrix_source.array_element) == types.FloatArray
    assert stride == 16
    assert array.count == count * stride

    inverse_bind_matrices = []
    yield "static const float inverse_bind_matrices[] = {"
    for i in range(count):
        offset = stride * i
        matrix = matrix_transpose(array.floats[offset:offset+stride])
        yield from render_matrix(matrix)
    yield "};"

def renderbin(f, elems, t):
    fmt = {
        float: '<f',
        int: '<i',
    }[t]
    for e in elems:
        assert type(e) is t, (e, elems)
        f.write(struct.pack(fmt, e))

def render_input_elements(input_source_table):
    for input, source in input_source_table:
        semantic = input.semantic if input.semantic != "VERTEX" else "POSITION"
        semantic_index = 0 if input.set is None else input.set
        assert all(param.type == 'float' for param in source.technique_common.accessor.params)
        assert type(source.technique_common.accessor.stride) is int
        stride = source.technique_common.accessor.stride

        yield "{"
        yield f'.semantic = "{semantic}",'
        yield f".semantic_index = {semantic_index},"
        yield f".format = input_format::FLOAT{stride},"
        yield "},"

def render_geometry(state, collada, geometry):
    mesh = geometry.geometric_element
    assert type(mesh) is types.Mesh
    vertex_buffer, index_buffer, vertex_index_table, input_source_table = buffer.mesh_vertex_index_buffer(collada, mesh)
    vertex_buffer_offset = state.buf.tell()
    renderbin(state.buf, vertex_buffer, float)
    vertex_buffer_size = state.buf.tell() - vertex_buffer_offset
    index_buffer_offset = state.buf.tell()
    renderbin(state.buf, index_buffer, int)
    index_buffer_size = state.buf.tell() - index_buffer_offset

    state.geometry__vertex_index_tables[geometry.id] = vertex_index_table

    yield "{"
    yield ".mesh = {"
    yield f".input_elements = {{"
    yield from render_input_elements(input_source_table)
    yield "},"
    yield f".input_elements_count = {len(input_source_table)},"
    yield ""
    yield f".vertex_buffer_offset = {vertex_buffer_offset},"
    yield f".vertex_buffer_size = {vertex_buffer_size},"
    yield ""
    yield f".index_buffer_offset = {index_buffer_offset},"
    yield f".index_buffer_size = {index_buffer_size},"
    yield "}"
    yield "},"

def render_library_geometries(state, collada):
    yield "geometry const geometries[] = {"
    next_index = 0
    for library_geometries in collada.library_geometries:
        for geometry in library_geometries.geometries:
            assert geometry.id is not None
            state.geometry__indices[geometry.id] = next_index
            next_index += 1
            yield from render_geometry(state, collada, geometry)
    yield "};"

if __name__ == "__main__":
    import sys
    collada = parse.parse_collada_file(sys.argv[1])

    #skin = collada.library_controllers[0].controllers[0].control_element
    #assert type(skin) is types.Skin
    #foo = render_inverse_bind_matrix(collada, skin)

    state = State()
    render, out = renderer()
    render(render_library_geometries(state, collada))
    print(out.getvalue())
