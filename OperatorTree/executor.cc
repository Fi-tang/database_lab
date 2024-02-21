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

/** use gloabl pointer.pointer to the compare function of qsort
 * first want to place it in Orderby Operator, but it stays at dump
 * and cause segment fault,so choose to place it as global variable.                   
 */
Orderby * orderby_cmp;

int Executor::exec(SelectQuery *query, ResultTable *result)
{
    //---------write your code here-------------------------
    static Operator *root;

    static Scan scan_node[4];
    static Filter filter_node[4];
    static Join join_node[4];
    static Project project_node[1];

    static Groupby groupby_node[1];
    static Filter having_node[1];
    static Orderby orderby_node[1];

    if(query){
        root = project_node;

        for (int i = 0; i < query->from_number; i++)
        {
            scan_node[i].init(query->from_table[i].name);
        }
        // conditions , at most 4
        int filter_and_join_num;
        filter_and_join_num = query->where.condition_num;

        // 4 conditions -> 4 buckets
        Conditions filter_conditions[4];
        for (int i = 0; i < 4; i++)
        {
            filter_conditions[i].condition_num = 0;
        }

        for (int i = 0; i < filter_and_join_num; i++)
        {
            if (query->where.condition[i].compare == CompareMethod:: LINK)
            {
                continue;
            }
            Column *column_id = (Column *)g_catalog.getObjByName(query->where.condition[i].column.name);
            int column_cid;
            column_cid = column_id->getOid();
            for (int j = 0; j < query->from_number; j++)
            {
                Table *table_id = (Table *)g_catalog.getObjByName(query->from_table[j].name);
                if (table_id->getColumnRank(column_cid) >= 0)
                {
                    filter_conditions[j].condition[filter_conditions[j].condition_num] = query->where.condition[i];
                    filter_conditions[j].condition_num++;
                }
            }
        }
        
        for (int i = 0; i < 4; i++)
        {
            if (filter_conditions[i].condition_num == 0)
            {
                continue;
            }
            else
            {
                filter_node[i].lchild = &scan_node[i];
                filter_node[i].rchild = NULL;
                filter_node[i].init(filter_conditions[i]);
            }
        }
        
        // join_node
        Conditions join_conditions[4];
        for (int i = 0; i < 4; i++)
        {
            join_conditions[i].condition_num = 0;
        }

        if (query->from_number > 2)
        {
            class TypeInt8 *padding[2];
            class TypeInt8 paddings[2];
            padding[0] = &paddings[0];
            padding[1] = &paddings[1];
            // padding[0] = (TypeInt8 *)malloc(sizeof(TypeInt8));
            // padding[1] = (TypeInt8 *)malloc(sizeof(TypeInt8));
            result->init((BasicType**)padding, 2);
            return 0;
        }

        for (int i = 0; i < filter_and_join_num; i++)
        {
            if (query->where.condition[i].compare != CompareMethod:: LINK)
            {
                continue;
            }
            join_conditions[0].condition[join_conditions[0].condition_num] = query->where.condition[i];
            join_conditions[0].condition_num++;
        }

        if (query->from_number == 2)
        {
            if (filter_conditions[0].condition_num > 0)
            {
                join_node[0].lchild = &filter_node[0];
            }
            else if (filter_conditions[0].condition_num == 0)
            {
                join_node[0].lchild = &scan_node[0];
            }
            if (filter_conditions[1].condition_num > 0)
            {
                join_node[0].rchild = &filter_node[1];
            }
            else if (filter_conditions[1].condition_num == 0)
            {
                join_node[0].rchild = &scan_node[1];
            }
            join_node[0].init(join_conditions[0]);
        }
        
        // Groupby_node
        // int groupby_number ; 需要对哪几列进行groupby
        // RequestColumn groupby[4]; 需要groupby的列名称
        int groupby_num; 
        groupby_num = query->groupby_number;

        RequestColumn groupby_conditions[4];
        int groupby_compute_num = 0;

        for(int i = 0; i < query->select_number; i++){
            if(query->select_column[i].aggrerate_method == NONE_CM){
                continue;
            }
            else{
                groupby_conditions[groupby_compute_num++]= query -> select_column[i];
            }
        }

        if (query->from_number == 2)
        {
            groupby_node[0].lchild = &join_node[0];
        }
        else if (query->from_number == 1)
        {
            if (filter_conditions[0].condition_num > 0)
            {
                groupby_node[0].lchild = &filter_node[0];
            }
            else if (filter_conditions[0].condition_num == 0)
            {
                groupby_node[0].lchild = &scan_node[0];
            }
        }
      
        if(groupby_num > 0){
             groupby_node[0].rchild = NULL;
             groupby_node[0].init(query->groupby_number, groupby_conditions,groupby_compute_num,query->groupby);
        }
        
        // having node;
        int filter_having_num;
        filter_having_num = query->having.condition_num;

        if(filter_having_num > 0){
            having_node[0].lchild = &groupby_node[0];
            having_node[0].rchild = NULL;
            having_node[0].init(query->having);
        }
        // orderby node;
        orderby_cmp = orderby_node;
        int orderby_num;
        orderby_num = query->orderby_number;

        if(filter_having_num > 0){
            orderby_node[0].lchild = &having_node[0];
        }
        else if(groupby_num > 0){
            orderby_node[0].lchild = &groupby_node[0];
        }
        else{
            orderby_node[0].lchild = groupby_node[0].lchild;
        }

        if(orderby_num > 0){
            orderby_node[0].rchild = NULL;
            orderby_node[0].init(query-> orderby_number,query->orderby);
        }
        
        // Project_node
        if(orderby_num > 0){
            project_node[0].lchild = &orderby_node[0];
        }
        else{
            project_node[0].lchild = orderby_node[0].lchild;
        } 
        
        project_node[0].rchild = NULL;
        project_node[0].init(query->select_number, query->select_column);
    }

    printf("\ntree:\n");
    root->print(0);

    int count = 0;
    BasicType **column_types;
    column_types = (BasicType **)malloc((root->Column_id_array.size()+1) * sizeof(BasicType *));

    for(int i = 0; i < root->Column_id_array.size(); i++){
        Column *get_column_id = (Column *)g_catalog.getObjById(root->Column_id_array[i]);
        column_types[i] = get_column_id->getDataType();
    }
    class TypeInt8 validation_int8;
    column_types[root->Column_id_array.size()] = &validation_int8;
    /** we need the Valiadation 'Y'--> result may print it as 89. */ 
    result -> init(column_types, root-> Column_id_array.size()+1);
    
    while ((root->getNext()) != false){
        for(int j = 0; j < root-> Column_id_array.size(); j++){ 
            result->writeRC(count,j, root->current_buffer + root->row_column_RPattern.getColumnOffset(j));
        }
        char valid = 'Y';
        result->writeRC(count, root-> Column_id_array.size(), &valid);
        count++;
        if(count == 1024*1024 / root->row_column_RPattern.getRowSize()) break;
    }
    result->row_number = count;
    
    if(count == 0){
        root->isEnd();
    }
    return count;

}

