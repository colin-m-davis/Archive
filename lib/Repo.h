#ifndef REPO_H_
#define REPO_H_

typedef struct repo Repo;
void get_path(const char* from, const char* to, char buf[256]);
void _mkdir(const char *dir);
Repo* repo_init(const char* path);

#endif // REPO_H_