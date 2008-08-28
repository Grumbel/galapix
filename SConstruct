# Zoomable Image Viewer
griv_env = Environment(CCFLAGS=["-Wall", "-Werror", "-O2", "-g3"], 
                       CXXFLAGS=["-Wall", "-Werror", "-O2", "-g3", "-Wnon-virtual-dtor"], 
                       LIBS=['jpeg',
                             'mhash', 
                             'spnav',
                             'GL', 
                             'GLU',
                             'GLEW'])
griv_env.ParseConfig("sdl-config --libs --cflags")
griv_env.ParseConfig("pkg-config sqlite3 --libs --cflags")
griv_env.Program('griv', [
        'src/blob.cpp',
        'src/math.cpp',
        'src/math/size.cpp',
        'src/math/rect.cpp',
        'src/math/vector2i.cpp',
        'src/math/vector2f.cpp',
        'src/math/vector3f.cpp',
        'src/file_database.cpp',
        'src/file_entry.cpp',
        'src/filesystem.cpp',
        'src/framebuffer.cpp',
        'src/jpeg.cpp',
        'src/jpeg_image.cpp',
        'src/jpeg_memory_src.cpp',
        'src/jpeg_memory_dest.cpp',
        'src/jpeg_decoder_thread.cpp',
        'src/job_handle.cpp',
        'src/griv.cpp',
        'src/image.cpp',
        'src/md5.cpp',
        'src/software_surface.cpp',
        'src/space_navigator.cpp',
        'src/sqlite.cpp',
        'src/surface.cpp',
        'src/texture.cpp',
        'src/thread.cpp',
        'src/tile_database.cpp',
        'src/tile_generator.cpp',
        'src/tile_generator_thread.cpp',
        'src/database_thread.cpp',
        'src/viewer_thread.cpp',
        'src/viewer.cpp',
        'src/workspace.cpp',
        ])

# EOF #
