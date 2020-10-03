//
// Created by Adar on 03/10/2020.
//

#include "pager.h"

Pager *pager_open(const char* filename) {
    int fd = open(filename, _O_RDWR | _O_CREAT, _S_IWUSR, _S_IRUSR);

    if (fd == -1) {
        std::cout << "Critical Error: unable to create db file\n";
        exit(EXIT_SUCCESS);
    }
    Pager *pager = (Pager *)malloc(sizeof(Pager));
    pager->fd = fd;
    pager->file_length = lseek(fd, 0, SEEK_END);
    for (int i = 0; i < TABLE_MAX_PAGES; i++) {
        pager->pages[i] = nullptr;
    }

    return pager;
}

char *get_page(Pager *pager, int page_num) {
    if (page_num > TABLE_MAX_PAGES) {
        std::cout << "Critical Error: fetching page number out of bounds\n";
        exit(EXIT_SUCCESS);
    }

    // Cache miss, need to allocate memory for a page

    if (pager->pages[page_num] == nullptr) {
        auto page = (char *) malloc(PAGE_SIZE);
        uint32_t num_pages = pager->file_length / PAGE_SIZE;
        if (pager->file_length % PAGE_SIZE != 0) num_pages++;

        if (page_num <= num_pages) {
            lseek(pager->fd, page_num * PAGE_SIZE, SEEK_SET);
            ssize_t bytes_read = read(pager->fd, page, PAGE_SIZE);

            if (bytes_read == -1) {
                std::cout << "Critical Error: corrupt db file\n";
                exit(EXIT_SUCCESS);
            }
        }
        pager->pages[page_num] = page;
    }

    return pager->pages[page_num];
}

void pager_flush(Pager *pager, uint32_t page_num, uint32_t size) {
    if (pager->pages[page_num] == nullptr) {
        std::cout << "Critical Error: attempt to flush a null page to disc\n";
        exit(EXIT_FAILURE);
    }
    off_t offset = lseek(pager->fd, page_num * PAGE_SIZE, SEEK_SET);

    if (offset == -1) {
        std::cout << "Critical Error: seeking failed\n";
        exit(EXIT_FAILURE);
    }

    ssize_t bytes_written = write(pager->fd, pager->pages[page_num], size);

    if (bytes_written == -1) {
        std::cout << "Critical Error: writing to file failed\n";
        exit(EXIT_FAILURE);
    }
}