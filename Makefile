.PHONY: all clean clang_tidy coverage cppcheck cpplint doc test

DATE=$(shell date '+%Y%m%d-%H%M%S')

all:
	@echo "clean - remove built files"
	@echo "clang_tidy - analyze code with Clang-Tidy"
	@echo "coverage - create coverage information with lcov"
	@echo "cppcheck - analyze code with cppcheck"
	@echo "cpplint - analyze code with cpplint"
	@echo "doc  - execute doxygen"
	@echo "test - compile and execute test suite"

clean:
	\rm -rf build_coverage build_test clang_tidy cppcheck cpplint

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

test:
	mkdir -p build_test
	cd build_test ; cmake -C ../cmake/clang-cmakeinit.cmake -DTIFFWRAP_BUILD_WITH_SANITIZER=ON -DTIFFWRAP_BUILD_UNIT_TESTS=ON ..
	cd build_test ; make
	cd build_test ; GTEST_COLOR=1 ctest -j10 -V

