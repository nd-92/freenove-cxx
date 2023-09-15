/*
Copyright (c) 2012, Broadcom Europe Ltd.
Copyright (c) 2016, Jeremy Garff
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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <stdint.h>
#include <errno.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/sysmacros.h>
#include <sys/stat.h>

#include "mailbox.h"

void *mapmem(const __off_t base, const size_t size, const char *mem_dev) // Safe
{
    const __off_t pagemask = static_cast<__off_t>(~0UL) ^ static_cast<__off_t>(getpagesize() - 1);
    const __off_t offsetmask = static_cast<__off_t>(getpagesize() - 1);

    const int mem_fd = open(mem_dev, O_RDWR | O_SYNC);
    if (mem_fd < 0)
    {
        perror("Can't open /dev/mem");
        return NULL;
    }

    const void *mem = mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd, base & pagemask);
    if (mem == MAP_FAILED)
    {
        perror("mmap error\n");
        return NULL;
    }

    close(mem_fd);

    return (char *)mem + (base & offsetmask);
}

void *unmapmem(const void *addr, const size_t size) // Safe
{
    uintptr_t pagemask = ~0UL ^ static_cast<uintptr_t>(getpagesize() - 1);
    uintptr_t baseaddr = reinterpret_cast<uintptr_t>(addr) & pagemask;

    const int s = munmap(reinterpret_cast<void *>(baseaddr), size);
    if (s != 0)
    {
        perror("munmap error\n");
    }

    return NULL;
}

/*
 * use ioctl to send mbox property message
 */

static int mbox_property(const int file_desc, const void *buf) // Safe
{
    int fd = file_desc;
    int ret_val = -1;

    if (fd < 0)
    {
        fd = mbox_open();
    }

    if (fd >= 0)
    {
        ret_val = ioctl(fd, IOCTL_MBOX_PROPERTY, buf);

        if (ret_val < 0)
        {
            perror("ioctl_set_msg failed\n");
        }
    }

    if (file_desc < 0)
    {
        mbox_close(fd);
    }

    return ret_val;
}

uint32_t mem_alloc(const int file_desc, const uint32_t size, const uint32_t align, const uint32_t flags) // Safe
{
    uint32_t i = 0;
    uint32_t p[32];

    p[i++] = 0;           // size
    p[i++] = 0x00000000;  // process request
    p[i++] = 0x3000c;     // (the tag id)
    p[i++] = 12;          // (size of the buffer)
    p[i++] = 12;          // (size of the data)
    p[i++] = size;        // (num bytes? or pages?)
    p[i++] = align;       // (alignment)
    p[i++] = flags;       // (MEM_FLAG_L1_NONALLOCATING)
    p[i++] = 0x00000000;  // end tag
    p[0] = i * sizeof *p; // actual size

    if (mbox_property(file_desc, p) < 0)
    {
        return 0;
    }
    else
    {
        return p[5];
    }
}

uint32_t mem_free(const int file_desc, const uint32_t handle) // Safe
{
    uint32_t i = 0;
    uint32_t p[32];

    p[i++] = 0;           // size
    p[i++] = 0x00000000;  // process request
    p[i++] = 0x3000f;     // (the tag id)
    p[i++] = 4;           // (size of the buffer)
    p[i++] = 4;           // (size of the data)
    p[i++] = handle;      // handle
    p[i++] = 0x00000000;  // end tag
    p[0] = i * sizeof *p; // actual size

    mbox_property(file_desc, p);

    return p[5];
}

uint32_t mem_lock(const int file_desc, const uint32_t handle) // Safe
{
    uint32_t i = 0;
    uint32_t p[32];

    p[i++] = 0;           // size
    p[i++] = 0x00000000;  // process request
    p[i++] = 0x3000d;     // (the tag id)
    p[i++] = 4;           // (size of the buffer)
    p[i++] = 4;           // (size of the data)
    p[i++] = handle;      // handle
    p[i++] = 0x00000000;  // end tag
    p[0] = i * sizeof *p; // actual size

    if (mbox_property(file_desc, p) < 0)
    {
        return static_cast<uint32_t>(~0);
    }
    else
    {
        return p[5];
    }
}

