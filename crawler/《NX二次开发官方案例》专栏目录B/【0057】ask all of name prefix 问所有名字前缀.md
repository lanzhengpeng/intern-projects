### 【0057】ask all of name prefix 问所有名字前缀

#### 代码

```cpp
    static int allocate_memory(unsigned int nbytes, void **where)  
    {  
        int  
            resp;  
        *where = UF_allocate_memory(nbytes, &resp);  
        return resp;  
    }  
    static int make_an_array(uf_list_p_t *object_list, tag_t **objects)  
    {  
        int  
            ii,  
            n;  
        uf_list_p_t  
            temp;  
        UF_CALL(UF_MODL_ask_list_count(*object_list, &n));  
        UF_CALL(allocate_memory(n * sizeof(tag_t), (void **)objects));  
        for (ii = 0, temp = *object_list; ii < n; temp = temp->next, ii++)  
            (*objects)[ii] = temp->eid;  
        UF_CALL(UF_MODL_delete_list(object_list));  
        return n;  
    }  
    static tag_t ask_next_of_name_prefix(char *prefix, tag_t part, tag_t object)  
    {  
        int  
            ii;  
        char  
            fullname[MAX_ENTITY_NAME_SIZE+1];  
     /*  make sure the name is all in upper case 译:确保名称全部使用大写字母。 */  
        if (object == NULL_TAG)  
            for (ii = 0; ii < strlen(prefix); ii++)  
                prefix[ii] = toupper(prefix[ii]);  
        while ((object = UF_OBJ_cycle_all(part, object)) != NULL_TAG)  
            if ((!UF_OBJ_ask_name(object, fullname)) &&  
                (strstr(fullname, prefix) == fullname))  
                break;  
        return object;  
    }  
    static int ask_all_of_name_prefix(char *prefix, tag_t **objects)  
    {  
        tag_t  
            object = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        uf_list_p_t  
            object_list;  
        UF_CALL(UF_MODL_create_list(&object_list));  
        while ((object = ask_next_of_name_prefix(prefix, part, object)) != NULL_TAG)  
            UF_CALL(UF_MODL_put_list_item(object_list, object));  
        return make_an_array(&object_list, objects);  
    }

```

#### 代码解析

> 这段NX二次开发代码主要实现了以下功能：
>
> 1. 内存分配：allocate_memory函数用于从NX内存池中分配指定大小的内存，并将分配的内存指针存储在where参数指向的指针中。
> 2. 创建数组：make_an_array函数用于创建一个tag数组，并从object_list中读取所有对象，并将它们的eid填充到数组中。然后删除object_list。
> 3. 查询对象：ask_next_of_name_prefix函数用于查询具有指定前缀名称的对象，返回找到的第一个对象，如果没有则返回NULL_TAG。
> 4. 查询所有对象：ask_all_of_name_prefix函数用于查询具有指定前缀名称的所有对象，将这些对象存储在一个list中，然后调用make_an_array将list中的对象转换为一个tag数组，并返回数组长度。
>
> 总体来说，这段代码提供了在NX中查询和创建对象数组的基本框架，可用于各种对象过滤和处理的场景。
>
