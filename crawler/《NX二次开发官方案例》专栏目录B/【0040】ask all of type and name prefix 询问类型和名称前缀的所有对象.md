### 【0040】ask all of type and name prefix 询问类型和名称前缀的所有对象

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
    static int ask_all_of_type_and_name_prefix(tag_t part, int type, char *prefix,  
        tag_t **objects)  
    {  
        int  
            ii;  
        tag_t  
            object = NULL_TAG;  
        uf_list_p_t  
            object_list;  
        char  
            fullname[MAX_ENTITY_NAME_SIZE+1];  
    /*  make sure the name is all in upper case 译:确保名称全部为大写字母。 */  
        for (ii = 0; ii < strlen(prefix); ii++) prefix[ii] = toupper(prefix[ii]);  
        UF_CALL(UF_MODL_create_list(&object_list));  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, type, &object)) &&  
            (object != NULL_TAG))  
        {  
            if ((!UF_OBJ_ask_name(object, fullname)) &&  
                (strstr(fullname, prefix) == fullname))  
                UF_CALL(UF_MODL_put_list_item(object_list, object));  
        }  
        return make_an_array(&object_list, objects);  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API编写的，其主要功能是根据类型和名称前缀查询NX部件中的对象，并将查询结果存储在一个数组中。具体来说，代码主要包含以下几个部分：
>
> 1. allocate_memory()函数：用于申请内存空间，将内存地址存储在where指针指向的地址中。
> 2. make_an_array()函数：将对象列表转换为一个tag_t数组，释放对象列表。
> 3. ask_all_of_type_and_name_prefix()函数：该函数是主函数，首先创建一个空的对象列表，然后遍历指定类型和名称前缀的对象，将满足条件的对象添加到对象列表中。最后，调用make_an_array()函数将对象列表转换为数组并返回数组元素个数。
> 4. 在主函数中，首先将名称前缀转换为大写，然后创建一个空的对象列表。接着，遍历部件中的对象，如果对象的名称以指定前缀开头，则将该对象添加到对象列表中。最后，调用make_an_array()函数将对象列表转换为数组，并返回数组元素个数。
>
> 总体来说，这段代码的主要功能是根据类型和名称前缀查询部件中的对象，并将查询结果存储在一个tag_t数组中。
>