int Executor::close()
{
    //---------write your code here-------------------------

    return 0;
}

// note: you should guarantee that col_types is useable as long as this ResultTable in use, maybe you can new from operate memory, the best method is to use g_memory.
int ResultTable::init(BasicType *col_types[], int col_num, int64_t capicity)
{
    column_type = col_types;
    column_number = col_num;
    row_length = 0;
    buffer_size = g_memory.alloc(buffer, capicity);
    if (buffer_size != capicity)
    {
        printf("[ResultTable][ERROR][init]: buffer allocate error!\n");
        return -1;
    }
    int allocate_size = 1;
    int require_size = sizeof(int) * column_number;
    while (allocate_size < require_size)
        allocate_size = allocate_size << 1;
    char *p = NULL;
    // printf("require: %d alloc: %d\n", require_size, allocate_size);
    offset_size = g_memory.alloc(p, allocate_size);
    if (offset_size != allocate_size)
    {
        printf("[ResultTable][ERROR][init]: offset allocate error!\n");
        return -2;
    }
    
    offset = (int *)p;
    for (int ii = 0; ii < column_number; ii++)
    {
        offset[ii] = row_length;
        row_length += column_type[ii]->getTypeSize();
    }
    row_capicity = (int)(capicity / row_length);
    row_number = 0;
    return 0;
}

int ResultTable::print(void)
{
    int row = 0;
    int ii = 0;
    char buffer[1024];
    char *p = NULL;
    while (row < row_number)
    {
        for (; ii < column_number - 1; ii++)
        {
            p = getRC(row, ii);
            column_type[ii]->formatTxt(buffer, p);
            printf("%s\t", buffer);
        }
        p = getRC(row, ii);
        column_type[ii]->formatTxt(buffer, p);
        printf("%s\n", buffer);
        row++;
        ii = 0;
    }
    return row;
}

int ResultTable::dump(FILE *fp)
{
    // write to file
    int row = 0;
    int ii = 0;
    char buffer[1024];
    char *p = NULL;
    while (row < row_number)
    {
        for (; ii < column_number - 1; ii++)
        {
            p = getRC(row, ii);
            column_type[ii]->formatTxt(buffer, p);
            fprintf(fp, "%s\t", buffer);
        }
        p = getRC(row, ii);
        column_type[ii]->formatTxt(buffer, p);
        fprintf(fp, "%s\n", buffer);
        row++;
        ii = 0;
    }
    return row;
}

// this include checks, may decrease its speed
char *ResultTable::getRC(int row, int column)
{
    return buffer + row * row_length + offset[column];
}

int ResultTable::writeRC(int row, int column, void *data)
{
    char *p = getRC(row, column);
    if (p == NULL)
        return 0;
    return column_type[column]->copy(p, data);
}

int ResultTable::shut(void)
{
    // free memory
    g_memory.free(buffer, buffer_size);
    g_memory.free((char *)offset, offset_size);
    return 0;
}

//---------------------operators implementation---------------------------
// ...
// ...

