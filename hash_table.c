#include "hash_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void initialise_hash_table(HashTable *ht)
{
    if (ht == NULL) {
        return;
    }
    
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        ht->buckets[i] = NULL;
    }
}

// This function should return a valid hash table
// index (which will be a value >= 0), or a value 
// of -1 if an error or issue is detected.
// Don't use the ERROR return type, use -1 instead
int get_bucket_for_user(int user_id)
{ 
    if (user_id < 0) {
        return -1;
    }
    
    int bucket = user_id % HASH_TABLE_SIZE;
    return bucket; 
}

// This function should return a pointer to a user found
// or a value of NULL otherwise. Don't use the ERROR return type
User *find_user(const HashTable *ht, int user_id)
{

    if (ht == NULL) {
        return NULL;
    }
    
    int bucket = get_bucket_for_user(user_id);
    
    if (bucket == -1) {
        return NULL;
    }

    User *current = ht->buckets[bucket];
    while (current != NULL) {
        if (current->user_id == user_id) {
            return current;
        }
        current = current->next;
    }    
    return NULL;
}

// This function should return SUCCESS upon successful addition
// of user to table (or successful updating of existing user).
// It should return ERROR for any other outcome.
int register_user(HashTable *ht, int user_id, char password[])
{
    if (ht == NULL || user_id < 0 || password == NULL || strlen(password) > MAX_PASSWORD_LEN) {
        return ERROR;
    }

    int bucket = get_bucket_for_user(user_id);
    if (bucket == -1) {
        return ERROR;
    }



    User *registered_user = find_user(ht, user_id);
    if (registered_user != NULL) {
        strcpy(registered_user->password, password);
        return SUCCESS;
    }
    User *new_user = malloc(sizeof(User));
    if (new_user == NULL) {
        return ERROR;
    }
    new_user->user_id = user_id;
    strcpy(new_user->password, password);
    new_user->next = ht->buckets[bucket];
    ht->buckets[bucket] = new_user;
    return SUCCESS;
}

// This function should return SUCCESS if it ascertains that 
// the password provided is a correct match for the relevant
// user in the table (as identified by 'user_id').
// It should return ERROR for any other outcome.
int is_valid_password(const HashTable *ht, int user_id, char password[])
{
    if (ht == NULL || password == NULL || strlen(password) > MAX_PASSWORD_LEN) {
        return ERROR;
    }

    User *user = find_user(ht, user_id);
    if (user == NULL) {
        return ERROR;
    }
    if (strcmp(user->password, password) == 0) {
        return SUCCESS;
    }
    return ERROR;
}

void free_buckets(HashTable *ht)
{
    if (ht == NULL) {
        return;
    }
    
    for (int bucket = 0; bucket < HASH_TABLE_SIZE; bucket++) {
        User *current = ht->buckets[bucket];
        for (; current != NULL; ) {
            User *temp = current;
            current = current->next;
            free(temp);
        }
        ht->buckets[bucket] = NULL;
    }
}

// This function should return SUCCESS upon successful loading
// of data from the file. 
// It should return ERROR for any other outcome.
int load_users_from_file(HashTable *ht, char filename[])
{
    if (ht == NULL || filename == NULL) {
        return ERROR;
    }

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return ERROR;
    }
    int user_id;
    char password[MAX_PASSWORD_LEN + 2];
    int status = SUCCESS;
    int fields_read;

    while ((fields_read = fscanf(file, "%d %41s", &user_id, password)) != EOF) {
        if (fields_read != 2 || register_user(ht, user_id, password) != SUCCESS) {
            status = ERROR;
            break;
        }
    }

    if (ferror(file)) {
        status = ERROR;
    }
    if (fclose(file) != 0) {
        status = ERROR;
    }
    return status;
}
