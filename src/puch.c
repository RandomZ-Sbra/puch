#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <termios.h>
#include <fcntl.h>

#define PATH "PUCH_PATH"
#define PORT 8080
#define BUFF_SIZE 16384

typedef struct puch
{
	char *host;
	
} puch_t;


void enable_raw_mode();
void disable_raw_mode();

int main(int argc, char const *argv[])
{
	struct stat check;
	char contact[256];
	char *dir, *cdir, *ldir, *aux;
	int send_pos, rec_pos, t_srv_ready = 1;
	struct dirent *dent;
	DIR *contact_dir;
	DIR *log_dir;
	FILE *messages;
	FILE *rec_lib;
	FILE *send_lib;
	FILE *t_srv_host;

	// FOR DEBUG
	// putenv(PATH);
	// setenv(PATH, "/mnt/e/visual studio/c/puch", 1);

	// Entorno y direcciones a carpetas
	if (!getenv(PATH))
	{
		dir = "/var/lib/puch-messages-v0.2";
		cdir = "/var/lib/puch-messages-v0.2/contactos";
		ldir = "/var/lib/puch-messages-v0.2/logs";
		putenv(PATH);
		setenv(PATH, dir, 1);
	}
	else
	{
		dir = getenv(PATH);
		cdir = (char *)malloc((strlen(dir) + strlen("/contactos")) * sizeof(char));
		strcpy(cdir, dir);
		strcat(cdir, "/contactos");
		ldir = (char *)malloc((strlen(dir) + strlen("/logs")) * sizeof(char));
		strcpy(ldir, dir);
		strcat(ldir, "/logs");
	}
	if (stat(dir, &check) || !S_ISDIR(check.st_mode))
	{
		if (!access(dir, F_OK))
		{
			aux = (char *)malloc((strlen(dir) + strlen(".unknown")) * sizeof(char));
			strcpy(aux, dir);
			strcat(aux, ".unknown");
			rename(dir, aux);
			free(aux);
		}
		mkdir(dir, 0664);
		mkdir(cdir, 0664);
		mkdir(ldir, 0664);
	}
	if (stat(cdir, &check) || !S_ISDIR(check.st_mode))
	{
		if (!access(cdir, F_OK))
		{
			aux = (char *)malloc((strlen(cdir) + strlen(".unknown")) * sizeof(char));
			strcpy(aux, cdir);
			strcat(aux, ".unknown");
			rename(cdir, aux);
			free(aux);
		}
		mkdir(cdir, 0664);
	}
	if (stat(ldir, &check) || !S_ISDIR(check.st_mode))
	{
		if (!access(ldir, F_OK))
		{
			aux = (char *)malloc((strlen(ldir) + strlen(".unknown")) * sizeof(char));
			strcpy(aux, ldir);
			strcat(aux, ".unknown");
			rename(ldir, aux);
			free(aux);
		}
		mkdir(ldir, 0664);
	}

	// Comprueba si el contacto existe
	if (argc == 1)
	{
		printf("Introduce nombre del contacto deseado: ");
		if (!fgets(contact, sizeof(contact), stdin))
		{
			printf("No se ha encontrado el contacto insertado\n");
			exit(0);
		}
		contact[strcspn(contact, "\r\n")] = '\0';
	}
	else
	{
		strcpy(contact, argv[2]);
	}
	aux = (char *)malloc((strlen(cdir) + strlen(contact) + 1) * sizeof(char));
	strcpy(aux, cdir);
	strcat(aux, "/");
	strcat(aux, contact);
	while (!(contact_dir = opendir(aux)))
	{
		stat(aux, &check);
		if (S_ISDIR(check.st_mode))
			fprintf(stderr, "Error: La aplicación no tiene permisos para acceder a \"%s\". Pruebe usar \"sudo\"\n\n", aux);
		else
			fprintf(stderr, "Error: No se ha encontrado la carpeta del contacto \"%s\"\n\n", aux);
		printf("Introduce nombre del contacto deseado: ");
		free(aux);
		if (!fgets(contact, sizeof(contact), stdin))
		{
			printf("\n");
			exit(0);
		}
		contact[strcspn(contact, "\r\n")] = '\0';
		aux = (char *)malloc((strlen(cdir) + strlen(contact) + 1) * sizeof(char));
		strcpy(aux, cdir);
		strcat(aux, "/");
		strcat(aux, contact);
	}
	free(cdir);
	cdir = aux;
	aux = (char *)malloc((strlen(cdir) + strlen("/libretas/Recibir")) * sizeof(char));
	strcpy(aux, cdir);
	strcat(aux, "/libretas/Recibir");
	rec_lib = fopen(aux, "R");
	strcpy(aux, cdir);
	strcat(aux, "/Enviar");
	send_lib = fopen(aux, "R");
	if (!rec_lib || !send_lib)
	{
		fprintf(stderr, "Error: Formato de usuario incompleto, faltan una o más libretas.\n\tSi las libretas están, compruebe que tiene acceso a ellas, use \"sudo\".\n");
		fprintf(stderr, "Saliendo...\n");
		exit(EXIT_FAILURE);
	}

	// Conexión

		// Trusted Server
	
	strcpy(aux, dir);
	strcat(aux, "/contactos/trusted_server");

	if (!access(aux,F_OK) || !access(aux,R_OK))
	{
		char response;
		fprintf(stdout, "No se ha encontrado el fichero del Trusted Server, ¿desea crear uno? (y/n) default y: ");
		response = fgetc(stdin);
		if (response != 'n' && response != 'N')
		{
			while (1)
			{
				char host[256];
				fprintf(stdout, "Host (nombre o dirección): ");
				fgets(host, sizeof(host), stdin);
				host[strcspn(host, "\r\n")] = '\0';
				t_srv_host = fopen(aux, O_WRONLY | O_CREAT | O_TRUNC); //TODO:
			}
		} else 
		{
			fprintf(stderr, "Sin ruta al servidor de confianza, comprobando el host de contacto \"%s/%s\"...\n", cdir, "host");
			t_srv_ready--;
		}
	}
	
	// Proceso t_server
	if (t_srv_ready)
	{
		pid_t t_srv = fork();

		if (t_srv == -1) 
		{
			perror("0xFF");
			fprintf(stderr, "Error al generar proceso de conexión al Trusted Server...\nSaliendo\n");
			exit(EXIT_FAILURE);
		}
		else if (t_srv == 0)
		{
			// TODO...
		}
		else
		{
			int stat;
			waitpid(t_srv, &stat, 0);
			if (WEXITSTATUS(stat) != 0)
			{
				fprintf(stderr, "Error al realizar la conexión con Trusted Server (ERR_TYPE: %d), no se pudo actualizar la dirección del host.\n\tIntentando conectar con la dirección actual.", WEXITSTATUS(stat));

			}
		}
	}
	return 0;
}

// void enable_raw_mode()
// {
// 	struct termios t;
// 	tcgetattr(STDIN_FILENO, &t);
// 	t.c_lflag &= ~(ICANON | ECHO); // Desactivar el modo canónico y el eco
// 	tcsetattr(STDIN_FILENO, TCSANOW, &t);
// }

// void disable_raw_mode()
// {
// 	struct termios t;
// 	tcgetattr(STDIN_FILENO, &t);
// 	t.c_lflag |= (ICANON | ECHO); // Restaurar el modo canónico y el eco
// 	tcsetattr(STDIN_FILENO, TCSANOW, &t);
// }