bool Scan::init(char *table_name)
{
    scan_row_table = (RowTable *)g_catalog.getObjByName(table_name);

    int column_total_num;
    column_total_num = scan_row_table ->getColumns().size();
    if(row_column_RPattern.init(column_total_num + 1)){
        for(int i = 0; i < column_total_num; i++){
            BasicType * scan_column_type = scan_row_table->getRPattern().getColumnType(i);
            row_column_RPattern.addColumn(scan_column_type);
        }
        class TypeInt8 validation_int8;
        row_column_RPattern.addColumn(&validation_int8);
    }
    
    Column_id_array.assign(scan_row_table->getColumns().begin(), scan_row_table->getColumns().end());
    
    scan_table_num = scan_row_table->getRecordNum();
    scan_count = 0;
    current_buffer = (char *)malloc(row_column_RPattern.getRowSize());
    return true;
}

bool Scan::getNext()
{
    if (scan_count < scan_table_num)
    {
        memcpy(current_buffer,scan_row_table->getRecordPtr(scan_count),row_column_RPattern.getRowSize());
        scan_count++;
        return true;
    }
    return false;
}

bool Scan::isEnd()
{
    lchild = NULL;
    rchild = NULL;
    parent = NULL;
    row_column_RPattern.reset();
    row_column_RPattern.shut();
    Column_id_array.clear();

    if(current_buffer != NULL){
        free(current_buffer);
    }
    return true;
}

bool Filter::init(Conditions filter_given_conditions)
{
    filter_judge_num = filter_given_conditions.condition_num;
    for(int i = 0; i < filter_judge_num; i++){
        filter_judge_condition[i] = filter_given_conditions.condition[i];
    }

    int column_total_num;
    column_total_num = ((Operator *)lchild)->Column_id_array.size();
    if(row_column_RPattern.init(column_total_num + 1)){
        for(int i = 0; i < column_total_num; i++){
            BasicType * filter_column_type = ((Operator *)lchild)->row_column_RPattern.getColumnType(i);
            row_column_RPattern.addColumn(filter_column_type);
        }
        class TypeInt8 validation_int8;
        row_column_RPattern.addColumn(&validation_int8);
    }

    Column_id_array.assign(((Operator *)lchild) -> Column_id_array.begin(), ((Operator *)lchild) -> Column_id_array.end());
    prev_buffer = (char *)malloc(row_column_RPattern.getRowSize());
    prev_buffer = ((Operator *)lchild)->current_buffer;
    current_buffer = (char *)malloc(row_column_RPattern.getRowSize());

    for(int i = 0; i < filter_judge_num; i++){
        Column * data1_column_id = (Column *)g_catalog.getObjByName(filter_judge_condition[i].column.name);
        int Column_rank;
        for(int j = 0; j < Column_id_array.size(); j++){
            if(Column_id_array[j] == data1_column_id->getOid()){
                 Column_rank = j;
                 break;
            }
        }
        Column_offset[i] = row_column_RPattern.getColumnOffset(Column_rank);
        column_data_type[i] = data1_column_id->getDataType();
    }
    return true;
}
/**
 * the function is used in filter.
 * @param type  point out the BasicType.
 * @param data1 operand1
 * @param value we first need to transfer char* to correst BasicType, operand2;
 * @param method the compare type LT/LE/EQ/NE/GT/GE
 * @retval true compare result
 * @retval false compare result --> the opposite
 */ 
bool compare(BasicType * type, void* data1, char* value, CompareMethod method){
    void *data2;
    data2 = (void *)malloc(1024);
    type->formatBin(data2,value);

    if(method == CompareMethod::LT){
        return type->cmpLT(data1,data2); 
    }
    else if(method == CompareMethod::LE){
        return type->cmpLE(data1,data2); 
    }
    else if(method == CompareMethod::EQ){
        return type->cmpEQ(data1,data2); 
    }
    else if(method == CompareMethod::NE){
        return !type->cmpEQ(data1,data2); 
    }
    else if(method == CompareMethod::GT){
        return type->cmpGT(data1,data2); 
    }
    else if(method == CompareMethod::GE){
        return type->cmpGE(data1,data2); 
    }
}

bool Filter::getNext()
{
    while(true){
        if(((Operator *)lchild)->getNext() == false) return false;
        int flag = 0;
        for(int i = 0; i < filter_judge_num; i++){
            if(!compare(column_data_type[i] ,prev_buffer + Column_offset[i],filter_judge_condition[i].value, filter_judge_condition[i].compare)){
                // printf("%s %s\n", column_data_type[i]->formatTxt(prev_buffer + Column_offset[i]), filter_judge_condition[i].value);
                flag = 1;
                break;
            }
        }
        if(flag){
            continue;
        }
        memcpy(current_buffer,prev_buffer,row_column_RPattern.getRowSize());
        // row_column_RPattern.print(current_buffer);
        // printf("\n");
        return true;
    }
}

