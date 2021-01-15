DATE:=$(shell date '+%Y%m%d-%H%M%S')
PMD:=""

all:
	@echo "clean - remove built files"
	@echo "clang_tidy - analyze code with Clang-Tidy"
	@echo "coverage - create coverage information with lcov"
	@echo "cppcheck - analyze code with cppcheck"
	@echo "cpplint - analyze code with cpplint"
	@echo "doc  - execute doxygen"
	@echo "flawfinder - analyze code with flawfinder"
	@echo "pmd-cpd - analyze code with pmd-cpd"
	@echo "test - compile and execute test suite"

clean:
	\rm -rf clang_tidy build_coverage cppcheck cpplint flawfinder pmd_cpd build_test 

clang_tidy:
	mkdir -p clang_tidy/${DATE}
	\cp -r include clang_tidy/${DATE}
	\cp -r src     clang_tidy/${DATE}
	cd clang_tidy/${DATE} ; clang-tidy src/*.cc -checks=-*,clang-analyzer-* -- -Iinclude 2>&1 | tee clang_tidy.log

coverage:
	mkdir -p build_coverage
	cd build_coverage ; cmake -C ../cmake/gcc-cmakeinit.cmake -DTIFFWRAP_BUILD_WITH_COVINFO=ON -DTIFFWRAP_BUILD_UNIT_TESTS=ON ..
	cd build_coverage ; make
	cd build_coverage ; GTEST_COLOR=1 ctest -j10 -V
	cd build_coverage ; make lcov_html

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

test:
	mkdir -p build_test
	cd build_test ; cmake -C ../cmake/clang-cmakeinit.cmake -DTIFFWRAP_BUILD_WITH_SANITIZER=ON -DTIFFWRAP_BUILD_UNIT_TESTS=ON ..
	cd build_test ; make
	cd build_test ; GTEST_COLOR=1 ctest -j10 -V

.PHONY: all clean clang_tidy coverage cppcheck cpplint doc flawfinder test

