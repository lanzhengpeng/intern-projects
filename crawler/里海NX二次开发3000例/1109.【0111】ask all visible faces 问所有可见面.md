### 【0111】ask all visible faces 问所有可见面

#### 代码

```cpp
    static tag_t ask_next_visible_face(tag_t face)  
    {  
        int  
            subtype,  
            type;  
        while (!UF_CALL(UF_VIEW_cycle_objects(NULL_TAG, UF_VIEW_VISIBLE_OBJECTS,  
                &face)) && (face != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(face, &type, &subtype));  
            if ((type == UF_solid_type) && (subtype == UF_solid_face_subtype))  
                return face;  
        }  
        return NULL_TAG;  
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
    static int ask_all_visible_faces(tag_t **faces)  
    {  
        tag_t  
            face = NULL_TAG;  
        uf_list_p_t  
            face_list;  
        UF_CALL(UF_MODL_create_list(&face_list));  
        while ((face = ask_next_visible_face(face)) != NULL_TAG)  
            UF_CALL(UF_MODL_put_list_item(face_list, face));  
        return (make_an_array(&face_list, faces));  
    }

```

#### 代码解析

> 这段NX Open API代码主要实现了以下功能：
>
> 1. ask_next_visible_face函数：用于遍历当前工作视图中所有可见的实体，并返回下一个可见的FACE实体。通过调用UF_VIEW_cycle_objects函数来循环获取实体，并检查其类型和子类型，如果为FACE，则返回该实体的tag。
> 2. allocate_memory函数：用于分配指定大小的内存，并返回指向该内存的指针。
> 3. make_an_array函数：将一个实体列表转换为tag数组，并释放列表。通过调用UF_MODL_ask_list_count获取列表中实体的数量，然后分配相应大小的内存来创建tag数组，并将列表中的实体依次放入数组中，最后释放列表。
> 4. ask_all_visible_faces函数：用于获取当前工作视图中所有可见的FACE实体，并返回这些FACE的tag数组。通过调用UF_MODL_create_list创建一个空列表，然后调用ask_next_visible_face函数来遍历所有可见实体，将FACE实体放入列表中，最后调用make_an_array函数将列表转换为tag数组并返回。
>
> 综上所述，这段代码主要用于获取NX模型中当前工作视图中所有可见的FACE实体的tag数组。
>
