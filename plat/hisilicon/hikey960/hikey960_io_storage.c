/*
 * Copyright (c) 2016, ARM Limited and Contributors. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * Neither the name of ARM nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific
 * prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <arch_helpers.h>
#include <assert.h>
#include <debug.h>
#include <errno.h>
#include <firmware_image_package.h>
#include <io_block.h>
#include <io_driver.h>
#include <io_fip.h>
#include <io_memmap.h>
#include <io_storage.h>
#include <mmio.h>
#include <platform_def.h>
#include <semihosting.h>	/* For FOPEN_MODE_... */
#include <string.h>
#include <ufs.h>

struct plat_io_policy {
	uintptr_t *dev_handle;
	uintptr_t image_spec;
	int (*check)(const uintptr_t spec);
};

static const io_dev_connector_t *ufs_dev_con, *fip_dev_con;
static uintptr_t ufs_dev_handle, fip_dev_handle;

static int check_ufs(const uintptr_t spec);
static int check_fip(const uintptr_t spec);
size_t ufs_read_lun3_blks(int lba, uintptr_t buf, size_t size);
size_t ufs_write_lun3_blks(int lba, const uintptr_t buf, size_t size);

static const io_block_spec_t ufs_fip_spec = {
	.offset		= HIKEY960_FIP_BASE,
	.length		= HIKEY960_FIP_MAX_SIZE,
};

static const io_block_spec_t ufs_data_spec = {
	.offset 	= 0,
	.length 	= 256 << 20,
};

static const io_block_dev_spec_t ufs_dev_spec = {
	/* It's used as temp buffer in block driver. */
	.buffer		= {
		.offset	= HIKEY960_UFS_DATA_BASE,
		.length	= HIKEY960_UFS_DATA_SIZE,
	},
	.ops		= {
		.read	= ufs_read_lun3_blks,
		.write	= ufs_write_lun3_blks,
	},
	.block_size	= UFS_BLOCK_SIZE,
};

static const io_uuid_spec_t bl2_uuid_spec = {
	.uuid = UUID_TRUSTED_BOOT_FIRMWARE_BL2,
};

static const struct plat_io_policy policies[] = {
	[FIP_IMAGE_ID] = {
		&ufs_dev_handle,
		(uintptr_t)&ufs_fip_spec,
		check_ufs
	},
	[BL2_IMAGE_ID] = {
		&fip_dev_handle,
		(uintptr_t)&bl2_uuid_spec,
		check_fip
	},
	[BL2U_IMAGE_ID] = {
		&ufs_dev_handle,
		(uintptr_t)&ufs_data_spec,
		check_ufs
	}
};

static int check_ufs(const uintptr_t spec)
{
	int result;
	uintptr_t local_handle;

	result = io_dev_init(ufs_dev_handle, (uintptr_t)NULL);
	if (result == 0) {
		result = io_open(ufs_dev_handle, spec, &local_handle);
		if (result == 0) {
			io_close(local_handle);
		}
	}
	return result;
}

static int check_fip(const uintptr_t spec)
{
	int result;
	uintptr_t local_image_handle;

	/* See if a Firmware Image Package is available */
	result = io_dev_init(fip_dev_handle, (uintptr_t)FIP_IMAGE_ID);
	if (result == 0) {
		result = io_open(fip_dev_handle, spec, &local_image_handle);
		if (result == 0) {
			VERBOSE("Using FIP\n");
			io_close(local_image_handle);
		}
	}
	return result;
}

void hikey960_io_setup(void)
{
	int result;

	result = register_io_dev_block(&ufs_dev_con);
	assert(result == 0);

	result = register_io_dev_fip(&fip_dev_con);
	assert(result == 0);

	result = io_dev_open(ufs_dev_con, (uintptr_t)&ufs_dev_spec,
			     &ufs_dev_handle);
	assert(result == 0);

	result = io_dev_open(fip_dev_con, (uintptr_t)NULL, &fip_dev_handle);
	assert(result == 0);

	/* Ignore improbable errors in release builds */
	(void)result;
}

/* Return an IO device handle and specification which can be used to access
 * an image. Use this to enforce platform load policy
 */
int plat_get_image_source(unsigned int image_id, uintptr_t *dev_handle,
			  uintptr_t *image_spec)
{
	int result;
	const struct plat_io_policy *policy;

	assert(image_id < ARRAY_SIZE(policies));

	policy = &policies[image_id];
	result = policy->check(policy->image_spec);
	assert(result == 0);

	*image_spec = policy->image_spec;
	*dev_handle = *(policy->dev_handle);

	return result;
}

size_t ufs_read_lun3_blks(int lba, uintptr_t buf, size_t size)
{
	return ufs_read_blocks(3, lba, buf, size);
}

size_t ufs_write_lun3_blks(int lba, const uintptr_t buf, size_t size)
{
	return ufs_write_blocks(3, lba, buf, size);
}
