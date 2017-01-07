/*-
 * Copyright (c) 2015 Stacey D. Son
 * All rights reserved.
 *
 * This software was developed by SRI International and the University of
 * Cambridge Computer Laboratory under DARPA/AFRL contract (FA8750-10-C-0237)
 * ("CTSRD"), as part of the DARPA CRASH research programme.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef	_MACHINE_CACHECTL_H_
#define	_MACHINE_CACHECTL_H_

#define	MIPS_CF_ICACHE	0x00000001	/* invalidate I-cache */
#define	MIPS_CF_DCACHE	0x00000002	/* writeback and invalidate D-cache */
#define	MIPS_CF_BCACHE	(MIPS_CF_ICACHE | MIPS_CF_DCACHE) /* invalidate both */

#define	MIPS_CF_ALLF	0x80000000 	/* invalidate entire cache flag bit */
#define	MIPS_CF_ICACHE_ALL (MIPS_CF_ALLF | MIPS_CF_ICACHE) /* Entire I-cache */	
#define	MIPS_CF_DCACHE_ALL (MIPS_CF_ALLF | MIPS_CF_DCACHE) /* Entire D-cache */	
#define	MIPS_CF_BCACHE_ALL (MIPS_CF_ALLF | MIPS_CF_BCACHE) /* Entire both */	

#if !defined(_KERNEL)
#include <machine/sysarch.h>

static inline int
mips_cacheflush(void *addr, size_t nbytes, unsigned whichcache)
{
	struct mips_cacheflush_args cfa;

	cfa.addr = addr;
	cfa.nbytes = nbytes;
	cfa.whichcache = whichcache;

	return (sysarch(MIPS_CACHEFLUSH, (void *)&cfa));
}

/* IRIX API compatible aliases. */
#define	ICACHE	MIPS_CF_ICACHE
#define	DCACHE	MIPS_CF_DCACHE
#define	BCACHE	MIPS_CF_BCACHE

static inline int
cacheflush(void *addr, int nbytes, int whichcache)
{

	return (mips_cacheflush(addr, (size_t)nbytes, whichcache));
}

static inline int
_flush_cache(char *addr, int nbytes, int whichcache)
{

	return (mips_cacheflush((void *)addr, (size_t)nbytes, whichcache));
}
#endif /* ! _KERNEL */
#endif	/* _MACHINE_CACHECTL_H_ */
