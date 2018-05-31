
.PHONY: all
all:

ifeq (,$(file))
file:=makefile
endif
$(info $(file))

makeviz.exe : makeviz.cpp
	g++ -omakeviz.exe makeviz.cpp

out.dot : $(file) makeviz.exe
	cd $(dir $(file)) && $(abspath ./makeviz.exe) make -f $(notdir $(file)) > $(abspath out.dot)

graph.svg : out.dot
	dot -Tsvg -ograph.svg out.dot

all: graph.svg
	start graph.svg
