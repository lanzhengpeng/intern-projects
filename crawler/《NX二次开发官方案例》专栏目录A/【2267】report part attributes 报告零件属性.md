### 【2267】report part attributes 报告零件属性

#### 代码

```cpp
    static void report_part_attributes(tag_t part)  
    {  
        int  
            ii,  
            n_attrs;  
        UF_ATTR_part_attr_t  
            *attrs;  
        char  
            *display_name,  
            msg[UF_UI_MAX_STRING_LEN+1],  
            pspec[MAX_FSPEC_SIZE+1];  
        UF_PART_ask_part_name(part, pspec);  
        UF_CALL(UF_PART_file_name_for_display(pspec, &display_name));  
        UF_CALL(UF_ATTR_ask_part_attrs(part, &n_attrs, &attrs));  
        sprintf(msg, "%s has %d attributes:\n", display_name, n_attrs);  
        ECHO(msg);  
        for (ii = 0; ii < n_attrs; ii++)  
        {  
            sprintf(msg, "  %d.  %s = %s\n", ii+1, attrs[ii].title,  
                attrs[ii].string_value);  
            ECHO(msg);  
        }  
        UF_free(display_name);  
        if (n_attrs > 0) UF_free(attrs);  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API编写的，主要功能是打印出指定部件的所有属性信息。
>
> 主要步骤包括：
>
> 1. 使用UF_PART_ask_part_name获取部件名称，并使用UF_PART_file_name_for_display获取部件的显示名称。
> 2. 调用UF_ATTR_ask_part_attrs获取部件的所有属性，包括属性数量和属性结构体数组。
> 3. 遍历属性数组，使用sprintf格式化输出每个属性的标题和字符串值。
> 4. 最后，释放显示名称和属性数组的内存。
> 5. 使用ECHO宏打印出格式化后的信息。
>
> 该代码通过NX Open C++ API获取并打印出了部件的属性信息，展示了NX二次开发中常用的API调用和内存管理方法。
>
