#//.PHONY: clean run build gitcom

# Запуск всей программы
all: assembler processor

# Флаги компиляции
flags = -D_DEBUG -Werror -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -pie -fPIE -Werror=vla -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr

# Общие .cpp файлы
common_cpp = COMMON/support.cpp

# Общие файлы
common_all = COMMON/color.h COMMON/comand.h COMMON/const.h COMMON/Makefile COMMON/support.h $(common_cpp)

# Файлы .cpp для сборки processor
proc_cpp = PROCESSOR/processor.cpp PROCESSOR/stack.cpp

# Файлы processor
proc_all = PROCESSOR/processor.h PROCESSOR/stack_define.h PROCESSOR/stack.h $(proc_cpp)

# Файлы .cpp assembler
asmb_cpp = ASSEMBLER/assembler.cpp

# Файлы assembler
asmb_all = ASSEMBLER/assembler.h $(asmb_cpp)

# Файлы .cpp disassembler
disasmb_cpp = DISASSEMBLER/disassembler.cpp

# Файлы disassembler
disasmb_all = DISASSEMBLER/disassembler.h $(disasmb_cpp)

# Текстовые файлы
text_file = TEXT_FILE/bin_code.bin TEXT_FILE/asm_code.asm TEXT_FILE/text_code.txt TEXT_FILE/disasmb_text.asm


# Компиляция processor
processor:
	@echo -------------------------------------------------------------------------
	g++ $(proc_cpp) $(common_cpp) $(flags) -o BIN/processor.exe

# Компиляция assembler
assembler:
	@echo -------------------------------------------------------------------------
	g++ $(asmb_cpp) $(common_cpp) $(flags) -o BIN/assembler.exe

# Запуск disassembler
disassembler:
	@echo -------------------------------------------------------------------------
	g++ $(disasmb_cpp) $(common_cpp) $(flags) -o BIN/disassembler.exe

# Запуск assembler
run-a:
	@echo -------------------------------------------------------------------------
	./BIN/assembler.exe

# Запуск processor
run-p:
	@echo -------------------------------------------------------------------------
	./BIN/processor.exe

# Запуск disassembler
run-d:
	@echo -------------------------------------------------------------------------
	./BIN/disassembler.exe

# Git commit
gitcom:
	@(git add ASSEMBLER/ BIN/ COMMON/ DISASSEMBLER/ PROCESSOR/ TEXT_FILE/ Makefile)
	@echo -- Add file to git commit ---
	@(git commit -m "$(m)")
	@echo --- Git committing ---
	@(git push)
	@echo --- Git push ---