bool Filter::isEnd()
{
    ((Operator *)lchild)->isEnd();
    lchild = NULL;
    rchild = NULL;
    parent = NULL;
    row_column_RPattern.reset();
    row_column_RPattern.shut();
    Column_id_array.clear();

    if(current_buffer != NULL){
        free(current_buffer);
    }
    return true;
}

bool Project::init(int project_given_number, RequestColumn *project_given_request){
    project_column_number = project_given_number;
    for(int i = 0; i < project_column_number; i++){
        Column_id_array.push_back(g_catalog.getObjByName(project_given_request[i].name)->getOid());
    }

    if(row_column_RPattern.init(project_column_number + 1)){
        // printf("******%d\n", ((Operator *)lchild)->Column_id_array.size());
        for(int i = 0; i < project_column_number; i++){
            for(unsigned int ii = 0; ii < ((Operator *)lchild)->Column_id_array.size(); ii++){
                // printf("%d: %d\n", ii, ((Operator *)lchild)->Column_id_array[ii]);
                if(Column_id_array[i] == ((Operator *)lchild)->Column_id_array[ii]){
                    project_column_rank[i] = ii;
                    BasicType * project_column_type = ((Operator *)lchild)->row_column_RPattern.getColumnType(ii);
                    row_column_RPattern.addColumn(project_column_type);
                    break;
                }
            }
        }
        class TypeInt8 validation_int8;
        row_column_RPattern.addColumn(&validation_int8);
    }

    prev_buffer = ((Operator *)lchild)->current_buffer;
    current_buffer = (char *)malloc(row_column_RPattern.getRowSize());
    return true;
}

bool Project::getNext(){
    
    if(((Operator *)lchild)->getNext() == false)  return false;
    
    for(int i = 0; i < project_column_number; i++){
        // 1. rank buffer -> original offset
        // 2. rpattern -> current offset
        // 3. type -> copy
        int origin_offset;
        origin_offset = ((Operator *)lchild)->row_column_RPattern.getColumnOffset(project_column_rank[i]);
        int current_offset;
        current_offset = row_column_RPattern.getColumnOffset(i);
        Column *project_copy_id = (Column *)g_catalog.getObjById(Column_id_array[i]);
        BasicType *project_copy_types = project_copy_id->getDataType();
        
        project_copy_types -> copy(current_buffer + current_offset,  ( ((Operator *)lchild)->current_buffer ) + origin_offset );
    }
    return true;
}

bool Project::isEnd(){
    ((Operator *)lchild)->isEnd();
    lchild = NULL;
    rchild = NULL;
    parent = NULL;
    row_column_RPattern.reset();
    row_column_RPattern.shut();
    Column_id_array.clear();
    
    if(current_buffer != NULL){
        free(current_buffer);
    }
    return true;
}

bool Join::init(Conditions join_given_conditions){
    
    join_given_condition_num = join_given_conditions.condition_num;
    for(int i = 0; i < join_given_condition_num; i++){
        Column_id_array_prepare.push_back(g_catalog.getObjByName(join_given_conditions.condition[i].column.name)->getOid()); 
        Column_id_array_join.push_back(g_catalog.getObjByName(join_given_conditions.condition[i].value)->getOid());
    }
   
    if(row_column_RPattern.init( ((Operator *)lchild)->Column_id_array.size() + ((Operator *)rchild)->Column_id_array.size() + 1)){
        for(int i = 0; i < ((Operator *)lchild)->Column_id_array.size(); i++){
            Column_id_array.push_back(((Operator *)lchild)->Column_id_array[i]);
            BasicType * join_column_type = ((Operator *)lchild)->row_column_RPattern.getColumnType(i);
            row_column_RPattern.addColumn(join_column_type);
            for(int j = 0; j < Column_id_array_prepare.size(); j++){
                if(((Operator *)lchild)->Column_id_array[i] == Column_id_array_prepare[j]){
                    join_lchild_rank[j] = i;
                }
                else if(((Operator *)lchild)->Column_id_array[i] == Column_id_array_join[j]){
                    join_lchild_rank[j] = i;
                    int temp;
                    temp = Column_id_array_prepare[j];
                    Column_id_array_prepare[j] = Column_id_array_join[j];
                    Column_id_array_join[j] = temp; 
                }
            }
        }

        for(int i = 0; i < ((Operator *)rchild)->Column_id_array.size(); i++){
            Column_id_array.push_back(((Operator *)rchild)->Column_id_array[i]);
            BasicType * join_column_type = ((Operator *)rchild)->row_column_RPattern.getColumnType(i);
            row_column_RPattern.addColumn(join_column_type);
            for(int j = 0; j < Column_id_array_prepare.size(); j++){
                if(((Operator *)rchild)->Column_id_array[i] == Column_id_array_join[j]){
                    join_rchild_rank[j] = i;
                }
            }
        }

        class TypeInt8 validation_int8;
        row_column_RPattern.addColumn(&validation_int8);
    }
    // prev_buffer = ((Operator *)lchild)->current_buffer;
    current_buffer = (char *)malloc(row_column_RPattern.getRowSize());
    class Key key1;
    key1.set( Column_id_array_prepare);

    hx = new HashIndex(100, "hashindex-100", key1);
  
    hx->init();
    hx->setCellCap(9);
    for(int i = 0; i < Column_id_array_prepare.size(); i++){
        BasicType *Index_lchild_ColumnType = ((Operator *)lchild) -> row_column_RPattern.getColumnType(join_lchild_rank[i]);
        int64_t Index_lchild_ColumnOffset = ((Operator *)lchild) -> row_column_RPattern.getColumnOffset(join_lchild_rank[i]);
        hx->addIndexDTpye(Index_lchild_ColumnType,Index_lchild_ColumnOffset);
    }
    
    hx->finish();
    
    void *data[((Operator*)lchild)->Column_id_array.size()];

    while( ((Operator *)lchild)->getNext() == true){
        char *temp;
        temp = (char *)malloc(((Operator *)lchild)->row_column_RPattern.getRowSize());
        memcpy(temp,((Operator *)lchild)->current_buffer,((Operator *)lchild)->row_column_RPattern.getRowSize());
        insert_hash_data.push_back(temp);
        for(int i = 0; i < Column_id_array_prepare.size(); i++){
            data[i] = temp + ((Operator *)lchild) -> row_column_RPattern.getColumnOffset(join_lchild_rank[i]);
        }
        hx->insert(data,temp);
    }

    data_rchild = (void**)malloc(((Operator *)rchild)->Column_id_array.size() * sizeof(void *));
    get_next_judge = true;
    lookup_hash_data = (char*)malloc(((Operator *)rchild)->row_column_RPattern.getRowSize());

    return true;
}


