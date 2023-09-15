/*
Copyright (c) 2012, Broadcom Europe Ltd.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the copyright holder nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <linux/ioctl.h>

#define MAJOR_NUM 100
#define IOCTL_MBOX_PROPERTY _IOWR(MAJOR_NUM, 0, char *)

#define DEV_MEM "/dev/mem"
#define DEV_GPIOMEM "/dev/gpiomem"

// Safe
void *mapmem(const __off_t base, const size_t size, const char *mem_dev);

// Safe
void *unmapmem(const void *addr, const size_t size);

// Safe
uint32_t mem_alloc(const int file_desc, const uint32_t size, const uint32_t align, const uint32_t flags);

// Safe
uint32_t mem_free(const int file_desc, const uint32_t handle);

// Safe
uint32_t mem_lock(const int file_desc, const uint32_t handle);

// Safe
uint32_t mem_unlock(const int file_desc, const uint32_t handle);

// Safe
uint32_t execute_code(const int file_desc, const uint32_t code, const uint32_t r0, const uint32_t r1, const uint32_t r2, const uint32_t r3, const uint32_t r4, const uint32_t r5);

// Safe
uint32_t qpu_enable(const int file_desc, const uint32_t enable);

// Safe
uint32_t execute_qpu(const int file_desc, const uint32_t num_qpus, const uint32_t control, const uint32_t noflush, const uint32_t timeout);

// Safe
int mbox_open(void);

// Safe
void mbox_close(const int file_desc);
