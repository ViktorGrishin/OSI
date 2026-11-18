#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>

int main() {
    uid_t ruid = getuid();
    uid_t euid = geteuid();

    printf("Идентификаторы процесса:\n");
    printf("  Реальный UID (RUID):    %d\n", ruid);
    printf("  Эффективный UID (EUID): %d\n", euid);

    const char *filename = "secret.txt";

    printf("Попытка чтении файла '%s'...\n", filename);
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        // Если файл не открылся, выводим причину (например, Permission denied)
        fprintf(stderr, "Ошибка открытия файла: %s (errno: %d)\n", strerror(errno), errno);
        printf("Результат: доступ запрещён.\n");
        return EXIT_FAILURE;
    }

    printf("Результат: Доступ разрешён. Содержимое файла:\n");

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        printf("%s", buffer);
    }

    fclose(file);
    return EXIT_SUCCESS;
}