bool Join::getNext(){
    void *temp;
    while(true){
        if(get_next_judge){
           if(((Operator *)rchild)->getNext()){
                memcpy(lookup_hash_data, ((Operator *)rchild)->current_buffer,((Operator *)rchild)->row_column_RPattern.getRowSize());
                for(int i = 0; i < Column_id_array_join.size(); i++){
                    data_rchild[i] = lookup_hash_data + ((Operator *)rchild) -> row_column_RPattern.getColumnOffset(join_rchild_rank[i]);
                }
                hx->set_ls(data_rchild, NULL, &hf);

                if(hx->lookup(data_rchild,&hf,temp)){
                    get_next_judge = false;
                    memcpy(current_buffer,temp,((Operator *)lchild)->row_column_RPattern.getRowSize());
                    memcpy(current_buffer + row_column_RPattern.getColumnOffset(((Operator *)lchild)->Column_id_array.size()),lookup_hash_data, ((Operator *)rchild)->row_column_RPattern.getRowSize());
                    return true;
                }
                else if(hx->lookup(data_rchild,&hf,temp) == false){
                    get_next_judge = true;
                    continue;
                }
           }
           else{
                return false;
           }
        }

        else{
           if(hx->lookup(data_rchild,&hf,temp)){
                get_next_judge = false;
                memcpy(current_buffer,temp,((Operator *)lchild)->row_column_RPattern.getRowSize());
                memcpy(current_buffer + row_column_RPattern.getColumnOffset(((Operator *)lchild)->Column_id_array.size()),lookup_hash_data, ((Operator *)rchild)->row_column_RPattern.getRowSize());
                return true;
            }
            else if(hx->lookup(data_rchild,&hf,temp) == false){
                get_next_judge = true;
                continue;
            } 
        }
    }
}

bool Join::isEnd(){
    ((Operator *)lchild)->isEnd();
    ((Operator *)rchild)->isEnd();
    lchild = NULL;
    rchild = NULL;
    parent = NULL;
    row_column_RPattern.reset();
    row_column_RPattern.shut();
    Column_id_array_prepare.clear();
    Column_id_array_join.clear();
    Column_id_array.clear();
    for(int i = 0; i < insert_hash_data.size(); i++){
        free(insert_hash_data[i]);
    }
    insert_hash_data.clear();
    
    if(current_buffer != NULL){
        free(current_buffer);
    }
    hx->shut();
    free(data_rchild);
    free(lookup_hash_data);

    return true;
}

/**
 * compute different types data's sum.
 * refer to datatype.h
 * @param data_type  use this to get datatype.h 's function getTypeCode()
 * @param data1   operand1
 * @param data2   operand2
 * @retval true   we have successfully write the result to data1
 * @retval false  write fail,can cause segment fault.
 */ 
bool Sum(BasicType *data_type, void *data1, void *data2){
    TypeCode type_code = data_type->getTypeCode();
    if(type_code == INT8_TC){
        *(int8_t *) data1 += *(int8_t *) data2;
    }
    else if(type_code == INT16_TC){
        *(int16_t *) data1 += *(int16_t *) data2;
    }
    else if(type_code == INT32_TC){
        *(int32_t *) data1 += *(int32_t *) data2;
    }
    else if(type_code == INT64_TC){
        *(int64_t *) data1 += *(int64_t *) data2;
    }
    else if(type_code == FLOAT32_TC){
        *(float *) data1 += *(float *) data2;
    }
    else if(type_code == FLOAT64_TC){
        *(double *) data1 += *(double *) data2;
    }
    return true;
}

