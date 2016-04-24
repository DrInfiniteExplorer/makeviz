
.PHONY: all
all:

ifeq (,$(file))
file:=makefile
endif
$(info $(file))

makeviz.exe : makeviz.cpp
	g++ -o$@ $<

out.dot : $(file) makeviz.exe
	cd $(dir $(file)) && $(abspath ./makeviz.exe) $(notdir $(file)) > $(abspath $@)

graph.svg : out.dot
	dot -Tsvg -o$@ out.dot

all: graph.svg
	start graph.svg
