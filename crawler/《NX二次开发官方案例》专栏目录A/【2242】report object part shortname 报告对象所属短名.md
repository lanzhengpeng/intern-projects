### 【2242】report object part shortname 报告对象所属短名

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
    #define ROP(X) report_object_part_shortname(#X, X)  
    static void report_object_part_shortname(char *what, tag_t object)  
    {  
        tag_t  
            owned_by;  
        UF_CALL(UF_OBJ_ask_owning_part(object, &owned_by));  
        ECHO(what);  
        ECHO(" ");  
        RPN(owned_by);  
    }

```

#### 代码解析

> 这段代码定义了两个宏RPN和ROP，用于打印NX部件的简称和NX对象的拥有部件的简称。
>
> RPN(X)宏调用了report_part_shortname函数，该函数首先调用ask_part_shortname函数获取部件的简称，然后格式化并打印出部件的简称。
>
> ask_part_shortname函数首先获取部件的全名，然后根据NX是否激活管理器，调用不同的函数获取简称。
>
> ROP(X)宏调用了report_object_part_shortname函数，该函数首先获取对象的所有者部件，然后打印对象名和所有者部件的简称。
>
> 这些宏和函数的目的是为了方便打印NX部件和对象的简称信息，以便于调试和日志记录。
>
