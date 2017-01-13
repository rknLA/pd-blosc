# Makefile to build class 'blosc' for Pure Data.
# Needs Makefile.pdlibbuilder as helper makefile for platform-dependent build
# settings and rules.

# library name
lib.name = saw~

# input source file (class name == source file basename)
class.sources = saw~.c

# all extra files to be included in binary distribution of the library
datafiles = saw~-help.pd saw~-meta.pd README.md

# include Makefile.pdlibbuilder from submodule directory 'pd-lib-builder'
PDLIBBUILDER_DIR=pd-lib-builder/
include $(PDLIBBUILDER_DIR)/Makefile.pdlibbuilder
