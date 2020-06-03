.PHONY: all clean clang_tidy clang_analyze coverage cppcheck cpplint doc test

all:
	@echo "clean - remove built files"
	@echo "clang_tidy - analyze code with Clang-Tidy"
	@echo "clang_analyze - analyze code with Clang-Analyzer"
	@echo "coverage - create coverage information with lcov"
	@echo "cppcheck - analyze code with cppcheck"
	@echo "cpplint - analyze code with cpplint"
	@echo "doc  - execute doxygen"
	@echo "test - compile and execute test suite"

clean:
	\rm -rf build_coverage build_test

clang_tidy:
	@echo "test"

clang_analyze:
	@echo "test"

coverage:
	mkdir -p build_coverage
	cd build_coverage ; cmake -C ../cmake/clang-cmakeinit.cmake
	cd build_coverage ; make
	cd build_coverage ; GTEST_COLOR=1 ctest -j10 -V
	cd build_coverage ; make lcov_html

cppcheck:
	mkdir -p cppcheck
	cd cppcheck ; cppcheck --language=c++ --std=c++17 --enable=all --suppress=missingIncludeSystem --suppress=unusedFunction -I ../include ../include/*.h ../src/*.cc 2>&1 | tee cppcheck.log

cpplint:
	mkdir -p cpplint
	cd cpplint ; wget https://raw.githubusercontent.com/google/styleguide/gh-pages/cpplint/cpplint.py

doc:
	cd doc ; doxygen Doxyfile

test:
	mkdir -p build_test
	cd build_test ; cmake -C ../cmake/clang-cmakeinit.cmake -DTIFFWRAP_BUILD_WITH_SANITIZER=ON -DTIFFWRAP_BUILD_UNIT_TESTS=ON ..
	cd build_test ; make
	cd build_test ; GTEST_COLOR=1 ctest -j10 -V

