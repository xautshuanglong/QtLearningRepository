# -*- coding: utf-8 -*-

import os, stat

exclude_dirs = [".idea", "Generated", "__pycache__"]
cwd = os.getcwd()
watched_dirs = list()
watched_dirs.append(cwd)


def translate_ui_to_py(in_filename):
    out_dir = os.path.join(cwd, "Generated")
    temp_filename = os.path.basename(in_filename)
    filename_without_ext = os.path.splitext(temp_filename)[0]
    out_filename = os.path.join(out_dir, filename_without_ext + "_ui.py")

    in_file_stat = os.stat(in_filename)
    if os.path.exists(out_filename):
        out_file_stat = os.stat(out_filename)
    if not os.path.exists(out_filename) or in_file_stat.st_mtime > out_file_stat.st_mtime:
        print("\tpyuic5 %s -o %s --import-from=Generated" % (in_filename, out_filename))
        os.system("pyuic5 %s -o %s --import-from=Generated" % (in_filename, out_filename))


if __name__ == '__main__':

    print("================================= PyUIC =================================")

    while len(watched_dirs) > 0:
        watched_dir = watched_dirs.pop(0)
        files = os.listdir(watched_dir)
        child_dirs = list()
        for filename in files:
            real_filename = os.path.join(watched_dir, filename)
            file_stat = os.stat(real_filename)
            if os.path.isfile(real_filename):
                extension = os.path.splitext(filename)
                if len(extension) > 1 and extension[1] == '.ui':
                    print("\ttranslate ui file : %s" % real_filename)
                    translate_ui_to_py(real_filename)
            elif os.path.isdir(real_filename):
                if not filename in exclude_dirs:
                    child_dirs.append(real_filename)
            else:
                print("Unknown file type ...")
        watched_dirs = child_dirs + watched_dirs