/**
 * compute different types data's Divide.
 * refer to datatype.h
 * @param data_type  use this to get datatype.h 's function getTypeCode()
 * @param data1   operand1
 * @param data2   operand2
 * @retval true   we have successfully write the result to data1,function successfully
 * compute operand1 / operand2
 * @retval false  write fail,can cause segment fault.
 */ 
bool Divide(BasicType *data_type, void *data1, void *data2){
    TypeCode type_code = data_type->getTypeCode();
    if(type_code == INT8_TC){
        *(int8_t *) data1 /= *(int64_t *) data2;
    }
    else if(type_code == INT16_TC){
        *(int16_t *) data1 /= *(int64_t *) data2;
    }
    else if(type_code == INT32_TC){
        *(int32_t *) data1 /= *(int64_t *) data2;
    }
    else if(type_code == INT64_TC){
        *(int64_t *) data1 /= *(int64_t *) data2;
    }
    else if(type_code == FLOAT32_TC){
        *(float *) data1 /= *(int64_t *) data2;
    }
    else if(type_code == FLOAT64_TC){
        *(double *) data1 /= *(int64_t *) data2;
    }
    return true;
}

bool Groupby::init(int groupby_num, RequestColumn groupby_conditions[4], int groupby_compute_number, RequestColumn groupby_column[4]){
    int column_total_num;
    column_total_num = ((Operator *)lchild)->Column_id_array.size();

    groupby_number = groupby_num;
    groupby_compute_num = groupby_compute_number;
    if(row_column_RPattern.init(column_total_num + 1)){
        for(int i = 0; i < column_total_num; i++){
            BasicType *groupby_column_type = ((Operator *)lchild)->row_column_RPattern.getColumnType(i);
            row_column_RPattern.addColumn(groupby_column_type);
        }
        class TypeInt8 validation_int8;
        row_column_RPattern.addColumn(&validation_int8);
    }

    Column_id_array.assign(((Operator *)lchild)->Column_id_array.begin(), ((Operator *)lchild)->Column_id_array.end());
    prev_buffer = (char *)malloc(row_column_RPattern.getRowSize());
    prev_buffer = ((Operator *)lchild)->current_buffer;
    current_buffer = (char *)malloc(row_column_RPattern.getRowSize());

    for(int i = 0; i < groupby_number; i++){
        Column *groupby_column_id = (Column *)g_catalog.getObjByName(groupby_column[i].name);
        int groupby_column_rank;
         for(int j = 0; j < Column_id_array.size(); j++){
            if(Column_id_array[j] == groupby_column_id->getOid()){
                 groupby_column_rank = j;
                 break;
            }
        }
        groupby_offset[i] = row_column_RPattern.getColumnOffset(groupby_column_rank);
        groupby_data_type[i] = groupby_column_id->getDataType();
    }

    for(int i = 0; i < groupby_compute_num; i++){
        groupby_compute_method[i] = groupby_conditions[i].aggrerate_method;
         Column *groupby_compute_id = (Column *)g_catalog.getObjByName(groupby_conditions[i].name);
        int groupby_compute_rank;
         for(int j = 0; j < Column_id_array.size(); j++){
            if(Column_id_array[j] == groupby_compute_id->getOid()){
                 groupby_compute_rank = j;
                 break;
            }
        }
        groupby_compute_offset[i] = row_column_RPattern.getColumnOffset(groupby_compute_rank);
        groupby_compute_data_type[i] = groupby_compute_id->getDataType();
    }

    int flag = 1;
    while(((Operator *)lchild)->getNext()){
        if(flag == 1){
            struct Groupby_struct first_struct;
            for(int i = 0; i < groupby_number; i++){
                groupby_data_type[i]-> copy( first_struct.given_condition[i],((Operator *)lchild)->current_buffer + groupby_offset[i]);
            }

            for(int i = 0; i < groupby_compute_num; i++){
                if(groupby_compute_method[i] == COUNT){
                    *((int64_t *)first_struct.value[i]) = 1;
                }
                else if(groupby_compute_method[i] == SUM || groupby_compute_method[i] == MAX || groupby_compute_method[i] == MIN){
                    groupby_compute_data_type[i]->copy( first_struct.value[i], ((Operator *)lchild)->current_buffer + groupby_compute_offset[i]);
                }
                else if(groupby_compute_method[i] == AVG){
                    *((int64_t *)first_struct.value[i]) = 1;
                    groupby_compute_data_type[i]->copy( first_struct.value[i] + sizeof(int64_t), ((Operator *)lchild)->current_buffer + groupby_compute_offset[i]);
                }
            }
            groupby_vector.push_back(first_struct);
            flag = 0;
        }
        else{
            int flag1 = 0;
            int flag2 = 0;
            for(int j = 0; j < groupby_vector.size(); j++){
                flag1 = 0;
                for(int k = 0; k < groupby_number; k++){
                    if(groupby_data_type[k]->cmpEQ(groupby_vector[j].given_condition[k],((Operator *)lchild)->current_buffer + groupby_offset[k])){
                        flag1++;
                    }
                }
                if(flag1 == groupby_number){
                    flag2 = 1;
                    for(int i = 0; i < groupby_compute_num; i++){
                        if(groupby_compute_method[i] == COUNT){
                            *((int64_t *)groupby_vector[j].value[i]) += 1;
                        }
                        else if(groupby_compute_method[i] == SUM){
                            Sum(groupby_compute_data_type[i], groupby_vector[j].value[i], ((Operator *)lchild)->current_buffer + groupby_compute_offset[i]);
                            //TODO: groupby_vector[j].value[i] += *(((Operator *)lchild)->current_buffer + groupby_compute_offset[i]);
                        }
                        else if(groupby_compute_method[i] == AVG){
                            *((int64_t *)groupby_vector[j].value[i]) += 1;
                            Sum(groupby_compute_data_type[i], groupby_vector[j].value[i] + sizeof(int64_t),((Operator *)lchild)->current_buffer + groupby_compute_offset[i]);
                            // TODO: groupby_vector[j].value[i] += *(current_buffer + groupby_compute_offset[i]);
                        }
                        else if(groupby_compute_method[i] == MAX){
                            if(groupby_compute_data_type[i]-> cmpGT(((Operator *)lchild)->current_buffer + groupby_compute_offset[i],groupby_vector[j].value[i])){
                                groupby_compute_data_type[i]-> copy(groupby_vector[j].value[i], ((Operator *)lchild)->current_buffer + groupby_compute_offset[i]);
                            }
                        }
                        else if(groupby_compute_method[i] == MIN){
                            if(groupby_compute_data_type[i]-> cmpLT(((Operator *)lchild)->current_buffer + groupby_compute_offset[i],groupby_vector[j].value[i])){
                                groupby_compute_data_type[i]-> copy(groupby_vector[j].value[i], ((Operator *)lchild)->current_buffer + groupby_compute_offset[i]);
                            }
                        }
                    }
                }
            }


            if(flag2 == 0){
                struct Groupby_struct temp_struct;
                for(int i = 0; i < groupby_number; i++){
                    groupby_data_type[i]-> copy( temp_struct.given_condition[i],((Operator *)lchild)->current_buffer + groupby_offset[i]);
                }
                for(int i = 0; i < groupby_compute_num; i++){
                    if(groupby_compute_method[i] == COUNT){
                        *((int64_t *)temp_struct.value[i]) = 1;
                    }
                    else if(groupby_compute_method[i] == SUM || groupby_compute_method[i] == MAX || groupby_compute_method[i] == MIN){
                        groupby_compute_data_type[i]->copy( temp_struct.value[i], ((Operator *)lchild)->current_buffer + groupby_compute_offset[i]);
                    }
                    else if(groupby_compute_method[i] == AVG){
                        *((int64_t *)temp_struct.value[i]) = 1;
                        groupby_compute_data_type[i]->copy( temp_struct.value[i] + sizeof(int64_t), ((Operator *)lchild)->current_buffer + groupby_compute_offset[i]);
                    }
                }
                groupby_vector.push_back(temp_struct);
            }
        }
    }
    count = 0;
    return true;
}

