import sys
from os import path

from collada import parse
from collada import header

def usage():
    name = sys.argv[0]
    print("usage (source):")
    print(f"  {name} [input_collada.dae] [output_source.cpp] [output_vertex.vtx] [output_vertex.idx]")
    print("usage (header):")
    print(f"  {name} [output_header.hpp]")
    sys.exit(1)

def parse_namespace(filename):
    namespace = path.splitext(path.split(filename)[1])[0]
    return namespace

def main():
    try:
        input_collada = sys.argv[1]
        output_source = sys.argv[2]
        output_vertex = sys.argv[3]
        output_index  = sys.argv[4]
        assert input_collada.lower().endswith(".dae")
        assert output_source.lower().endswith(".cpp")
        assert output_vertex.lower().endswith(".vtx")
        assert output_index.lower().endswith(".idx")
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
