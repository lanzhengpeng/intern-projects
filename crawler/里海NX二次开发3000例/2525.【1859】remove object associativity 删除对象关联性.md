### 【1859】remove object associativity 删除对象关联性

#### 代码

```cpp
    static void remove_object_associativity(tag_t object)  
    {  
        uf_list_p_t  
            list;  
        UF_CALL(UF_MODL_create_list(&list));  
        UF_CALL(UF_MODL_put_list_item(list, object));  
        UF_CALL(UF_MODL_delete_object_parms(list));  
        UF_CALL(UF_MODL_delete_list(&list));  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API编写的，主要功能是移除NX对象之间的关联性。具体来说，代码的作用是：
>
> 1. 创建一个对象列表list。
> 2. 将要移除关联性的对象object添加到list中。
> 3. 调用UF_MODL_delete_object_parms(list)函数，移除list中对象之间的关联性。
> 4. 删除list。
>
> 通过这段代码，可以方便地移除一个或多个NX对象之间的关联性，实现解耦操作。该函数适用于需要移除对象之间关联性的场景，例如在编辑模型时，可以避免误操作导致对象关联性被破坏。
>
