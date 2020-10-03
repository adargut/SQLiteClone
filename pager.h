//
// Created by Adar on 03/10/2020.
//

#ifndef SQLITECLONE_PAGER_H
#define SQLITECLONE_PAGER_H

#include "utils.h"
#include <sys/stat.h>

typedef struct {
    int fd;
    uint32_t file_length;
    char *pages[TABLE_MAX_PAGES];
} Pager;

Pager *pager_open(const char* filename);
char *get_page(Pager *pager, int page_num);
void pager_flush(Pager *pager, uint32_t page_num, uint32_t size);

#endif //SQLITECLONE_PAGER_H
