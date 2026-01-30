import sys
import os.path

from collada import parse
from collada import header

def usage():
    name = sys.argv[0]
    print("usage (source):")
    print(f"  {name} [input_collada.dae] [output_source.cpp] [output_vertex.vtx] [output_vertex.idx] [output_joints_weights_vertex.vjw] [output_resource.rc] [output_makefile.mk]")
    print("usage (header):")
    print(f"  {name} [output_header.hpp]")
    sys.exit(1)

def parse_namespace(filename):
    namespace = os.path.splitext(os.path.split(filename)[1])[0]
    return namespace

def render_resource_file(state, f):
    for resource_name, path in state.resource_names.items():
        filename = os.path.split(path)[1]
        filename = os.path.splitext(filename)[0]
        f.write(f'{resource_name} RCDATA "image/{filename}.DDS"\r\n'.encode('ascii'))

def escape_space(s):
    def _escape_space(s):
        for c in s:
            if c == ' ':
                yield '\\'
            yield c
    return "".join(_escape_space(s))

def render_makefile(state, f):
    for _, path in state.resource_names.items():
        filename = os.path.split(path)[1]
        filename, ext = os.path.splitext(filename)
        filename = escape_space(filename)

        escaped = escape_space(path)
        escaped_dds = os.path.splitext(filename)

        f.write(f"IMAGES += image/{filename}.DDS\r\n".encode('ascii'))
        f.write(f"image/{filename}.DDS: {escaped}\r\n".encode('ascii'))
        f.write('\ttexconv10.exe -f BC1_UNORM -nologo "$<"\r\n'.encode('ascii'))
        f.write(f'\tmv "$(<:.{ext[1:]}=.DDS)" "$@"\r\n\r\n'.encode('ascii'))

def main():
    try:
        input_collada = sys.argv[1]
        output_source = sys.argv[2]
        output_vertex = sys.argv[3]
        output_index  = sys.argv[4]
        output_joints_weights_vertex = sys.argv[5]
        output_resource = sys.argv[6]
        output_makefile = sys.argv[7]
        assert input_collada.lower().endswith(".dae")
        assert output_source.lower().endswith(".cpp")
        assert output_vertex.lower().endswith(".vtx")
        assert output_index.lower().endswith(".idx")
        assert output_joints_weights_vertex.lower().endswith(".vjw")
        assert output_resource.lower().endswith(".rc")
        assert output_makefile.lower().endswith(".mk")
    except Exception as e:
        usage()

    collada = parse.parse_collada_file(input_collada)
    namespace = parse_namespace(input_collada)
    state, out_source = header.render_all(collada, namespace)

    with open(output_source, 'wb') as f:
        source_buf = out_source.getvalue()
        assert "\r\n" in source_buf
        f.write(source_buf.encode('utf-8'))

    with open(output_vertex, 'wb') as f:
        f.write(state.vertex_buffer.getvalue())

    with open(output_index, 'wb') as f:
        f.write(state.index_buffer.getvalue())

    with open(output_joints_weights_vertex, 'wb') as f:
        f.write(state.joints_weights_vertex_buffer.getvalue())

    with open(output_resource, 'wb') as f:
        render_resource_file(state, f)

    with open(output_makefile, 'wb') as f:
        render_makefile(state, f)

def main_header():
    try:
        output_header = sys.argv[1]
        assert output_header.lower().endswith(".hpp")
    except Exception as e:
        usage()

    namespace = parse_namespace(output_header)
    out_header = header.render_all_hpp(namespace)

    with open(output_header, 'wb') as f:
        header_buf = out_header.getvalue()
        assert "\r\n" in header_buf
        f.write(header_buf.encode('utf-8'))

if __name__ == "__main__":
    if len(sys.argv) == 2:
        main_header()
    else:
        main()
