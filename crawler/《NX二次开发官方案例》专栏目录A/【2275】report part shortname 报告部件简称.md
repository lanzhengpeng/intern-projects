### 【2275】report part shortname 报告部件简称

#### 代码

```cpp
    #define RPN(X) report_part_shortname(#X, X)  
    static void ask_part_shortname(tag_t part, char *shortname)  
    {  
        logical  
            nxman;  
        char  
            partname[MAX_FSPEC_SIZE+1];  
        UF_CALL(UF_PART_ask_part_name(part, partname));  
        UF_CALL(UF_is_ugmanager_active(&nxman));  
        if (nxman)  
            UF_CALL(UF_PART_name_for_display(partname, shortname));  
        else  
            UF_CALL(uc4574(partname, 2, shortname));  
    }  
    static void report_part_shortname(char *what, tag_t part)  
    {  
        char  
            msg[MAX_FSPEC_SIZE+1],  
            shortname[MAX_FSPEC_SIZE+1];  
        ask_part_shortname(part, shortname);  
        sprintf(msg, "%s = %s\n", what, shortname);  
        ECHO(msg);  
    }

```

#### 代码解析

> 这段代码是NX Open API编写的，用于获取NX部件的短名称并打印出来。
>
> 主要包含以下几个部分：
>
> 1. RPN 宏：用于获取部件短名称并打印。
> 2. ask_part_shortname 函数：用于获取指定部件的短名称。
> 3. report_part_shortname 函数：用于打印部件的短名称。
>
> 其中，RPN 宏定义了获取并打印部件短名称的快捷方式。ask_part_shortname 函数通过NX API获取部件的名称，并判断NX Manager是否激活，若激活则调用UF_PART_name_for_display获取短名称，否则调用自定义函数uc4574获取短名称。report_part_shortname 函数用于格式化短名称并打印。
>
> 通过这段代码，可以方便地获取NX部件的短名称，并在二次开发中进行打印输出。
>
