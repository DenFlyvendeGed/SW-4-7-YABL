#!/bin/python
import sys
import os

PATH = os.path.dirname(os.path.abspath(__file__))

OUT_FILE_C = PATH + "/const-code.c"
OUT_FILE_H = PATH + "/const-code.h"
WRITE_ESCAPE = "//#\n"


def parse(str_name, out_file, in_path):
    write = False
    out_file.write("const char* " + str_name + " = \"")
    with open(in_path, "r") as input:
        for line in input:
            if write and line != WRITE_ESCAPE:
                for c in line:
                    if c == '\\':
                        out_file.write('\\\\')
                    elif c == '"':
                        out_file.write('\\"')
                    elif c == '\n':
                        out_file.write('\\n')
                    elif c == '\t':
                        out_file.write('    ')
                    else:
                        out_file.write(c)
            elif line == WRITE_ESCAPE:
                write = not write

    out_file.write("\";\n")


if __name__ == "__main__":
    with open(OUT_FILE_C, "w+") as out_file_c,\
         open(OUT_FILE_H, "w+") as out_file_h:
        for arg in sys.argv[1:]:
            file = arg
            parse(arg[27:-2].upper(), out_file_c, file)
            out_file_h.write("extern const char* " + arg[27:-2].upper() + ";\n")
