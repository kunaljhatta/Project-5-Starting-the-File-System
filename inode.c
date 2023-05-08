#include "inode.h"
#include "block.h"
#include "free.h"

int ialloc(void){
    unsigned char inode_block[BLOCK_SIZE] = {0};
    bread(FREE_INODE_BLOCK_NUM, inode_block);
    int free_bit = find_free(inode_block);
    if(free_bit != -1){
        set_free(inode_block, free_bit, 1);
    }
    bwrite(FREE_INODE_BLOCK_NUM, inode_block);
    return free_bit;
}
