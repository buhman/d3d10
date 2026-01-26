import sys
from os import path

from collada import parse
from collada import header

def usage():
    name = sys.argv[0]
    print("usage:")
    print(f"  {name} [input_collada.dae] [output_header.hpp] [output_vertex.vtx] [output_vertex.idx]")
    sys.exit(1)

def main():
    try:
        input_collada = sys.argv[1]
        output_header = sys.argv[2]
        output_vertex = sys.argv[3]
        output_index  = sys.argv[4]
        assert input_collada.lower().endswith(".dae")
        assert output_header.lower().endswith(".hpp")
        assert output_vertex.lower().endswith(".vtx")
        assert output_index.lower().endswith(".idx")
    except Exception as e:
        usage()

    collada = parse.parse_collada_file(input_collada)
    namespace = path.splitext(path.split(input_collada)[1])[0]
    state, out = header.render_all(collada, namespace)

    with open(output_header, 'wb') as f:
        header_buf = out.getvalue()
        assert "\r\n" in header_buf
        f.write(header_buf.encode('utf-8'))

    with open(output_vertex, 'wb') as f:
        f.write(state.vertex_buffer.getvalue())

    with open(output_index, 'wb') as f:
        f.write(state.index_buffer.getvalue())

if __name__ == "__main__":
    main()
