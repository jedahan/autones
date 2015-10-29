CONTAINER = $(USER)/fceuxos
TMP = /var/tmp
CWD = $(shell pwd)
DEPS = aglar-starwars.ino
SCRIPT = movie_to_autones.lua

all: $(DEPS)

%.ino: movies/%.fm2 roms/%.zip
	cp $^ $(TMP)
	cp scripts/$(SCRIPT) $(TMP)
	docker run -v $(TMP):$(TMP) -w $(TMP) -t $(CONTAINER):latest fceux --loadlua $(SCRIPT) --playmov $(@:.ino=.fm2) $(@:.ino=.zip)

fceuxos:
	docker build -t $(CONTAINER) .

push: %-push
.PHONY: push

%-push:
	PROGRAM=$(@:-push=) make -f Makefile.arduino

clean: $(DEPS:.ino=-clean)
.PHONY: clean

%-clean:
	docker run -v $(CWD)/src:$(TMP) -w $(TMP)/$(@:-clean=) -t $(CONTAINER):latest make clean
