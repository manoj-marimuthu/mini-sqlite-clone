#include <db_executor.h>
#include <db_error.h>
#include <db_parser.h>
#include <stdio.h>
#include <stdlib.h>
#include <db_memory.h>
#include <string.h>

void print_line_break(int col_count){
    int width = col_count * 33 + 1;
    char line_break[width + 2];
    int j = 0;
    while(j < width){
        if(j % 33 == 0){
            line_break[j] = '+';
        }else{
            line_break[j] = '-';
        }
        j++;
    }
    line_break[j] = '\n';
    line_break[j+1] = '\0';
    printf("%s",line_break);
}
Column* createColumn(ColumnType type){
    DbMemNode* colObj = createMemNode(sizeof(Column));
    Column* col = colObj->ptr;
    col->col_size = 0;
    col->next = NULL;
    col->type = type;
    return col;
}

Row* createRow(ColumnType type){
    DbMemNode* rowObj = createMemNode(sizeof(Row));
    Row* row = rowObj->ptr;
    row->type = type;
    row->next = NULL;
    return row;
}

void execute(astNode* node){
    if(node == NULL){
        log_error("Undefined error in executor",COMPILE_TIME_ERROR);
    }
    size_t column_struct_size = sizeof(Column);
    size_t int_size = sizeof(int);
    size_t row_struct_size = sizeof(Row);
    switch(node->type){
        case AST_CREATE:{
                char file_name[strlen(node->data.strData) + 4 + 1];
                strcpy(file_name,node->data.strData);
                strcat(file_name,".tbl");
                FILE* table_file = fopen(file_name,"wb");
                if(table_file  == NULL) log_error("Table creation failed",COMPILE_TIME_ERROR);
                astNode* cur_column = node->child; //
                fwrite(&node->col_count,int_size,1,table_file);
                while(cur_column){
                    if(cur_column->type == AST_TYPE_TEXT){
                        Column* col = createColumn(TEXT_COL);
                        col->col_size = 32;
                        strcpy(col->col_name,cur_column->data.strData);
                        fwrite(col,column_struct_size,1,table_file);
                    }else if(cur_column->type == AST_TYPE_INT){
                        Column* col = createColumn(INT_COL);
                        col->col_size = int_size;
                        strcpy(col->col_name,cur_column->data.strData);
                        fwrite(col,column_struct_size,1,table_file);
                    }else{
                        log_error("Unknown type found by executor for CREATE",RUNTIME_ERROR);
                    }   
                    cur_column = cur_column->next;
                }
                fclose(table_file);
                break;
            }
        case AST_INSERT:{
            char file_name[strlen(node->data.strData) + 4 + 1];
            strcpy(file_name,node->data.strData);
            strcat(file_name,".tbl");
            FILE* table_file = fopen(file_name,"rb+");
            if(table_file == NULL){
                log_error("Table not found",RUNTIME_ERROR);
            }
            astNode* cur_val = node->child;
            // read schema
            Column* cur_col = createColumn(TEXT_COL);
            Row* cur_row = NULL;
            Row* tail = NULL;
            int col_count;
            fread(&col_count,int_size,1,table_file);
            for(int i =0;i < col_count;i++){
                fread(cur_col,column_struct_size,1,table_file);
                if(cur_val == NULL){
                    log_error("Too few values to insert into the table",RUNTIME_ERROR);
                    return;
                }
                if((cur_col->type == TEXT_COL && cur_val->type == AST_STRING) || (cur_col->type == INT_COL && cur_val->type == AST_NUMBER)){
                    Row* row = createRow(cur_col->type);
                    switch (cur_col->type)
                    {
                        case INT_COL:
                            row->data.numData = cur_val->data.numData; 
                            break;
                        case TEXT_COL:
                            strcpy(row->data.strData,cur_val->data.strData);
                            break;
                        default:
                            log_error("Unknown type found for column by executor",RUNTIME_ERROR);
                    }
                    if(cur_row == NULL){
                        cur_row = row;
                        tail = row;
                    }else{
                        tail->next = row;
                        tail = row;
                    }
                    cur_val = cur_val->next;
                }else{
                    log_error("Type mismatch during INSERT",RUNTIME_ERROR);
                }
            }
            // type validation is done, write the row
            fseek(table_file, 0, SEEK_END);
            while(cur_row){
                fwrite(cur_row,row_struct_size,1,table_file);
                cur_row = cur_row->next;
            }
            fclose(table_file);
            break;
        }
        case AST_SELECT:{
            char table_name[strlen(node->data.strData) + 5];
            strcpy(table_name,node->data.strData);
            strcat(table_name,".tbl");
            FILE* table_file = fopen(table_name,"rb");
            if(table_file == NULL) {
                printf("Table name =%s \n",table_name);
                log_error("Table not found",RUNTIME_ERROR);
            }
            int col_count;
            fread(&col_count,int_size,1,table_file);
            Column col;
            
            for(int i =0;i < col_count;i++){
                fread(&col,column_struct_size,1,table_file);
                printf("|%-32s",col.col_name);
                if(i == col_count - 1) printf("|\n");
            }
            print_line_break(col_count);
            Row row;
            while(1){
                for(int i =0;i < col_count;i++){
                    if(fread(&row,row_struct_size,1,table_file) == 1){
                       if(row.type == TEXT_COL){
                            printf("|%-32s",row.data.strData);
                       }else{
                            printf("|%-32d",row.data.numData);
                       }
                       if(i == col_count - 1) printf("|\n");
                    }else{
                        print_line_break(col_count);
                        return;
                    }
                }
            }
            fclose(table_file);
            break;
        }
        case AST_DESCRIBE:{
            char table_name[strlen(node->data.strData) + 5];
            strcpy(table_name,node->data.strData);
            strcat(table_name,".tbl");
            FILE* table_file = fopen(table_name,"rb");
            if(table_file == NULL) {
                printf("Table name =%s \n",table_name);
                log_error("Table not found",RUNTIME_ERROR);
            }
            int col_count = 0;
            fread(&col_count,int_size,1,table_file);
            print_line_break(3);
            Column col;
            printf("|%-32s","Column name");
            printf("|%-32s","Type");
            printf("|%-32s|\n","Size");
            print_line_break(3);
            for(int i = 0;i < col_count;i++){
                fread(&col,column_struct_size,1,table_file);
                printf("|%-32s",col.col_name);
                printf("|%-32s",col.type == TEXT_COL ? "TEXT" : "INT");
                printf("|%-32d|\n",col.col_size);
            }
            print_line_break(3);
            break;
        }
        default:
            log_error("Unknown statement type found by executor",RUNTIME_ERROR);
            break;
    }
}