### 【2904】sort tag array by object ID 根据对象ID对标签数组进行排序

#### 代码

```cpp
    static int ask_object_id(tag_t object)  
    {  
        unsigned int  
            ID,  
            ver;  
        char  
            *file_data,  
            *handle;  
        handle = UF_TAG_ask_handle_of_tag(object);  
        UF_CALL(UF_TAG_decompose_handle(handle, &file_data, &ID, &ver));  
        UF_free(handle);  
        UF_free(file_data);  
        return ID;  
    }  
    // qsort int comparison function  
    // integer comparison: returns negative if b > a and positive if a > b  
    int int_cmp(const void *a, const void *b)  
    {  
        const int *ia = (const int *)a;  
        const int *ib = (const int *)b;  
        return *ia  - *ib;  
    }  
    struct entity_info_s  
    {  
        int ID;  
        tag_t entity;  
    };  
    typedef struct entity_info_s entity_info_t, *entity_info_p_t;  
    static void sort_by_ID(tag_t *objects, int n_objects)  
    {  
        int  
            last = 0;  
        logical  
            ok = TRUE;  
        entity_info_t  
            *object_data;  
        UF_CALL(allocate_memory(n_objects * sizeof(entity_info_t),  
            (void **)&object_data));  
        for (int ii = 0; ii < n_objects; ii++)  
        {  
            object_data[ii].ID = ask_object_id(objects[ii]);  
            object_data[ii].entity = objects[ii];  
            if (object_data[ii].ID > last)  
                last = object_data[ii].ID;  
            else  
                ok = FALSE;  
        }  
        if (!ok)  
        {  
            qsort(object_data, n_objects, sizeof(entity_info_t), int_cmp);  
            for (int ii = 0; ii < n_objects; ii++)  
            {  
                objects[ii] = object_data[ii].entity;  
            }  
        }  
        UF_free((void *)object_data);  
    }

```

#### 代码解析

> 这段NX Open C++代码的功能是按照对象的ID进行排序。
>
> 主要步骤包括：
>
> 1. 定义了一个结构体entity_info_t，用于存储对象的ID和实体信息。
> 2. 实现了一个ask_object_id函数，用于获取NX对象对应的ID。
> 3. 实现了一个int类型的比较函数int_cmp，用于后续的排序。
> 4. 实现了sort_by_ID函数，用于对NX对象数组按照ID进行排序。具体步骤包括：遍历对象数组，获取每个对象的ID，并检查是否已排序如果未排序，使用qsort函数和int_cmp比较函数进行排序重新赋值排序后的对象数组
> 5. 遍历对象数组，获取每个对象的ID，并检查是否已排序
> 6. 如果未排序，使用qsort函数和int_cmp比较函数进行排序
> 7. 重新赋值排序后的对象数组
> 8. 最后释放了分配的内存。
>
> 通过这段代码，可以实现对NX对象数组按照ID进行排序，以便进行后续处理。
>
