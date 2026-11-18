#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <libgen.h>

void reverse_string(char *str) {
    if (!str) return;
    int i = 0, j = strlen(str) - 1;
    while (i < j) {
        char temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++; j--;
    }
}

void reverse_file_content(const char *src_path, const char *dst_path, mode_t mode) {
    int src_fd = open(src_path, O_RDONLY);
    if (src_fd < 0) return;

    int dst_fd = open(dst_path, O_WRONLY | O_CREAT | O_TRUNC, mode);
    if (dst_fd < 0) { close(src_fd); return; }

    struct stat st;
    fstat(src_fd, &st);
    off_t size = st.st_size;
    char buffer;

    for (off_t i = 1; i <= size; i++) {
        lseek(src_fd, -i, SEEK_END);
        if (read(src_fd, &buffer, 1) == 1) {
            write(dst_fd, &buffer, 1);
        }
    }
    close(src_fd);
    close(dst_fd);
}

void reverse_dir(const char *src_path, const char *dst_path) {
    // Создаем текущую целевую директорию
    if (mkdir(dst_path, 0755) && errno != EEXIST) {
        perror("Ошибка mkdir");
        return;
    }

    DIR *dir = opendir(src_path);
    if (!dir) return;

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        // Пропускаем "." и ".."
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        // Формируем полный путь к текущему объекту в источнике
        char src_item_path[1024];
        snprintf(src_item_path, sizeof(src_item_path), "%s/%s", src_path, entry->d_name);

        struct stat st;
        if (lstat(src_item_path, &st) == -1) continue;

        // Инвертируем имя цели
        char rev_name[256];
        strncpy(rev_name, entry->d_name, sizeof(rev_name));
        reverse_string(rev_name);

        // Формируем полный путь к объекту в цели
        char dst_item_path[1024];
        snprintf(dst_item_path, sizeof(dst_item_path), "%s/%s", dst_path, rev_name);

        if (S_ISDIR(st.st_mode)) {
            reverse_dir(src_item_path, dst_item_path);
        } else if (S_ISREG(st.st_mode)) {
            reverse_file_content(src_item_path, dst_item_path, st.st_mode);
        }
    }
    closedir(dir);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Использование: %s <путь_к_каталогу>\n", argv[0]);
        return 1;
    }
    char *src_path = argv[1];

    char *tmp_path = strdup(src_path);
    char *base = basename(tmp_path);
    char *rev_root_name = strdup(base);
    reverse_string(rev_root_name);

    printf("Start reversing: %s -> %s\n", src_path, rev_root_name);

    struct stat st;
    if (lstat(src_path, &st) == -1) return -1;

    if (S_ISDIR(st.st_mode)) {
        reverse_dir(src_path, rev_root_name);
    } else if (S_ISREG(st.st_mode)) {
        reverse_file_content(src_path, rev_root_name, st.st_mode);
    }

    printf("Done!\n");
    free(tmp_path);
    free(rev_root_name);
    return 0;
}
