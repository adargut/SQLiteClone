//
// Created by Adar on 03/10/2020.
//

#ifndef SQLITECLONE_PAGER_H
#define SQLITECLONE_PAGER_H

#include "utils.h"
#include <sys/stat.h>

Pager *pager_open(const char* filename);
char *get_page(Pager *pager, int page_num);
void pager_flush(Pager *pager, uint32_t page_num);

#endif //SQLITECLONE_PAGER_H
