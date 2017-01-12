# Makefile to build class 'blosc' for Pure Data.
# Needs Makefile.pdlibbuilder as helper makefile for platform-dependent build
# settings and rules.

# library name
lib.name = blosc

# input source file (class name == source file basename)
class.sources = blosc.c

# all extra files to be included in binary distribution of the library
datafiles = blosc-help.pd blosc-meta.pd README.md

# include Makefile.pdlibbuilder from submodule directory 'pd-lib-builder'
PDLIBBUILDER_DIR=pd-lib-builder/
include $(PDLIBBUILDER_DIR)/Makefile.pdlibbuilder
