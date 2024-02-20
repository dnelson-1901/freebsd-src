/*-
 * SPDX-License-Identifier: BSD-4-Clause
 *
 * Copyright (c) 1990 The Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	from: @(#)sys_machdep.c	5.5 (Berkeley) 1/19/91
 */

#include <sys/cdefs.h>
#include <sys/param.h>
#include <sys/systm.h>
#include <sys/proc.h>
#include <sys/sysproto.h>
#include <sys/syscall.h>
#include <sys/sysent.h>

#include <machine/cache.h>
#include <machine/cachectl.h>
#include <machine/cpufunc.h>
#include <machine/cpuinfo.h>
#include <machine/sysarch.h>
#include <machine/cpuregs.h>
#include <machine/tls.h>

#ifndef _SYS_SYSPROTO_H_
struct sysarch_args {
	int op;
	char *parms;
};
#endif

static int
mips_sysarch_cacheflush(struct thread *td __unused, struct sysarch_args *uap)
{
	struct mips_cacheflush_args cfua;
	vm_offset_t va;
	vm_size_t nbytes;

	if (copyin(uap->parms, &cfua, sizeof(cfua)) != 0)
		return (EFAULT);

	va = (vm_offset_t)cfua.addr;
	nbytes = (vm_size_t)cfua.nbytes;

	if ((cfua.whichcache & MIPS_CF_ALLF) == 0 && nbytes == 0)
		return (0);

	switch (cfua.whichcache) {
	case MIPS_CF_ICACHE:
		mips_icache_sync_range(va, nbytes);
		break;

	case MIPS_CF_DCACHE:
		mips_dcache_wbinv_range(va, nbytes);
		break;

	case MIPS_CF_BCACHE:
		mips_icache_sync_range(va, nbytes);
		mips_dcache_wbinv_range(va, nbytes);
		break;

	case MIPS_CF_ICACHE_ALL:
		mips_icache_sync_all();
		break;

	case MIPS_CF_DCACHE_ALL:
		mips_dcache_wbinv_all();
		break;

	case MIPS_CF_BCACHE_ALL:
		mips_icache_sync_all();
		mips_dcache_wbinv_all();
		break;

	default:
		return (EINVAL);
	}

	return (0);
}

int
sysarch(struct thread *td, struct sysarch_args *uap)
{
	int error;
	void *tlsbase;

	switch (uap->op) {
	case MIPS_SET_TLS:
		td->td_md.md_tls = uap->parms;

		/*
		 * If there is an user local register implementation (ULRI)
		 * update it as well.  Add the TLS and TCB offsets so the
		 * value in this register is adjusted like in the case of the
		 * rdhwr trap() instruction handler.
		 */
		if (cpuinfo.userlocal_reg == true) {
			mips_wr_userlocal((unsigned long)(uap->parms +
			    td->td_proc->p_md.md_tls_tcb_offset));
		}
		return (0);
	case MIPS_GET_TLS: 
		tlsbase = td->td_md.md_tls;
		error = copyout(&tlsbase, uap->parms, sizeof(tlsbase));
		return (error);
	case MIPS_CACHEFLUSH:
		return (mips_sysarch_cacheflush(td, uap));
	default:
		break;
	}
	return (EINVAL);
}
