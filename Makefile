CONTAINER = $(USER)/fceuxos
TMP = /var/tmp
CWD = $(shell pwd)
SOURCE = movies/
MOVIES := $(wildcard $(SOURCE)*.fm2)
TARGETS = $(MOVIES:$(SOURCE)%.fm2=%.ino)
SCRIPT = movie_to_autones.lua

all: $(TARGETS)

%.ino: movies/%.fm2
	@cp scripts/$(SCRIPT) $(TMP)
	@cp movies/$(*).fm2 $(TMP)
	@cp roms/$(shell echo $* | cut -d'-' -f2).nes $(TMP)
	@docker run -v $(CWD)/src:$(TMP) -w $(TMP) -t $(CONTAINER):latest \
    fceux --loadlua $(SCRIPT) --playmov $(*).fm2 $(shell echo $* | cut -d'-' -f2).nes

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
