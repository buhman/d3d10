import sys
import os.path

from collada import parse
from collada import header
from collada import cpp_header
from collada import lua_header

def usage():
    name = sys.argv[0]
    print("usage (source):")
    print(f"  {name} [namespace] [input_collada.dae] [output_source.cpp] [output_position_normal_texture.vtx] [output_joint_weight.vjw] [output_index.idx]") # [output_resource.rc] [output_makefile.mk]
    print()
    print("usage (header):")
    print(f"  {name} [namespace] [output_header.h]")
    sys.exit(1)

def render_resource_file(state, namespace, output_vtx, output_vjw, output_idx, f):
    f.write(f'RES_SCENES_{namespace.upper()}_VTX RCDATA "{output_vtx}"\n'.encode('ascii'))
    f.write(f'RES_SCENES_{namespace.upper()}_VJW RCDATA "{output_vjw}"\n'.encode('ascii'))
    f.write(f'RES_SCENES_{namespace.upper()}_IDX RCDATA "{output_idx}"\n'.encode('ascii'))
    f.write(b"\n")
    for resource_name, path in state.resource_names.items():
        filename = os.path.split(path)[1]
        filename = os.path.splitext(filename)[0]
        f.write(f'{resource_name} RCDATA "image/{filename}.DDS"\n'.encode('ascii'))

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

        f.write(f"IMAGES += image/{filename}.DDS\n".encode('ascii'))
        f.write(f"image/{filename}.DDS: {escaped}\n".encode('ascii'))
        f.write(f"\t@mkdir -p image\n".encode('ascii'))
        f.write('\ttexconv10.exe -f BC1_UNORM -nologo "$<"\n'.encode('ascii'))
        f.write(f'\tmv "$(<:.{ext[1:]}=.DDS)" "$@"\n'.encode('ascii'))
        f.write(b"\n")

def main():
    try:
        namespace = sys.argv[1]
        input_collada = sys.argv[2]
        output_source = sys.argv[3]
        output_position_normal_texture = sys.argv[4]
        output_joint_weight = sys.argv[5]
        output_index  = sys.argv[6]
        #output_resource = sys.argv[7]
        #output_makefile = sys.argv[8]
        assert input_collada.lower().endswith(".dae")
        assert output_source.lower().endswith(".cpp") or output_source.lower().endswith(".lua")
        assert output_position_normal_texture.lower().endswith(".vtx")
        assert output_joint_weight.lower().endswith(".vjw")
        assert output_index.lower().endswith(".idx")
        #assert output_resource.lower().endswith(".rc")
        #assert output_makefile.lower().endswith(".mk")
    except Exception as e:
        usage()

    collada = parse.parse_collada_file(input_collada)

    if output_source.lower().endswith(".cpp"):
        header.lang_header = cpp_header
    elif output_source.lower().endswith(".lua"):
        header.lang_header = lua_header
    else:
        assert False
    state, out_source = header.render_all(collada, namespace, input_collada)

    with open(output_source, 'wb') as f:
        source_buf = out_source.getvalue()
        assert "\n" in source_buf
        f.write(source_buf.encode('utf-8'))

    with open(output_position_normal_texture, 'wb') as f:
        f.write(state.vertex_buffer.getvalue())

    with open(output_index, 'wb') as f:
        f.write(state.index_buffer.getvalue())

    with open(output_joint_weight, 'wb') as f:
        f.write(state.joint_weight_vertex_buffer.getvalue())

    #with open(output_resource, 'wb') as f:
    #    render_resource_file(state, namespace, output_position_normal_texture, output_joint_weight, output_index, f)

    #with open(output_makefile, 'wb') as f:
    #    render_makefile(state, f)

def main_header():
    try:
        namespace = sys.argv[1]
        output_header = sys.argv[2]
        assert output_header.lower().endswith(".h")
    except Exception as e:
        usage()

    header.lang_header = cpp_header

    out_header = header.render_all_hpp(namespace)

    with open(output_header, 'wb') as f:
        header_buf = out_header.getvalue()
        assert "\n" in header_buf
        f.write(header_buf.encode('utf-8'))

if __name__ == "__main__":
    if len(sys.argv) == 3:
        main_header()
    else:
        main()
