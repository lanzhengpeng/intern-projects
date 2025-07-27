### 【0180】ask data from text file 从文本文件中请求数据

#### 代码

```cpp
    static int allocate_memory(unsigned int nbytes, void **where)  
    {  
        int  
            resp;  
        *where = UF_allocate_memory(nbytes, &resp);  
        return resp;  
    }  
    static int ask_data_from_text_file(char *fspec, char ***data)  
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
            /*  strip off the carriage return 译:剥离回车换行符 */  
                a_line[strlen(a_line) - 1] = '\0';  
                UF_CALL(allocate_memory((unsigned int)(strlen(a_line) + 1),  
                    (void **)&(*data)[ii]));  
                strcpy((*data)[ii], a_line);  
            }  
            fclose(the_file);  
        }  
        return n_lines;  
    }

```

#### 代码解析

> 这段代码是用于NX（Unigraphics）的二次开发，主要包含两个静态函数：
>
> 1. allocate_memory函数：该函数用于从NX的内存管理器分配指定大小的内存块，并将内存地址存储在提供的指针中。
> 2. ask_data_from_text_file函数：该函数用于从指定的文本文件中读取数据，并按行存储在动态分配的字符数组中。该函数首先统计文件中的行数，然后分配足够的内存来存储每一行的指针。接着逐行读取文件，去掉每行末尾的回车换行符，并动态分配内存来存储每行文本，最后将每行的文本复制到对应位置的指针中。
>
> 该代码的关键点包括：
>
> 综上所述，这段代码实现了从文本文件读取数据并按行存储的功能，为后续的NX二次开发提供了基础的数据处理能力。
>
