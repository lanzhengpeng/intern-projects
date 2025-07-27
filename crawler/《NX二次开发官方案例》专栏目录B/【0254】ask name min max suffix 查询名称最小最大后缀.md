### 【0254】ask name min max suffix 查询名称最小最大后缀

#### 代码

```cpp
    static tag_t cycle_by_name_prefix(char *prefix, tag_t part, tag_t object,  
        char *fullname)  
    {  
        int  
            ii;  
     /*  make sure the name is all in upper case 译:确保名称全部为大写。 */  
        if (object == NULL_TAG)  
            for (ii = 0; ii < strlen(prefix); ii++)  
                prefix[ii] = toupper(prefix[ii]);  
        while ((object = UF_OBJ_cycle_all(part, object)) != NULL_TAG)  
            if ((!UF_OBJ_ask_name(object, fullname)) &&  
                (strstr(fullname, prefix) == fullname))  
                break;  
        return object;  
    }  
    static void ask_name_min_max_suffix(char *prefix, int *min, int *max)  
    {  
        int  
            n;  
        tag_t  
            object = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        char  
            fmt[MAX_ENTITY_NAME_SIZE + 1],  
            fullname[MAX_ENTITY_NAME_SIZE + 1];  
        sprintf(fmt, "%s%s", prefix, "%d");  
        *min = 0;  
        *max = 0;  
        while ((object = cycle_by_name_prefix(prefix, part, object, fullname))  
            != NULL_TAG)  
        {  
            sscanf(fullname, fmt, &n);  
            if ((*min == 0) || (n < *min)) *min = n;  
            if (n > *max) *max = n;  
        }  
    }

```

#### 代码解析

> 这段NX二次开发代码主要包括两个函数：
>
> 1. cycle_by_name_prefix：该函数用于遍历NX部件树，寻找名称前缀匹配特定字符串的对象。它会遍历给定部件的所有对象，并检查每个对象的名称是否以给定前缀开头。若找到匹配的对象，则返回该对象的tag。若遍历完所有对象都未找到匹配对象，则返回NULL_TAG。
> 2. ask_name_min_max_suffix：该函数用于分析名称包含特定前缀的对象的编号范围。它会遍历名称前缀匹配的对象，解析每个对象的编号，并更新编号的最小值和最大值。函数最终返回编号的最小值和最大值。
>
> 这两个函数通常用于NX二次开发中，以便在部件树中查找和遍历对象，并获取与名称相关的信息。例如，可以用于查找和遍历所有名称以特定前缀开头的对象，并获取它们的编号范围。这些功能在自动化处理NX模型时非常有用。
>
