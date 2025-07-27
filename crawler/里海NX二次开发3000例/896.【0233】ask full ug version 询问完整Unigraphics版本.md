### 【0233】ask full ug version 询问完整Unigraphics版本

#### 代码

```cpp
    static int allocate_memory(unsigned int nbytes, void **where)  
    {  
        int  
            resp;  
        *where = UF_allocate_memory(nbytes, &resp);  
        return resp;  
    }  
    static void strip_extra_decimal_points(char *s)  
    {  
        unsigned int  
            dots = 0,  
            ii,  
            jj;  
        for (ii = 0; ii < strlen(s); ii++)  
        {  
            if (s[ii] == '.')  
            {  
                dots++;  
                if (dots > 1)  
                    for (jj = ii; jj < strlen(s); jj++)  
                        s[jj] = s[jj+1];  
            }  
        }  
    }  
    // This returns a double so that you can use > or < for comparisons  
    static double ask_full_nx_version(char **version)  
    {  
        double  
            rel_no;  
        UF_system_info_t  
            info;  
        UF_CALL(UF_ask_system_info(&info));  
        UF_CALL(allocate_memory(  
            (unsigned int)(sizeof(char)*(strlen(info.program_name)+1)),  
            (void **)version));  
        strcpy(*version, info.program_name);  
        strip_extra_decimal_points(info.program_name);  
        rel_no = atof(strrchr(info.program_name, 'X')+1);  
        UF_CALL(UF_free_system_info(&info));  
        return rel_no;  
    }

```

#### 代码解析

> 这段NX Open C++代码主要用于获取NX软件的版本号，并对其进行处理。下面是对代码功能的详细介绍：
>
> 1. 内存分配：allocate_memory函数用于在NX内存中分配指定大小的内存，并将内存指针返回给调用者。它通过调用UF库中的UF_allocate_memory函数来实现内存分配。
> 2. 处理版本号：strip_extra_decimal_points函数用于删除版本号字符串中的额外小数点，只保留第一个小数点。这可以确保版本号符合预期的格式。
> 3. 获取版本号：ask_full_nx_version函数用于获取NX的完整版本号，并将其转换为浮点数返回。它首先通过调用UF_ask_system_info获取NX的系统信息，然后从中提取程序名称（即版本号）。接着，它去除版本号中的额外小数点，并使用atof将版本号转换为浮点数。最后，释放系统信息占用的内存并返回版本号浮点数。
>
> 总的来说，这段代码的主要作用是获取NX软件的版本号，并将其转换为浮点数，以便进行版本比较。它通过调用UF库中的函数来实现内存分配和系统信息获取。
>
