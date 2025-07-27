### 【1342】get list of all of name prefix 获取所有名称前缀列表

#### 代码

```cpp
     /*  The name prefix must be all in upper case 译:名称前缀必须全部大写。 */  
    static tag_t ask_next_of_name_prefix(char *prefix, tag_t part, tag_t object)  
    {  
        char  
            fullname[MAX_ENTITY_NAME_SIZE+1];  
        while ((object = UF_OBJ_cycle_all(part, object)) != NULL_TAG)  
            if ((!UF_OBJ_ask_name(object, fullname)) &&  
                (strstr(fullname, prefix) == fullname))  
                break;  
        return object;  
    }  
    static uf_list_p_t get_list_of_all_of_name_prefix(char *prefix)  
    {  
        tag_t  
            object = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        uf_list_p_t  
            object_list;  
        UF_CALL(UF_MODL_create_list(&object_list));  
        while ((object = ask_next_of_name_prefix(prefix, part, object)) != NULL_TAG)  
            UF_CALL(UF_MODL_put_list_item(object_list, object));  
        return object_list;  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API编写的，用于在NX中获取名称前缀匹配的实体列表。
>
> 主要功能包括：
>
> 1. ask_next_of_name_prefix函数：该函数接受名称前缀、组件和对象标签，用于在组件中循环查找名称以给定前缀开头的下一个对象，并返回该对象标签。
> 2. get_list_of_all_of_name_prefix函数：该函数接受名称前缀，用于获取组件中所有名称以给定前缀开头的对象列表。
> 3. 在组件中循环查找名称以给定前缀开头的对象，并将它们添加到列表中。
> 4. 如果找到匹配的对象，则返回对象标签；否则返回NULL_TAG。
> 5. 如果成功获取对象列表，则返回列表；否则返回NULL。
> 6. 名称前缀必须全部大写。
> 7. 获取显示组件中的对象。
> 8. 使用NX Open C++ API中的函数，如UF_OBJ_cycle_all、UF_OBJ_ask_name、UF_PART_ask_display_part、UF_MODL_create_list、UF_MODL_put_list_item等。
>
> 总的来说，这段代码提供了在NX中根据名称前缀获取对象列表的功能。
>
