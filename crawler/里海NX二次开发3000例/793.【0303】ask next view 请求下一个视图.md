### 【0303】ask next view 请求下一个视图

#### 代码

```cpp
    static tag_t ask_next_view(tag_t part, tag_t view)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_view_type, &view));  
        return (view);  
    }

```

#### 代码解析

> 这段代码是NX二次开发中的一个函数，其作用是在指定的部件(part)中，获取下一个视图(view)。
>
> 具体来说，函数ask_next_view接受两个参数：
>
> 函数内部调用了NX提供的API函数UF_OBJ_cycle_objs_in_part，这个函数可以在部件内部按照某种规则遍历对象。在这里，我们指定遍历的规则为UF_view_type，也就是只遍历视图类型的对象。此外，我们还指定了当前的视图view作为遍历的起始点。
>
> 函数返回值是遍历到的下一个视图，如果没有下一个视图，则返回当前视图view。
>
> 因此，这个函数的作用就是获取指定部件中的下一个视图，方便用户在多个视图之间切换。这个功能在二次开发中比较常见，可以提高用户体验。
>
