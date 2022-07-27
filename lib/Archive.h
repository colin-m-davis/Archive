#ifndef ARCHIVE_H_
#define ARCHIVE_H_

typedef struct archive {
    char* wt_path;
    char* arc_path; // worktree/.arc
} Archive;
void get_path(const char* from, const char* to, char buf[256]);
void _mkdir(const char *dir);
Archive* archive_init(const char* path);

#endif // ARCHIVE_H_