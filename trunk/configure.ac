AC_PREREQ(2.61)
AC_INIT(Village, 0.1)
AM_INIT_AUTOMAKE
AC_PROG_CC
AC_C_CONST
AC_C_INLINE
AC_HEADER_STDC
AC_CONFIG_SRCDIR([src/library.h])
AC_CHECK_HEADERS([stdlib.h string.h])
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([atexit floor memmove memset])


SDL_LIBS=`sdl-config --libs`
AC_SUBST([SDL_LIBS])
CFLAGS="$CFLAGS `sdl-config --cflags`"
AC_CHECK_HEADERS(SDL/SDL_image.h,,AC_MSG_ERROR([*** Unable to find SDL_Image library]))
#AC_CHECK_LIB(SDL_image, IMG_Load,,AC_MSG_ERROR([*** Unable to find SDL_Image library]),$SDL_LIBS)
SDL_IMAGE_LIBS="-lSDL_image"
AC_SUBST([SDL_IMAGE_LIBS])

AM_CONFIG_HEADER(src/config.h)
AC_CONFIG_FILES([Makefile
                 data/Makefile
                 src/Makefile])
AC_OUTPUT
