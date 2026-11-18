#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <libgen.h>
#include <errno.h>


void read_symlink_target(const char* path) {
    int fd = open(path, O_RDONLY);
    if (fd < 0) {
        perror("open");
        return;
    }
    char buf[4096];
    ssize_t bytes;
    while ((bytes = read(fd, buf, sizeof(buf))) > 0) {
        write(STDOUT_FILENO, buf, bytes);
    }
    close(fd);
    printf("\n");
}

void create_dir(const char* target) {
    if (mkdir(target, 0755) == -1) perror("mkdir");
}

void read_dir(const char* target) {
    DIR* dir = opendir(target);
    if (!dir) {
        perror("opendir");
        return;
    }
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        printf("%s\n", entry->d_name);
    }
    closedir(dir);
}
void delete_dir(const char* target) {
    if (rmdir(target) == -1) perror("rmdir");
}

void create_file(const char* target) {
    int fd = open(target, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd == -1) perror("open");
    else close(fd);
}

void read_file(const char* path) {
    int fd = open(path, O_RDONLY);
    if (fd < 0) {
        perror("open");
        return;
    }
    char buf[4096];
    ssize_t bytes;
    while ((bytes = read(fd, buf, sizeof(buf))) > 0) {
        write(STDOUT_FILENO, buf, bytes);
    }
    close(fd);
    printf("\n");
}

void delete_file(const char* target) {
    if (unlink(target) == -1) perror("unlink");
}

void create_symlink(const char* target, const char* program_name, int argc, char* argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usages: %s <target> <symlink name>\n", program_name);
    }
    if (symlink(target, argv[2]) == -1) perror("symlink");
}

void read_symlink(const char* target) {
    char buf[1024];
    ssize_t len = readlink(target, buf, sizeof(buf) - 1);
    if (len != -1) {
        buf[len] = '\0';
        printf("Link to: %s\n", buf);
    } else perror("readlink");
}


void delete_symlink(const char* target) {
    if (unlink(target) == -1) perror("unlink");
}

int create_hardlink(const char* target, const char* program_name, int argc, char* argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Использование: %s <цель> <имя_ссылки>\n", program_name);
        return 1;
    }
    if (link(target, argv[2]) == -1) perror("link");
    return 0;
}

void delete_hardlink(const char* target) {
    if (unlink(target) == -1) perror("unlink");
}


int stat_file(const char* target) {
    struct stat st;
    if (stat(target, &st) == -1) {
        perror("stat");
        return 1;
    }
    printf("Количество жестких ссылок: %lu\n", (unsigned long) st.st_nlink);
    printf("Права доступа (octal): %o\n", st.st_mode & 0777);
    return 0;
}


int chmod_file(const char* target, const char* program_name, int argc, char* argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Использование: %s <файл> <права_в_8-ричной_системе (напр. 0777)>\n", program_name);
        return 1;
    }
    mode_t mode = strtol(argv[2], NULL, 8);
    if (chmod(target, mode) == -1) perror("chmod");
    return 0;
}


int main(int argc, char* argv[]) {
    // Получаем чистое имя, по которому вызвали программу (без ./ )
    char* program_name = basename(argv[0]);

    if (argc < 2) {
        fprintf(stderr, "Not enough arguments\n");
        return 1;
    }

    const char* target = argv[1];

    if (strcmp(program_name, "create_dir") == 0) {
        create_dir(target);
    } else if (strcmp(program_name, "read_dir") == 0) {
        read_dir(target);
    } else if (strcmp(program_name, "delete_dir") == 0) {
        delete_dir(target);
    } else if (strcmp(program_name, "create_file") == 0) {
        create_file(target);
    } else if (strcmp(program_name, "read_file") == 0) {
        read_file(target);
    } else if (strcmp(program_name, "delete_file") == 0) {
        delete_file(target);
    } else if (strcmp(program_name, "create_symlink") == 0) {
        create_symlink(target, program_name, argc, argv);
    } else if (strcmp(program_name, "read_symlink") == 0) {
        read_symlink(target);
    } else if (strcmp(program_name, "read_symlink_target") == 0) {
        read_symlink_target(target);
    } else if (strcmp(program_name, "delete_symlink") == 0) {
        delete_symlink(target);
    } else if (strcmp(program_name, "create_hardlink") == 0) {
        create_hardlink(target, program_name, argc, argv);
    } else if (strcmp(program_name, "delete_hardlink") == 0) {
        delete_hardlink(target);
    } else if (strcmp(program_name, "stat_file") == 0) {
        stat_file(target);
    } else if (strcmp(program_name, "chmod_file") == 0) {
        chmod_file(target, program_name, argc, argv);
    } else {
        fprintf(stderr, "Unknown command: %s\n", program_name);
    }

    return 0;
}
