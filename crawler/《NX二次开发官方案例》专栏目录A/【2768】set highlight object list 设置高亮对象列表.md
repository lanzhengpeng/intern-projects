### 【2768】set highlight object list 设置高亮对象列表

#### 代码

```cpp
    static void set_highlight_object_list(uf_list_p_t list, int flag)  
    {  
        uf_list_p_t  
            tmp;  
        for (tmp = list; tmp != NULL; tmp = tmp->next)  
            UF_CALL(UF_DISP_set_highlight(tmp->eid, flag));  
    }

```

#### 代码解析

> 这段代码是一个用于设置NX(UG)模型中对象高亮的二次开发函数。其基本功能是遍历一个对象链表，并对链表中的每个对象设置高亮状态。下面是代码的详细解释：
>
> 1. 函数名为set_highlight_object_list，接受两个参数：对象链表list和高亮标志flag。
> 2. 遍历对象链表，链表中的每个节点都包含一个对象ID(eid)。
> 3. 对每个对象，调用NX提供的UF_DISP_set_highlight函数，设置其高亮状态。flag参数为1表示高亮，为0表示取消高亮。
> 4. UF_CALL宏用于调用NX的UF函数，并处理错误。
> 5. 整个函数通过遍历对象链表，实现了对多个对象高亮状态的批量设置。
>
> 该函数的用途是在NX的二次开发中，快速高亮或取消高亮多个对象，以提高交互性。
>
