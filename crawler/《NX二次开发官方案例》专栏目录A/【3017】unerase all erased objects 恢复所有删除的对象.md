### 【3017】unerase all erased objects 恢复所有删除的对象

#### 代码

```cpp
    static void unerase_all_erased_objects(char *view)  
    {  
        int  
            irc;  
        tag_t  
            object = NULL_TAG;  
        uf_list_p_t  
            erased_list,  
            temp;  
        UF_CALL(UF_MODL_create_list(&erased_list));  
        while (((irc = uc6457(view, 3, &object)) == 0) && (object != NULL_TAG))  
            UF_CALL(UF_MODL_put_list_item(erased_list, object));  
        for (temp = erased_list; temp->next != NULL_TAG; temp = temp->next)  
            WRITENZ(uc6403(view, temp->eid));  
        UF_CALL(UF_MODL_delete_list(&erased_list));  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API编写的NX二次开发代码，主要用于恢复NX装配体中所有被擦除的对象。
>
> 主要功能包括：
>
> 1. 使用UF_MODL_create_list函数创建一个链表erased_list，用于存储被擦除的对象。
> 2. 通过循环调用uc6457函数遍历当前视图中的所有被擦除的对象，将它们添加到erased_list链表中。
> 3. 遍历erased_list链表，使用WRITENZ宏调用uc6403函数逐个恢复链表中的被擦除对象。
> 4. 最后调用UF_MODL_delete_list函数删除erased_list链表。
>
> 总体来说，这段代码的目的是遍历并恢复当前视图中的所有被擦除的对象，实现了对象恢复的功能。代码结构清晰，调用NX Open API，符合NX二次开发规范。
>
