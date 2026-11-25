#include "hash_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
	HashTable users;
	initialise_hash_table(&users);

	if (load_users_from_file(&users, "users.txt") != SUCCESS) {
		fprintf(stderr, "Could not load users.txt\n");
		free_buckets(&users);
		return EXIT_FAILURE;
	}

	for (int bucket = 0; bucket < HASH_TABLE_SIZE; bucket++) {
		for (const User *user = users.buckets[bucket]; user != NULL; user = user->next) {
			printf("Bucket %d: user ID %d, password %s\n",
				   bucket, user->user_id, user->password);
		}
	}

	free_buckets(&users);
	return EXIT_SUCCESS;
}

