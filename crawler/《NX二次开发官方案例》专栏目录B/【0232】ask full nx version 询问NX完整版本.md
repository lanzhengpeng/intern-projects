### 【0232】ask full nx version 询问NX完整版本

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

> 这段NX二次开发代码主要实现了以下功能：
>
> 1. 内存分配函数：allocate_memory用于在NX中动态分配指定大小的内存，并将分配的内存地址存储在传入的指针参数where中。
> 2. 去除字符串多余小数点函数：strip_extra_decimal_points用于去除传入字符串s中的多余小数点，只保留第一个小数点。
> 3. 获取NX版本号函数：ask_full_nx_version用于获取NX的完整版本号，并将其存储在传入的字符串指针参数version中。此函数还会去除版本号中的多余小数点，并返回版本号的小数部分，以便于进行版本比较。
>
> 整体而言，这段代码定义了在NX中动态分配内存、去除字符串多余小数点以及获取NX版本号等辅助功能，为NX的二次开发提供了基础支持。
>
