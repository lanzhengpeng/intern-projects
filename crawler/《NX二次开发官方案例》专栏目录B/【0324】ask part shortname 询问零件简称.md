### 【0324】ask part shortname 询问零件简称

#### 代码

```cpp
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

```

#### 代码解析

> 这段NX二次开发代码的功能是获取NX中零件的简称。以下是该代码的主要功能：
>
> 1. 定义了ask_part_shortname函数，用于获取指定零件的简称。
> 2. 在函数内部，首先调用UF_PART_ask_part_name函数获取零件的全名，并存储在partname数组中。
> 3. 然后调用UF_is_ugmanager_active函数判断UG管理员是否激活。
> 4. 如果UG管理员已激活，调用UF_PART_name_for_display函数获取零件的显示名称，即简称，并存储在shortname参数指向的数组中。
> 5. 如果UG管理员未激活，调用自定义函数uc4574，它将截取partname数组的前两个字符作为简称，并存储在shortname参数指向的数组中。
> 6. 最后，函数通过shortname参数返回零件的简称。
> 7. 该函数可以被其他代码调用，以获取指定零件的简称。
>
> 总的来说，这段代码通过NX的UF函数获取零件的全名，并根据管理员设置是否激活来决定获取零件的显示名称还是简名称作为简称。
>
