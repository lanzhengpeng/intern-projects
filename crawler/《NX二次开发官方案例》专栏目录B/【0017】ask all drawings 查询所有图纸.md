### 【0017】ask all drawings 查询所有图纸

#### 代码

```cpp
    /*  The methods used in this sample are obsolete - please use the function  
        UF_DRAW_ask_drawings!  
    */  
    static tag_t ask_next_drawing(tag_t part, tag_t drawing)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_drawing_type, &drawing));  
        return (drawing);  
    }  
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
    static int ask_all_drawings(tag_t part, tag_t **drawings)  
    {  
        tag_t  
            drawing = NULL_TAG;  
        uf_list_p_t  
            draw_list;  
        UF_CALL(UF_MODL_create_list(&draw_list));  
        while ((drawing = ask_next_drawing(part, drawing)) != NULL_TAG)  
            UF_CALL(UF_MODL_put_list_item(draw_list, drawing));  
        return make_an_array(&draw_list, drawings);  
    }

```

#### 代码解析

> 这段代码是一个NX Open C++二次开发示例，主要功能是从NX部件中查询所有图纸并返回它们的数组。
>
> 关键点包括：
>
> 1. 使用UF_OBJ_cycle_objs_in_part函数遍历部件中的所有图纸对象，并返回下一个图纸的tag。
> 2. 使用UF_MODL_create_list和UF_MODL_put_list_item函数创建一个图纸对象列表，并逐个添加遍历到的图纸。
> 3. 定义了一个make_an_array函数，将对象列表转换成tag数组并返回数组大小。
> 4. 定义了一个ask_all_drawings函数，调用前面定义的函数获取所有图纸，并返回它们的tag数组。
> 5. 使用了allocate_memory函数动态分配内存，用于存储图纸tag数组。
> 6. 在代码顶部有注释提示，建议使用UF_DRAW_ask_drawings函数替代示例中使用的过时方法。
> 7. 该代码主要用于演示NX二次开发中对象遍历、列表操作、内存分配等基础功能的使用。
>
> 综上所述，这段代码为NX二次开发提供了遍历查询部件中所有图纸的示例，展示了NX Open C++编程的基础用法。
>
