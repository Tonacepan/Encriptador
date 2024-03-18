#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_MESSAGE_LENGTH 1000

char message[MAX_MESSAGE_LENGTH];
int key;
sem_t sem;

void *encrypt(void *arg) {
    sem_wait(&sem); // Esperar a que el semáforo esté libre
    char *ptr = (char *) arg;
    int i;
    for (i = 0; ptr[i] != '\0'; i++) {
        if (ptr[i] >= 'A' && ptr[i] <= 'Z') {
            ptr[i] = ((ptr[i] - 'A') + key) % 26 + 'A';
        } else if (ptr[i] >= 'a' && ptr[i] <= 'z') {
            ptr[i] = ((ptr[i] - 'a') + key) % 26 + 'a';
        }
    }
    sem_post(&sem); // Liberar el semáforo
    return NULL;
}

int main() {
    printf("Ingrese el mensaje a encriptar: ");
    fgets(message, MAX_MESSAGE_LENGTH, stdin);
    printf("Ingrese la clave de encriptación: ");
    scanf("%d", &key);

    sem_init(&sem, 0, 1); // Inicializar el semáforo con un valor de 1

    pthread_t thread;
    pthread_create(&thread, NULL, encrypt, message);

    pthread_join(thread, NULL); // Esperar a que el hilo termine

    printf("Mensaje encriptado: %s\n", message);

    sem_destroy(&sem); // Destruir el semáforo

    return 0;
}
