CONTAINER = $(USER)/fceuxos
TMP = /var/tmp
CWD = $(shell pwd)
SOURCE = movies/
MOVIES := $(wildcard $(SOURCE)*.fm2)
TARGETS = $(MOVIES:$(SOURCE)%.fm2=%.ino)
SCRIPT = movie_to_autones.lua

all: $(TARGETS)

%.ino: movies/$(%.fm2) roms/$(%.nes)
	@echo making $@
	@echo cp $^ $(TMP)
	@echo cp scripts/$(SCRIPT) $(TMP)
	@echo docker run -d -v /autones:$(TMP) -w /autones -t $(CONTAINER):latest /usr/games/fceux --loadlua $(SCRIPT) --playmov $(@:.ino=.fm2) $(@:.ino=.zip)

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
