### 【3117】get next unique name of prefix 根据前缀获取下一个唯一名称

#### 代码

```cpp
    static int get_next_unique_name_of_prefix(char *prefix, char *name)  
    {  
        int  
            ii;  
        tag_t  
            object = NULL_TAG;  
     /*  make sure the prefix is all in upper case 译:确保前缀全部为大写 */  
        for (ii = 0; ii < strlen(prefix); ii++)  
            prefix[ii] = toupper(prefix[ii]);  
        ii = 0;  
        do  
        {  
            ii++;  
            sprintf(name, "%s%d", prefix, ii);  
            object = NULL_TAG;  
            UF_CALL(UF_OBJ_cycle_by_name(name, &object));  
        } while (object != NULL_TAG);  
        return ii;  
    }

```

#### 代码解析

> 这段代码实现了在NX CAD系统中获取唯一名称的功能。主要功能包括：
>
> 1. 将传入的前缀字符串全部转换为大写。
> 2. 循环生成名称，格式为前缀+序号，序号从1开始递增。
> 3. 在NX系统中查询生成的名称是否已存在，如果存在则继续递增序号，直到查询不到为止。
> 4. 返回找到的唯一序号。
>
> 该函数的作用是给定一个前缀，生成一个唯一的名称，适用于在NX中创建新对象时需要保证名称唯一性的场景。
>
