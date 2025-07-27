### 【2466】roll back and delete undo mark 回滚并删除撤销标记

#### 代码

```cpp
    static void roll_back_and_delete_undo_mark(UF_UNDO_mark_id_t mark_id)  
    {  
        UF_CALL(UF_UNDO_undo_to_mark(mark_id, NULL));  
        UF_CALL(UF_UNDO_delete_mark(mark_id, NULL));  
    }

```

#### 代码解析

> 根据代码，这段NX二次开发代码实现了撤销到指定标记并删除该标记的功能。具体来说：
>
> 1. static void roll_back_and_delete_undo_mark(UF_UNDO_mark_id_t mark_id)定义了一个静态函数，用于撤销到指定标记并删除该标记。
> 2. UF_CALL(UF_UNDO_undo_to_mark(mark_id, NULL));调用了UF_UNDO_undo_to_mark函数，将撤销操作回滚到指定的标记ID。NULL表示不关心撤销过程中的错误。
> 3. UF_CALL(UF_UNDO_delete_mark(mark_id, NULL));调用了UF_UNDO_delete_mark函数，用于删除指定的标记。NULL表示不关心删除过程中的错误。
> 4. 通过组合调用这两个函数，实现了先撤销到指定标记，然后删除该标记的功能。
> 5. 使用static关键字将函数声明为静态的，意味着这个函数只能在本文件内部使用，不会暴露给外部。
> 6. UF_CALL宏用于简化UFUNC调用的错误处理，它会自动检查UFUNC调用是否返回错误码，如果返回错误码则打印错误信息。
>
> 综上所述，这段代码实现了先撤销到指定标记，然后删除该标记的功能，并且简化了UFUNC调用的错误处理。
>
