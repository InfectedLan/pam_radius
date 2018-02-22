#include "radius_users.h"

static bool search_user(const char *name)
{
	FILE *file = fopen(USERS_FILE, "r");

	if (file == NULL) {
		return false;
	}

	while (!feof(file)) {
		char buffer[BUFFER_SIZE];

		if (fgets(buffer, BUFFER_SIZE, file) == NULL){
			return false;
		}

		size_t i = 0;

		while (i < BUFFER_SIZE - 1 && buffer[i] != 10) {
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

static bool add_user(const char *name)
{
	FILE *file;

	file = fopen(USERS_FILE, "a");

	if (file == NULL) {
		return false;
	}

	fprintf(file, "%s\n", name);

	fclose(file);

	return false;
}

static bool save_user_to_file(const char *name)
{
	if (!search_user(name)) {
		return add_user(name);
	}

	return false;
}
