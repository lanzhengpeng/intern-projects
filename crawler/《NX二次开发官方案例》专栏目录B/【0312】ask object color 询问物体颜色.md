### 【0312】ask object color 询问物体颜色

#### 代码

```cpp
    static int ask_object_color(tag_t obj)  
    {  
        logical  
            is_disp;  
        UF_OBJ_disp_props_t  
            disp_props;  
        UF_CALL(UF_OBJ_is_displayable(obj, &is_disp));  
        if (is_disp)  
        {  
            UF_CALL(UF_OBJ_ask_display_properties(obj, &disp_props));  
            return disp_props.color;  
        }  
        else return 0;  
    }

```

#### 代码解析

> 这段代码是NX二次开发中用于查询对象显示颜色的函数。具体来说，它的主要功能是：
>
> 1. 首先调用UF_OBJ_is_displayable函数，判断传入的对象是否可以显示。
> 2. 如果对象可以显示，则调用UF_OBJ_ask_display_properties函数获取对象的显示属性结构体。
> 3. 从显示属性结构体中提取出颜色信息，并返回给调用者。
> 4. 如果对象不可以显示，则直接返回0表示颜色不可用。
> 5. 整个函数通过NX提供的UF函数进行对象属性的查询，利用NX提供的对象模型进行二次开发。
> 6. 函数以static修饰，表示只在当前文件内部可见，不需要被外部文件调用。
> 7. 参数使用NX提供的tag_t类型表示对象标识，返回值使用整数表示颜色。
> 8. 使用UF_CALL宏来简化UF函数的调用，确保调用成功。
>
> 总的来说，这段代码实现了通过NX的对象模型查询对象显示颜色的功能，是NX二次开发中典型的对象属性查询代码示例。
>
