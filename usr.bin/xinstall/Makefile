.include <src.opts.mk>

PACKAGE=	runtime

PROG=		xinstall
PROGNAME=	install
SRCS=		xinstall.c getid.c
MAN=		install.1
NO_SHARED?=	YES

.info install debug:${.newline}\
  DESTDIR=${DESTDIR}${.newline}\
  PATH=${PATH}${.newline}
.if ${DESTDIR:M*legacy*} && ${PATH:M*legacy*} && ${MACHINE} == "mips"
# install is installing on top of itself?
.info install installing onto itself
INSTALL=	/usr/bin/install
INSTALLFLAGS+=	-S -v -U
.endif

.PATH:		${SRCTOP}/contrib/mtree
CFLAGS+=	-I${SRCTOP}/contrib/mtree
CFLAGS+=	-I${SRCTOP}/lib/libnetbsd

LIBADD=		md
CFLAGS+=	-DWITH_MD5 -DWITH_RIPEMD160

.ifdef BOOTSTRAPPING
# For the bootstrap we disable copy_file_range()
CFLAGS+=	-DBOOTSTRAP_XINSTALL
.endif

HAS_TESTS=
SUBDIR.${MK_TESTS}+= tests

.include <bsd.prog.mk>
