noinst_PROGRAMS = createlib
createlib_SOURCES = createlib.c
createlib_LDADD = $(SDL_LIBS) $(SDL_IMAGE_LIBS)

villagedir = $(datadir)/${PACKAGE_TARNAME}
datafiles = \
background.ini \
background.png \
menubar.ini \
menubar.png \
buildings.ini \
buildings.png \
cursor.ini \
cursor.png \
flora.ini \
flora.png \
miscfixed.ini \
miscfixed.png \
humans.ini \
humans.png

packedfiles = \
background.lfg \
menubar.lfg \
buildings.lfg \
cursor.lfg \
flora.lfg \
miscfixed.lfg \
humans.lfg

village_DATA = $(packedfiles)
extra_DIST = $(datafiles)

$(packedfiles): createlib
	./createlib c background background.lfg
	./createlib c menubar menubar.lfg
	./createlib c buildings buildings.lfg
	./createlib c cursor cursor.lfg
	./createlib c flora flora.lfg
	./createlib c miscfixed miscfixed.lfg
	./createlib c humans humans.lfg
