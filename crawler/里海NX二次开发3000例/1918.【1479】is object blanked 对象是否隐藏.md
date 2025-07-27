### 【1479】is object blanked 对象是否隐藏

#### 代码

```cpp
    static logical is_object_blanked(tag_t object)  
    {  
        UF_OBJ_disp_props_t  
            disp_props;  
        UF_CALL(UF_OBJ_ask_display_properties(object, &disp_props));  
        return (logical) disp_props.blank_status;  
    }

```

#### 代码解析

> 这段代码用于检查NX中的对象是否被隐藏或被遮蔽。具体来说，它执行以下功能：
>
> 1. 定义了一个名为is_object_blanked的静态逻辑函数，用于检查对象是否被隐藏。
> 2. 该函数接受一个tag_t类型的参数，代表要检查的对象。
> 3. 定义了一个UF_OBJ_disp_props_t类型的结构体变量disp_props，用于存储对象的显示属性。
> 4. 调用UF_OBJ_ask_display_properties函数，传入对象tag和disp_props，获取对象的显示属性。
> 5. 返回disp_props结构体中的blank_status成员，该成员表示对象是否被隐藏。如果为真，表示对象被隐藏。
> 6. 整个函数以逻辑值的形式返回对象是否被隐藏的结果。
>
> 综上所述，这段代码实现了通过NX的UF函数检查对象是否被隐藏的功能，返回一个逻辑值表示结果。
>
