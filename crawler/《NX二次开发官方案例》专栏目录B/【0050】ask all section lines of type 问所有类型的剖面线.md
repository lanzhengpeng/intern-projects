### 【0050】ask all section lines of type 问所有类型的剖面线

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
    static int ask_all_section_lines_of_type(tag_t part,  
        UF_DRAW_sxline_type_t type, tag_t **objects)  
    {  
        UF_DRAW_sxline_type_t  
            this_type;  
        tag_t  
            object = NULL_TAG;  
        uf_list_p_t  
            object_list;  
        UF_CALL(UF_MODL_create_list(&object_list));  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_section_line_type,  
            &object)) && (object != NULL_TAG))  
        {  
            UF_CALL(UF_DRAW_ask_sxline_type(object, &this_type));  
            if (this_type == type)  
                (UF_MODL_put_list_item(object_list, object));  
        }  
        return (make_an_array(&object_list, objects));  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API编写的NX二次开发代码，其主要功能是遍历指定部件中的所有截面线，并筛选出指定类型的截面线，然后将这些截面线的标签存储在一个数组中。
>
> 具体来说，代码的主要逻辑如下：
>
> 1. allocate_memory函数用于分配指定大小的内存，返回指向该内存的指针。
> 2. make_an_array函数接收一个UF_MODL对象列表和一个指向tag_t数组指针，将列表中的对象标签存入数组，并释放列表。函数返回数组中对象的数量。
> 3. ask_all_section_lines_of_type函数是主函数，接收部件的tag、要筛选的截面线类型，以及指向tag_t数组指针。首先创建一个UF_MODL对象列表，然后遍历部件中的所有截面线，如果类型匹配，则将对象放入列表中。最后调用make_an_array函数，将列表中的对象标签存入数组并返回数组大小。
> 4. 函数使用了UF_OBJ和UF_DRAW的相关函数来获取部件中的截面线对象，检查类型，并存储到数组中。
> 5. 函数使用UF_MODL的列表数据结构来临时存储匹配的截面线对象。
> 6. UF_CALL宏用于简化函数调用，并处理返回码。
> 7. 整个代码的目的是为了获取并存储指定类型的截面线对象标签数组，以便后续处理或使用。
>
