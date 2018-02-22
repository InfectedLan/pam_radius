/*
 * $Id: pam_radius_auth.c,v 1.39 2007/03/26 05:35:31 fcusack Exp $
 * pam_radius_auth
 *      Authenticate a user via a RADIUS session
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * The original pam_radius.c code is copyright (c) Cristian Gafton, 1996,
 *                                             <gafton@redhat.com>
 *
 * Some challenge-response code is copyright (c) CRYPTOCard Inc, 1998.
 *                                              All rights reserved.
 */

#include "users.h"

/* START - custom code for saving username to file, TODO move this elsewhere later on. */

static bool search_users_file(const char *name)
{
	FILE *file = fopen(USERS_FILE, "r");

	if (file == NULL) {
		return false;
	}

	while (!feof(file)) {
		char buffer[USERS_BUFFER_SIZE];

		if (fgets(buffer, USERS_BUFFER_SIZE, file) == NULL){
			return false;
		}

		size_t i = 0;

		while (i < USERS_BUFFER_SIZE - 1 && buffer[i] != 10) {
			i++;
		}

		buffer[i] = 0;

		if (strcmp(buffer, "") == 0) {
			continue;
		}

		if (strcmp(buffer, name) == 0) {
			fclose(file);
			return true;
		}
	}

	fclose(file);
	return false;
}

static void write_to_users_file(const char *name)
{
	FILE *file;

	file = fopen(USERS_FILE, "a");

	if (file == NULL) {
		return;
	}

	fprintf(file, "%s\n", name);

	fclose(file);
}

static void _pam_save_user(const char *name)
{
	if (!search_users_file(name)) {
		write_to_users_file(name);
	}
}
