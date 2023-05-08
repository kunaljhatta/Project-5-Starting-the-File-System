#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "free.h"
#include "image.h"
#include "mkfs.h"
#include "ctest.h"
#include "inode.h"
#include "block.h"

void test_image_open(){   
    char *file = "Hello.txt!";
    CTEST_ASSERT((image_open(file, 0) != -1), "File is not there!");
    remove(file);
}

void test_image_close(){
    char *file = "test.txt";
    image_open(file, 1);
    CTEST_ASSERT(image_close() == 0, "");
    remove(file);
}

void test_bwrite_and_bread(){
	unsigned char test[BLOCK_SIZE] = {0}; 
	image_open("anyfile.txt", 0); 
	bwrite(9, test);
	unsigned char read[BLOCK_SIZE];
	bread(9, read);
	CTEST_ASSERT(memcmp(test, read, 3) == 0, "Testing bwrite and bread");
	image_close();
}

void test_alloc(){
    image_open("anyfile.txt", 0);
    CTEST_ASSERT(alloc() == 0, "empty block map");
    CTEST_ASSERT(alloc() == 1, "non-empty block map");
    image_close();
}

void test_set_free_and_find_free(void) {
    unsigned char data[4096] = {0};
    CTEST_ASSERT(find_free(data) == 0, "testing empty");
    set_free(data, 0, 1);
    CTEST_ASSERT(find_free(data) == 1, "testing find_free to 1 and set_free to 1");
    set_free(data, 1, 1);
    CTEST_ASSERT(find_free(data) == 2, "testing find_free to 2 and set_free to 1");
    set_free(data, 0, 0);
    CTEST_ASSERT(find_free(data) == 0, "testing find_free to 0 and set_free to 0");
}

void test_ialloc(void) {
	image_open("any.txt", 0);
	int first = ialloc();
	CTEST_ASSERT(first == 0, "testing first block");
	int second = ialloc();
	CTEST_ASSERT(second == 1, "testing second block");
    image_close();
    remove("any.txt");
}

void test_mkfs(void) {
    image_open("test_file.txt", 0);
    unsigned char test_block[4096] = {0};
    unsigned char test_outblock[4096];
    mkfs();
    CTEST_ASSERT(memcmp(bread(8, test_outblock), test_block, 4) == 0, "setting blocks to 0");
    int next_free_block = alloc();
    CTEST_ASSERT(next_free_block == 7, "testing next free block");
    image_close();
    remove("test_file.txt");
}

int main(void)
{
    CTEST_VERBOSE(1);
    test_image_open();
    test_image_close();
    test_alloc();
    test_bwrite_and_bread();
    test_set_free_and_find_free();
    test_ialloc();
    test_mkfs();
    CTEST_RESULTS();
    CTEST_EXIT();
}
