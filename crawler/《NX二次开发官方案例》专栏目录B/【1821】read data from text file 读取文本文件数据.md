### 【1821】read data from text file 读取文本文件数据

#### 代码

```cpp
    static int allocate_memory(unsigned int nbytes, void **where)  
    {  
        int  
            resp;  
        *where = UF_allocate_memory(nbytes, &resp);  
        return resp;  
    }  
    static int read_data_from_text_file(char *fspec, char ***data)  
    {  
        int  
            ii = 0,  
            n_lines = 0;  
        char  
            a_line[MAX_LINE_SIZE+1];  
        FILE  
            *the_file;  
        if ((the_file = fopen(fspec, "r")) != NULL)  
        {  
            while ((fgets(a_line, MAX_LINE_SIZE, the_file)) != NULL) n_lines++;  
            rewind(the_file);  
            UF_CALL(allocate_memory(n_lines * sizeof(char *), (void **)data));  
            for (ii = 0; ii < n_lines; ii++)  
            {  
                fgets(a_line, MAX_LINE_SIZE, the_file);  
                a_line[strlen(a_line) - 1] = '\0'; /* take off carriage return 译:去掉回车符 */  
                UF_CALL(allocate_memory(strlen(a_line) + 1, (void **)&(*data)[ii]));  
                strcpy((*data)[ii], a_line);  
            }  
            fclose(the_file);  
        }  
        return n_lines;  
    }

```

#### 代码解析

> 这段代码用于从文本文件中读取数据并存储到内存中，主要包含以下几个关键点：
>
> 1. allocate_memory函数：该函数用于动态分配指定大小的内存，并返回指向这块内存的指针。
> 2. read_data_from_text_file函数：该函数用于从指定路径的文本文件中读取数据。首先，它会统计文件的行数，然后为每一行动态分配内存并存储该行的数据。最终，函数返回文件中的总行数。
> 3. 关键变量说明：fspec：文件路径data：指向char**的指针，用于存储每行的数据a_line：用于临时存储从文件中读取的一行数据n_lines：文件中的总行数
> 4. fspec：文件路径
> 5. data：指向char**的指针，用于存储每行的数据
> 6. a_line：用于临时存储从文件中读取的一行数据
> 7. n_lines：文件中的总行数
> 8. 函数流程：打开文件并统计行数为每一行动态分配内存并存储数据关闭文件
> 9. 打开文件并统计行数
> 10. 为每一行动态分配内存并存储数据
> 11. 关闭文件
> 12. 注意事项：检查文件是否成功打开为每行动态分配内存前检查返回值使用rewind函数重置文件指针去掉每行末尾的回车符
> 13. 检查文件是否成功打开
> 14. 为每行动态分配内存前检查返回值
> 15. 使用rewind函数重置文件指针
> 16. 去掉每行末尾的回车符
>
> 这段代码实现了文本文件数据读取和内存存储的功能。
>
