CONTAINER = $(USER)/fceuxos
ROOTFS = /var/tmp/rootfs
TMP = $(shell mktemp)
CWD = $(shell pwd)
DEPS = aglar_starwars.ino

all: $(DEPS)

%.ino: movies/$(@:.ino=.fm2)
	echo cp movies/$(%) $(TMP)
	echo
	echo cp roms/$(@:.ino=.zip) $(TMP)
	echo
	echo docker run -v $(TMP):$(TMP) -w $(TMP)/$(@:.ino=) -t $(CONTAINER):latest make

fceuxos:
	docker build -t $(CONTAINER) .

push: %-push
.PHONE: push

%-push:
	PROGRAM=$(@:-push=) make -f Makefile.arduino

clean: $(DEPS:.ino=-clean)
.PHONY: clean

%-clean:
	docker run -v $(CWD)/src:$(TMP) -w $(TMP)/$(@:-clean=) -t $(CONTAINER):latest make clean
