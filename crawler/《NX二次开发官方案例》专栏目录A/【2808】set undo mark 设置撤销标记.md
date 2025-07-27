### 【2808】set undo mark 设置撤销标记

#### 代码

```cpp
    #include <uf_undo.h>  
    static UF_UNDO_mark_id_t set_undo_mark(char *label)  
    {  
        UF_UNDO_mark_id_t  
            mark_id;  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, label, &mark_id));  
        return mark_id;  
    }

```

#### 代码解析

> 这是NX Open C++ API编写的NX二次开发代码，用于在NX中设置撤销标记。
>
> 主要步骤包括：
>
> 1. 包含UF_UNDO.h头文件，该文件定义了与NX撤销/重做相关的函数和数据类型。
> 2. 定义一个名为set_undo_mark的函数，该函数接受一个字符指针参数label。
> 3. 在函数内部，定义一个UF_UNDO_mark_id_t类型的变量mark_id，用于存储生成的撤销标记ID。
> 4. 调用UF_UNDO_set_mark函数，设置一个可见的撤销标记，并传入label和mark_id的地址。
> 5. 如果调用成功，返回生成的撤销标记ID；如果失败，可以添加错误处理代码。
> 6. 使用UF_UNDO_set_mark函数，可以实现在NX中设置撤销标记，方便后续的撤销操作。
>
> 这段代码提供了在NX中设置撤销标记的基础功能，可以根据需要进一步扩展和优化。
>
