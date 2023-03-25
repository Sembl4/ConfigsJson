CMAKE_COMMON_FLAGS ?=
CMAKE_DEBUG_FLAGS ?=
CMAKE_RELEASE_FLAGS ?=

NPROCS ?= $(shell nproc)
# NOTE: use Makefile.local for customization
-include Makefile.local

build: clear
	@mkdir -p build
	@cd build && \
	  cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_OPTIMIZE=O3 $(CMAKE_COMMON_FLAGS) .. && \
	  make

clear:
	@rm -rf build