bool Groupby::getNext(){
    if(count < groupby_vector.size()){
        for(int i = 0; i < groupby_number; i++){
            groupby_data_type[i]->copy(current_buffer + groupby_offset[i], groupby_vector[count].given_condition[i]);
        }
        for(int i = 0; i < groupby_compute_num; i++){
            if(groupby_compute_method[i] != AVG){
                groupby_compute_data_type[i]->copy(current_buffer + groupby_compute_offset[i], groupby_vector[count].value[i]);
            }
            else{
                Divide(groupby_compute_data_type[i],groupby_vector[count].value[i] + sizeof(int64_t),groupby_vector[count].value[i]);
                groupby_compute_data_type[i]->copy(current_buffer + groupby_compute_offset[i],groupby_vector[count].value[i] + sizeof(int64_t));
            }
        }
        count++;
        return true;
    }
    return false;
}

bool Groupby::isEnd(){
    ((Operator *)lchild)->isEnd();
    lchild = NULL;
    rchild = NULL;
    parent = NULL;
    row_column_RPattern.reset();
    row_column_RPattern.shut();
    Column_id_array.clear();
    
    groupby_vector.clear();

    if(current_buffer != NULL){
        free(current_buffer);
    }
    return true;
}

/**
 * the compare function of qsort, use it in Orderby Operator
 * @param item1 one element in qsort array
 * @param item2 one element in qsort array
 * @retval 0 item1 == item2
 * @retval -1 item1 < item2
 * @retval 1 item1 > item2
 */ 
