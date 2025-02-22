#include <stdlib.h>
#include <stdio.h>

typedef struct contact
{
	char *name;
	char *host;
	int chat_fd;
	int key_fd;
} _contact;


int create_contact(char *name, char *host);
_contact get_contact(char *name);
