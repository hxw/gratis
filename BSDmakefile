# Makefile

DESTDIR ?= ""
PREFIX ?= /usr/local

PANEL_VERSION ?= NOT-SET

.ifmake rpi || raspberrypi || rpi-install
EPD_IO ?= epd_io_free_uart.h
.else
EPD_IO ?= epd_io.h
.endif

SUPPORTED_PANEL_VERSIONS = V231_G2

.PHONY: help
help:
	@echo
	@echo Please use the platform specific targets:
	@echo
	@echo Version Setting:
	@echo '  G2 options:   PANEL_VERSION=V231_G2'
	@echo
	@echo 'currently:      PANEL_VERSION=${PANEL_VERSION}'
	@echo
	@echo Custom I/O pin arrangement can be selected, add: EPD_IO=filename.h
	@echo '(the file will be taken from RaspberryPi or BeagleBone directory as appropriate)'
	@echo currently: EPD_IO=${EPD_IO}
	@echo
	@echo Raspberry Pi:
	@echo '   $(MAKE) rpi            = build all targets'
	@echo '   $(MAKE) rpi-install    = install fuse driver in PREFIX=${PREFIX}'
	@echo
	@echo BeagleBone
	@echo '   $(MAKE) bb             = build all targets'
	@echo '   $(MAKE) bb-install     = install fuse driver in PREFIX=${PREFIX}'
	@echo
	@echo Where T is one of:
	@echo '    all install remove clean'
	@echo '    epd_test gpio_test epd_fuse'
	@echo
	@echo Notes:
	@echo 1. the default install: PREFIX=${PREFIX}
	@echo 2. if not root must use sudo when installing
	@echo

.PHONY: all
all: help

.PHONY: version-error
version-error:
	@echo
	@echo "ERROR:  PANEL_VERSION='${PANEL_VERSION}' is incorrect"
	@echo
	@echo please set to one of:
	@echo
	@for v in ${SUPPORTED_PANEL_VERSIONS} ; \
	 do \
	   echo "  PANEL_VERSION='$${v}'" ; \
	 done
	@echo
	@echo For more information, run:
	@echo
	@echo '  $(MAKE) help'
	@echo

.PHONY: version-check
version-check:
	@flag= ; \
	 for v in ${SUPPORTED_PANEL_VERSIONS} ; \
	 do \
	   [ X"$${v}" = X"${PANEL_VERSION}" ] && flag=x ; \
	 done ; \
	 [ -z "$${flag}" ] && $(MAKE) version-error && false || true


# Raspberry Pi targets
# --------------------

.PHONY: rpi raspberrypi
rpi raspberrypi: version-check
	$(MAKE) DESTDIR=${DESTDIR} PREFIX=${PREFIX} PLATFORM=../RaspberryPi PANEL_VERSION="${PANEL_VERSION}" EPD_IO="${EPD_IO}" -C PlatformWithOS/FreeBSD

.PHONY: rpi-install
rpi-install: version-check
	$(MAKE) DESTDIR=${DESTDIR} PREFIX=${PREFIX} PLATFORM=../RaspberryPi PANEL_VERSION="${PANEL_VERSION}" EPD_IO="${EPD_IO}" -C PlatformWithOS/FreeBSD install


# BeagleBone Black targets
# ------------------------

.PHONY: bb beaglebone
bb beaglebone: version-check
	$(MAKE) DESTDIR=$(DESTDIR) PREFIX=$(PREFIX) PLATFORM=../BeagleBone PANEL_VERSION="${PANEL_VERSION}" EPD_IO="${EPD_IO}" -C PlatformWithOS/FreeBSD

.PHONY: bb-install
bb-install: version-check
	$(MAKE) DESTDIR=$(DESTDIR) PREFIX=$(PREFIX) PLATFORM=../BeagleBone PANEL_VERSION="${PANEL_VERSION}" EPD_IO="${EPD_IO}" -C PlatformWithOS/FreeBSD install
