### 【2791】set search directories from text file 设置搜索目录从文本文件

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
                if (ii < (n_lines - 1)) a_line[strlen(a_line) - 1] = '\0';  
                UF_CALL(allocate_memory((strlen(a_line) + 1) * sizeof(char),   
                    (void **)&(*data)[ii]));  
                strcpy((*data)[ii], a_line);  
            }  
            fclose(the_file);  
        }  
        else  
            ECHO("file cannot be read %s\n", fspec);  
        return n_lines;  
    }  
    static void set_search_directories_from_text_file(char *fspec, logical subs)  
    {  
        int  
            ii,  
            n_dirs,  
            *do_subs;  
        char  
            **dirs;  
        UF_ASSEM_options_t  
            options;  
        UF_CALL(UF_ASSEM_ask_assem_options(&options));  
        options.load_options = UF_ASSEM_load_from_search_dirs;  
        UF_CALL(UF_ASSEM_set_assem_options(&options));  
        n_dirs = ask_data_from_text_file(fspec, &dirs);  
        if (n_dirs == 0) return;  
        UF_CALL(allocate_memory(n_dirs * sizeof(int), &do_subs));  
        for (ii = 0; ii < n_dirs; ii++) do_subs[ii] = subs;  
        UF_CALL(UF_ASSEM_set_search_directories(n_dirs, dirs, do_subs));  
        UF_free_string_array(n_dirs, dirs);  
        UF_free(do_subs);  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API编写的，用于从文本文件中读取搜索路径并设置NX的装配搜索路径。
>
> 主要功能包括：
>
> 1. allocate_memory函数：用于分配内存，其中nbytes指定要分配的字节数，where用于存放分配的内存地址，并返回响应码。
> 2. ask_data_from_text_file函数：用于从指定文件读取数据，其中fspec为文件路径，data用于存放读取的字符串数组。函数会统计文件行数，为data分配内存，并逐行读取文件内容到data中。
> 3. set_search_directories_from_text_file函数：用于设置NX的装配搜索路径。首先读取指定文本文件中的路径，然后调用UF_ASSEM_set_search_directories函数设置NX的装配搜索路径。
> 4. UF_ASSEM_ask_assem_options函数：用于获取当前的装配选项。
> 5. UF_ASSEM_set_assem_options函数：用于设置装配选项，这里将load_options设置为UF_ASSEM_load_from_search_dirs，表示从搜索路径加载装配。
> 6. UF_ASSEM_set_search_directories函数：用于设置NX的装配搜索路径。
> 7. UF_free_string_array函数：用于释放字符串数组内存。
> 8. UF_free函数：用于释放内存。
>
> 通过这段代码，用户可以灵活地从文本文件中设置NX的装配搜索路径，提高了NX二次开发的灵活性。
>
