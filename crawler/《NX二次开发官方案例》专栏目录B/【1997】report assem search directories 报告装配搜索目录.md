### 【1997】report assem search directories 报告装配搜索目录

#### 代码

```cpp
    static void report_assem_search_directories(void)  
    {  
        char  
            **dir_list;  
        int  
            ii,  
            num_dir = 0;  
        logical  
            *subs;  
        UF_CALL(UF_ASSEM_ask_search_directories(&num_dir, &dir_list, &subs));  
        for (ii = 0 ; ii < num_dir; ii ++)  
        {  
            ECHO("%s", dir_list[ii]);  
            if (subs[ii]) ECHO("...");  
            ECHO("\n");  
        }  
        UF_free_string_array(num_dir, dir_list);  
        UF_free(subs);  
    }

```

#### 代码解析

> 这段代码实现了在NX CAD软件中检索和打印当前装配体搜索路径目录的功能。代码主要功能如下：
>
> 1. 定义一个静态函数report_assem_search_directories，用于报告当前装配体的搜索路径目录。
> 2. 在函数内部，首先调用UF_ASSEM_ask_search_directories函数，获取装配体的搜索路径目录列表和是否递归搜索标志。
> 3. 遍历获取的目录列表，并使用ECHO宏打印每个目录的路径。如果目录需要递归搜索，则后跟省略号。
> 4. 最后，释放由UF函数分配的目录列表和递归标志数组内存。
>
> 通过调用这个函数，可以方便地获取当前装配体所使用的搜索路径目录列表，并判断是否需要递归搜索这些目录，这对于NX二次开发中处理装配体文件路径很有帮助。
>
