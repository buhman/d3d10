import sys

from collada import parse
from collada import header

def usage():
    name = sys.argv[0]
    print("usage:")
    print(f"  {name} [input_collada.dae] [output_header.hpp] [output_binary.bin]")
    sys.exit(1)

def main():
    try:
        input_collada = sys.argv[1]
        output_header = sys.argv[2]
        output_binary = sys.argv[3]
        assert input_collada.lower().endswith(".dae")
        assert output_header.lower().endswith(".hpp")
        assert output_binary.lower().endswith(".bin")
    except:
        usage()

    collada = parse.parse_collada_file(input_collada)
    state, out = header.render_all(collada)

    with open(output_header, 'wb') as f:
        header_buf = out.getvalue()
        assert "\r\n" in header_buf
        f.write(header_buf.encode('utf-8'))

    with open(output_binary, 'wb') as f:
        f.write(state.buf.getvalue())

if __name__ == "__main__":
    main()
