/**
 * @file    executor.cc
 * @author  liugang(liugang@ict.ac.cn)
 * @version 0.1
 *
 * @section DESCRIPTION
 *  
 * definition of executor
 *
 */
#include "executor.h"

int Executor::exec(SelectQuery *query, ResultTable *result)
{
    //---------write your code here-------------------------


    return 0;
}

int Executor::close() 
{
    //---------write your code here-------------------------
    
    return 0;
}

// note: you should guarantee that col_types is useable as long as this ResultTable in use, maybe you can new from operate memory, the best method is to use g_memory.
int ResultTable::init(BasicType *col_types[], int col_num, int64_t capicity) {
    column_type = col_types;
    column_number = col_num;
    row_length = 0;
    buffer_size = g_memory.alloc (buffer, capicity);
    if(buffer_size != capicity) {
        printf ("[ResultTable][ERROR][init]: buffer allocate error!\n");
        return -1;
    }
    int allocate_size = 1;
    int require_size = sizeof(int)*column_number;
    while (allocate_size < require_size)
        allocate_size = allocate_size << 1;
    char *p = NULL;
    offset_size = g_memory.alloc(p, allocate_size);
    if (offset_size != allocate_size) {
        printf ("[ResultTable][ERROR][init]: offset allocate error!\n");
        return -2;
    }
    offset = (int*) p;
    for(int ii = 0;ii < column_number;ii ++) {
        offset[ii] = row_length;
        row_length += column_type[ii]->getTypeSize(); 
    }
    row_capicity = (int)(capicity / row_length);
    row_number   = 0;
    return 0;
}

int ResultTable::print (void) {
    int row = 0;
    int ii = 0;
    char buffer[1024];
    char *p = NULL; 
    while(row < row_number) {
        for( ; ii < column_number-1; ii++) {
            p = getRC(row, ii);
            column_type[ii]->formatTxt(buffer, p);
            printf("%s\t", buffer);
        }
        p = getRC(row, ii);
        column_type[ii]->formatTxt(buffer, p);
        printf("%s\n", buffer);
        row ++; ii=0;
    }
    return row;
}

int ResultTable::dump(FILE *fp) {
    // write to file
    int row = 0;
    int ii = 0;
    char buffer[1024];
    char *p = NULL; 
    while(row < row_number) {
        for( ; ii < column_number-1; ii++) {
            p = getRC(row, ii);
            column_type[ii]->formatTxt(buffer, p);
            fprintf(fp,"%s\t", buffer);
        }
        p = getRC(row, ii);
        column_type[ii]->formatTxt(buffer, p);
        fprintf(fp,"%s\n", buffer);
        row ++; ii=0;
    }
    return row;
}

// this include checks, may decrease its speed
char* ResultTable::getRC(int row, int column) {
    return buffer+ row*row_length+ offset[column];
}

int ResultTable::writeRC(int row, int column, void *data) {
    char *p = getRC (row,column);
    if (p==NULL) return 0;
    return column_type[column]->copy(p,data);
}

int ResultTable::shut (void) {
    // free memory
    g_memory.free (buffer, buffer_size);
    g_memory.free ((char*)offset, offset_size);
    return 0;
}

//---------------------operators implementation---------------------------
// ...
// ...