uint32_t mem_unlock(const int file_desc, const uint32_t handle) // Safe
{
    uint32_t i = 0;
    uint32_t p[32];

    p[i++] = 0;            // size
    p[i++] = 0x00000000;   // process request
    p[i++] = 0x3000e;      // (the tag id)
    p[i++] = 4;            // (size of the buffer)
    p[i++] = 4;            // (size of the data)
    p[i++] = handle;       // handle
    p[i++] = 0x00000000;   // end tag
    p[0] = i * sizeof(*p); // actual size

    mbox_property(file_desc, p);

    return p[5];
}

uint32_t execute_code(const int file_desc, const uint32_t code, const uint32_t r0, const uint32_t r1, const uint32_t r2, const uint32_t r3, const uint32_t r4, const uint32_t r5) // Safe
{
    uint32_t i = 0;
    uint32_t p[32];

    p[i++] = 0;            // size
    p[i++] = 0x00000000;   // process request
    p[i++] = 0x30010;      // (the tag id)
    p[i++] = 28;           // (size of the buffer)
    p[i++] = 28;           // (size of the data)
    p[i++] = code;         // code
    p[i++] = r0;           // r0
    p[i++] = r1;           // r1
    p[i++] = r2;           // r2
    p[i++] = r3;           // r3
    p[i++] = r4;           // r4
    p[i++] = r5;           // r5
    p[i++] = 0x00000000;   // end tag
    p[0] = i * sizeof(*p); // actual size

    mbox_property(file_desc, p);

    return p[5];
}

uint32_t qpu_enable(const int file_desc, const uint32_t enable) // Safe
{
    uint32_t i = 0;
    uint32_t p[32];

    p[i++] = 0;            // size
    p[i++] = 0x00000000;   // process request
    p[i++] = 0x30012;      // (the tag id)
    p[i++] = 4;            // (size of the buffer)
    p[i++] = 4;            // (size of the data)
    p[i++] = enable;       // enable
    p[i++] = 0x00000000;   // end tag
    p[0] = i * sizeof(*p); // actual size

    mbox_property(file_desc, p);

    return p[5];
}

uint32_t execute_qpu(const int file_desc, const uint32_t num_qpus, const uint32_t control, const uint32_t noflush, const uint32_t timeout) // Safe
{
    uint32_t i = 0;
    uint32_t p[32];

    p[i++] = 0;            // size
    p[i++] = 0x00000000;   // process request
    p[i++] = 0x30011;      // (the tag id)
    p[i++] = 16;           // (size of the buffer)
    p[i++] = 16;           // (size of the data)
    p[i++] = num_qpus;     // num_qpus
    p[i++] = control;      // control
    p[i++] = noflush;      // noflush
    p[i++] = timeout;      // ms
    p[i++] = 0x00000000;   // end tag
    p[0] = i * sizeof(*p); // actual size

    mbox_property(file_desc, p);

    return p[5];
}

int mbox_open(void) // Safe
{
    char filename[64];

    int file_desc = open("/dev/vcio", 0);
    if (file_desc >= 0)
    {
        return file_desc;
    }

    // open a char device file used for communicating with kernel mbox driver
    sprintf(filename, "/tmp/mailbox-%d", getpid());
    unlink(filename);
    if (mknod(filename, S_IFCHR | 0600, makedev(100, 0)) < 0)
    {
        perror("Failed to create mailbox device\n");
        return -1;
    }
    file_desc = open(filename, 0);
    if (file_desc < 0)
    {
        perror("Can't open device file\n");
        unlink(filename);
        return -1;
    }
    unlink(filename);

    return file_desc;
}

void mbox_close(const int file_desc) // Safe
{
    close(file_desc);
}
