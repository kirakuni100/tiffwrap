COMPILER:=""
DATE:=$(shell date '+%Y%m%d-%H%M%S')
PMD:=""
TYPE:=""

all:
	@echo "build - build with system compiler"
	@echo "build_gcc - build with gcc compiler"
	@echo "build_clang - build with clang compiler"
	@echo "build_coverage - create coverage information with lcov"
	@echo "build_test - build and execute test suite"
	@echo "clean - remove built files"
	@echo "clang_tidy - analyze code with Clang-Tidy"
	@echo "cppcheck - analyze code with cppcheck"
	@echo "cpplint - analyze code with cpplint"
	@echo "doc  - execute doxygen"
	@echo "flawfinder - analyze code with flawfinder"
	@echo "pmd-cpd - analyze code with pmd-cpd"

build:
	mkdir -p build
ifeq ($(COMPILER),"")
ifeq ($(TYPE),"")
	cd build ; cmake -DCMAKE_BUILD_TYPE=Release ..
else
	cd build ; cmake -DCMAKE_BUILD_TYPE=$(TYPE) ..
endif
else
ifeq ($(TYPE),"")
	cd build ; cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=$(COMPILER) ..
else
	cd build ; cmake -DCMAKE_BUILD_TYPE=$(TYPE) -DCMAKE_CXX_COMPILER=$(COMPILER) ..
endif
endif
	cd build ; make

build_gcc:
	mkdir -p build_gcc
ifeq ($(COMPILER),"")
ifeq ($(TYPE),"")
	cd build_gcc ; cmake -C ../cmake/gcc-cmakeinit.cmake -DCMAKE_BUILD_TYPE=Release ..
else
	cd build_gcc ; cmake -C ../cmake/gcc-cmakeinit.cmake -DCMAKE_BUILD_TYPE=$(TYPE) ..
endif
else
ifeq ($(TYPE),"")
	cd build_gcc ; cmake -C ../cmake/gcc-cmakeinit.cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=$(COMPILER) ..
else
	cd build_gcc ; cmake -C ../cmake/gcc-cmakeinit.cmake -DCMAKE_BUILD_TYPE=$(TYPE) -DCMAKE_CXX_COMPILER=$(COMPILER) ..
endif
endif
	cd build_gcc ; make

build_clang:
	mkdir -p build_clang
ifeq ($(COMPILER),"")
ifeq ($(TYPE),"")
	cd build_clang ; cmake -C ../cmake/clang-cmakeinit.cmake -DCMAKE_BUILD_TYPE=Release ..
else
	cd build_clang ; cmake -C ../cmake/clang-cmakeinit.cmake -DCMAKE_BUILD_TYPE=$(TYPE) ..
endif
else
ifeq ($(TYPE),"")
	cd build_clang ; cmake -C ../cmake/clang-cmakeinit.cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=$(COMPILER) ..
else
	cd build_clang ; cmake -C ../cmake/clang-cmakeinit.cmake -DCMAKE_BUILD_TYPE=$(TYPE) -DCMAKE_CXX_COMPILER=$(COMPILER) ..
endif
endif
	cd build_clang ; make

build_coverage:
	mkdir -p build_coverage
	cd build_coverage ; cmake -C ../cmake/gcc-cmakeinit.cmake -DCMAKE_BUILD_TYPE=Debug -DTIFFWRAP_BUILD_WITH_COVINFO=ON -DTIFFWRAP_BUILD_UNIT_TESTS=ON ..
	cd build_coverage ; make
	cd build_coverage ; GTEST_COLOR=1 ctest -j10 -V
	cd build_coverage ; make lcov_html

build_test:
	mkdir -p build_test
	cd build_test ; cmake -C ../cmake/clang-cmakeinit.cmake -DCMAKE_BUILD_TYPE=Debug -DTIFFWRAP_BUILD_WITH_SANITIZER=ON -DTIFFWRAP_BUILD_UNIT_TESTS=ON ..
	cd build_test ; make
	cd build_test ; GTEST_COLOR=1 ctest -j10 -V

clean:
	\rm -rf build build_gcc build_clang build_coverage build_test clang_tidy cppcheck cpplint flawfinder pmd_cpd

clang_tidy:
	mkdir -p clang_tidy/${DATE}
	\cp -r include clang_tidy/${DATE}
	\cp -r src     clang_tidy/${DATE}
	cd clang_tidy/${DATE} ; clang-tidy src/*.cc -checks=-*,clang-analyzer-* -- -Iinclude 2>&1 | tee clang_tidy.log

cppcheck:
	mkdir -p cppcheck/${DATE}
	\cp -r include cppcheck/${DATE}
	\cp -r src     cppcheck/${DATE}
	cd cppcheck/${DATE} ; cppcheck --language=c++ --std=c++17 --enable=all --suppress=missingIncludeSystem --suppress=unusedFunction -I include src/*.cc 2>&1 | tee cppcheck.log

cpplint:
	mkdir -p cpplint/${DATE}
	\cp -r include cpplint/${DATE}
	\cp -r src     cpplint/${DATE}
	cd cpplint/${DATE} ; cpplint include/*.h src/*.cc 2>&1 | tee cpplint.log

doc:
	cd doc ; doxygen Doxyfile

flawfinder:
	mkdir -p flawfinder/${DATE}
	\cp -r include flawfinder/${DATE}
	\cp -r src     flawfinder/${DATE}
	cd flawfinder/${DATE} ; flawfinder include/*.h src/*.cc 2>&1 | tee flawfinder.log

pmd-cpd:
ifeq ($(PMD),"")
	@echo "make PMD=<path to pmd> pmd-cpd"
else
	mkdir -p pmd_cpd/${DATE}
	\cp -r include pmd_cpd/${DATE}
	\cp -r src     pmd_cpd/${DATE}
	cd pmd_cpd/${DATE} ; $(PMD) cpd --minimum-tokens 100 --language cpp --files include/*.h src/*.cc 2>&1 | tee pmd_cpd.log
endif

.PHONY: all build build_gcc build_clang build_coverage build_test clean clang_tidy cppcheck cpplint doc flawfinder

