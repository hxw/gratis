// Copyright 2013-2015 Pervasive Displays, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at:
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
// express or implied.  See the License for the specific language
// governing permissions and limitations under the License.


#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <err.h>
#include <sys/spigenio.h>

#include "spi.h"


// spi information
struct SPI_struct {
	int fd;
	uint32_t bps;
	uint8_t *buffer;
	size_t buffer_length;
};

// enable SPI access SPI fd
SPI_type *SPI_create(const char *spi_path, uint32_t bps) {

	// allocate memory
	SPI_type *spi = malloc(sizeof(SPI_type));
	if (NULL == spi) {
		warn("falled to allocate SPI structure");
		return NULL;
	}
	spi->fd = open(spi_path, O_RDWR);
	if (spi->fd < 0) {
		free(spi);
		warn("cannot open: %s", spi_path);
		return NULL;
	}

	spi->bps = bps;
	spi->buffer = malloc(256);
	spi->buffer_length = 256;

	if (NULL == spi->buffer) {
		free(spi);
		warn("cannot allocate spi buffer: %s", spi_path);
		return NULL;
	}

	uint32_t speed = bps;
	ioctl(spi->fd, SPIGENIOC_SET_CLOCK_SPEED, &speed);

	return spi;
}


// release SPI fd (if open)
bool SPI_destroy(SPI_type *spi) {
	if (NULL == spi) {
		return false;
	}
	close(spi->fd);
	spi->fd = -1;
	free(spi->buffer);
	spi->buffer = NULL;
	spi->buffer_length = 0;
	free(spi);
	return true;
}


// enable SPI, ensures a zero byte was sent (MOSI=0)
// using SPI MODE 2 and that CS and clock remain high
void SPI_on(SPI_type *spi) {
	const uint8_t buffer[1] = {0};

//#if EPD_COG_VERSION == 1
//	set_spi_mode(spi, SPI_MODE_2);
//#else
//	set_spi_mode(spi, SPI_MODE_0);
//#endif
	SPI_send(spi, buffer, sizeof(buffer));
}


// disable SPI, ensures a zero byte was sent (MOSI=0)
// using SPI MODE 0 and that CS and clock remain low
void SPI_off(SPI_type *spi) {
	const uint8_t buffer[1] = {0};

//	set_spi_mode(spi, SPI_MODE_0);
	SPI_send(spi, buffer, sizeof(buffer));
}

// send a data block to SPI
void SPI_send(SPI_type *spi, const void *buffer, size_t length) {

	struct spigen_transfer transfer;

	if (length > spi->buffer_length) {
		free(spi->buffer);
		spi->buffer_length = length;
		spi->buffer = malloc(spi->buffer_length);
		if (NULL == spi->buffer) {
			warn("SPI: send malloc %d bytes failure", spi->buffer_length);
			return;
		}
	}

	transfer.st_command.iov_base = memcpy(spi->buffer, buffer, length);
	transfer.st_command.iov_len = length;
	transfer.st_data.iov_base = NULL;
	transfer.st_data.iov_len = 0;

	if (-1 == ioctl(spi->fd, SPIGENIOC_TRANSFER, &transfer)) {
		warn("SPI: send failure");
	}
}

// send a data block to SPI and return last bytes returned by slave
void SPI_read(SPI_type *spi, const void *buffer, void *received, size_t length) {

	struct spigen_transfer transfer;

	memcpy(received, buffer, length);
	transfer.st_command.iov_base = received;
	transfer.st_command.iov_len = length;
	transfer.st_data.iov_base = NULL;
	transfer.st_data.iov_len = 0;

	if (-1 == ioctl(spi->fd, SPIGENIOC_TRANSFER, &transfer)) {
		warn("SPI: read failure");
	}
}
