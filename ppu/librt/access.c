/* This is file ACCESS.C */
/*
 * Copyright (C) 1993 DJ Delorie
 * All rights reserved.
 *
 * Redistribution, modification, and use in source and binary forms is permitted
 * provided that the above copyright notice and following paragraph are
 * duplicated in all such forms.
 *
 * This file is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

extern int errno;

int __attribute__((weak)) access(const char *fn, int flags)
{
	struct stat s;

	/* Did the pass us the right flags? */
	if ( (flags != F_OK) && (flags != R_OK) && (flags != W_OK) && (flags != X_OK) ) {
		/* Nope. */
		errno = EINVAL;
		return -1;
	}

	if (stat(fn, &s)) {
		/* return -1 because the file does not exist or pathname is too long. */
		return -1;
	}

	if (flags == F_OK) {
		/* We know the file exists because stat didn't fail. */
		return 0;
	}

	if (flags & W_OK) {
		/* Do we have write permission to the file? */
		if (s.st_mode & S_IWRITE) {
			/* We do. */
			return 0;
		}

		/* Nope. */
		errno = EACCES;
		return -1;
	}

	if (flags & R_OK) {
		/* Do we have read permission to the file? */
		if (s.st_mode & S_IREAD) {
			/* We do. */
			return 0;
		}

		/* Nope. */
		errno = EACCES;
		return -1;
	}

	if (flags & X_OK) {
		/* Do we have executable permission to the file? */
		if (s.st_mode & S_IEXEC) {
			/* We do. */
			return 0;
		}

		/* Nope. */
		errno = EACCES;
		return -1;
	}

	/* We should never reach this, ever...in case we do though, lets return -1. */
	/* and set errno to ENOSYS (Function not implemented */
	errno = ENOSYS;
	return -1;
}