static int Orderby_Compare(const void * item1, const void * item2){
    
    char **buffer1 = (char **)item1;
    char **buffer2 = (char **)item2;
    
    for(int i = 0; i < orderby_cmp-> orderby_number; i++){
        if(orderby_cmp-> orderby_data_type[i]->cmpEQ((*buffer1) + orderby_cmp-> orderby_offset[i], (*buffer2) + orderby_cmp->orderby_offset[i])){
            if(i == orderby_cmp->orderby_number - 1){
                return 0;
            }
            else{
                continue;
            }
        }
        else if(orderby_cmp->orderby_data_type[i]->cmpLT((*buffer1) + orderby_cmp->orderby_offset[i], (*buffer2) + orderby_cmp->orderby_offset[i])){
            return -1;
        }
        else if(orderby_cmp->orderby_data_type[i]->cmpGT((*buffer1) + orderby_cmp->orderby_offset[i], (*buffer2) + orderby_cmp->orderby_offset[i])){
            return 1;
        }
    }
}

bool Orderby::init(int orderby_given_number, RequestColumn *orderby_given){
    orderby_number = orderby_given_number;
    for(int i = 0; i < orderby_number; i++){
        memcpy(orderby[i].name, orderby_given[i].name, 128);
        orderby[i].aggrerate_method = orderby_given[i].aggrerate_method;
    }
    int column_total_num;
    column_total_num = ((Operator *)lchild)->Column_id_array.size();
    if(row_column_RPattern.init(column_total_num + 1)){
        for(int i = 0; i < column_total_num; i++){
            BasicType *orderby_column_type = ((Operator *)lchild)->row_column_RPattern.getColumnType(i);
            row_column_RPattern.addColumn(orderby_column_type);
        }
        class TypeInt8 validation_int8;
        row_column_RPattern.addColumn(&validation_int8);
    }

    Column_id_array.assign(((Operator *)lchild)->Column_id_array.begin(),((Operator*)lchild)->Column_id_array.end());
    prev_buffer = (char *)malloc(row_column_RPattern.getRowSize());
    prev_buffer = ((Operator *)lchild)->current_buffer;
    current_buffer = (char *)malloc(row_column_RPattern.getRowSize());


    for(int i = 0; i < orderby_number; i++){
        Column *orderby_column_id = (Column *)g_catalog.getObjByName(orderby[i].name);
        int orderby_column_rank;
        for(int j = 0; j < Column_id_array.size(); j++){
            if(Column_id_array[j] == orderby_column_id->getOid()){
                orderby_column_rank = j;
                break;
            }
        }
        orderby_offset[i] = row_column_RPattern.getColumnOffset(orderby_column_rank);
        orderby_data_type[i] = orderby_column_id->getDataType();
    }

    while(((Operator *)lchild)->getNext()){
       char *temp;
       temp = (char *)malloc(row_column_RPattern.getRowSize());
       memcpy(temp,((Operator *)lchild)->current_buffer,row_column_RPattern.getRowSize());
       orderby_vector.push_back(temp);
    }

    qsort(&orderby_vector[0],orderby_vector.size(),sizeof(orderby_vector[0]),Orderby_Compare);
    count = 0;
    return true;
}

bool Orderby::getNext(){
    if(count < orderby_vector.size()){
        memcpy(current_buffer , orderby_vector[count], row_column_RPattern.getRowSize());
        count++;
        return true;
    }
    return false;
}

bool Orderby::isEnd(){
    ((Operator *)lchild)->isEnd();
    lchild = NULL;
    rchild = NULL;
    parent = NULL;
    row_column_RPattern.reset();
    row_column_RPattern.shut();
    Column_id_array.clear();

    for(int i = 0; i < orderby_vector.size(); i++){
        free(orderby_vector[i]);
    }
    orderby_vector.clear();

    if(current_buffer != NULL){
        free(current_buffer);
    }
    return true;
}

/**
 * print function.
*/
void Scan::print(int n){
    for(int i = 0; i < n; i++){
        printf("---");
    }
    printf("Scan\n");
    if(lchild) ((Operator*)lchild)->print(n+1);
    if(rchild) ((Operator*)rchild)->print(n+1);
}

void Filter::print(int n){
    for(int i = 0; i < n; i++){
        printf("---");
    }
    printf("Filter\n");
    if(lchild) ((Operator*)lchild)->print(n+1);
    if(rchild) ((Operator*)rchild)->print(n+1);
}

void Project::print(int n){
    for(int i = 0; i < n; i++){
        printf("---");
    }
    printf("Project\n");
    if(lchild) ((Operator*)lchild)->print(n+1);
    if(rchild) ((Operator*)rchild)->print(n+1);
}

void Join::print(int n){
    for(int i = 0; i < n; i++){
        printf("---");
    }
    printf("Join\n");
    if(lchild) ((Operator*)lchild)->print(n+1);
    if(rchild) ((Operator*)rchild)->print(n+1);
}

void Groupby::print(int n){
    for(int i = 0; i < n; i++){
        printf("---");
    }
    printf("Groupby\n");
    if(lchild) ((Operator*)lchild)->print(n+1);
    if(rchild) ((Operator*)rchild)->print(n+1);
}

void Orderby::print(int n){
    for(int i = 0; i < n; i++){
        printf("---");
    }
    printf("Orderby\n");
    if(lchild) ((Operator*)lchild)->print(n+1);
    if(rchild) ((Operator*)rchild)->print(n+1);
}