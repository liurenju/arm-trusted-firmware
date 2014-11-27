/*
 * Copyright (c) 2014, Hisilicon Ltd.
 * Copyright (c) 2014, Linaro Ltd.
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

#ifndef __HI6553_H__
#define __HI6553_H__

#define DISABLE6_XO_CLK				0x036

#define DISABLE6_XO_CLK_BB			(1 << 0)
#define DISABLE6_XO_CLK_CONN			(1 << 1)
#define DISABLE6_XO_CLK_NFC			(1 << 2)
#define DISABLE6_XO_CLK_RF1			(1 << 3)
#define DISABLE6_XO_CLK_RF2			(1 << 4)

#define PERI_EN_MARK				0x040
#define BUCK2_REG1				0x04a
#define BUCK2_REG5				0x04e
#define BUCK2_REG6				0x04f
#define BUCK3_REG3				0x054
#define BUCK3_REG5				0x056
#define BUCK3_REG6				0x057
#define BUCK4_REG2				0x05b
#define BUCK4_REG5				0x05e
#define BUCK4_REG6				0x05f
#define CLK_TOP0				0x063
#define CLK_TOP3				0x066
#define CLK_TOP4				0x067
#define VSET_BUCK2_ADJ				0x06d
#define VSET_BUCK3_ADJ				0x06e
#define LDO19_REG_ADJ				0x084
#define LDO20_REG_ADJ				0x085
#define NP_REG_ADJ1				0x0be
#define NP_REG_CHG				0x0c0
#define BUCK01_CTRL2				0x0d9
#define BUCK0_CTRL1				0x0dd
#define BUCK0_CTRL5				0x0e1
#define BUCK0_CTRL7				0x0e3
#define BUCK1_CTRL1				0x0e8
#define BUCK1_CTRL5				0x0ec
#define BUCK1_CTRL7				0x0ef
#define CLK19M2_600_586_EN			0x0fe

unsigned char hi6553_read_8(unsigned int offset);
void hi6553_write_8(unsigned int offset, unsigned int value);

#endif	/* __HI6553_H__ */