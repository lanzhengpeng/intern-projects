### 【0314】ask object layer 询问对象层

#### 代码

```cpp
    static int ask_object_layer(tag_t object)  
    {  
        logical  
            is_displayable;  
        UF_OBJ_disp_props_t  
            disp_props;  
        UF_CALL(UF_OBJ_is_displayable(object, &is_displayable));  
        if (is_displayable &&  
            !UF_CALL(UF_OBJ_ask_display_properties(object, &disp_props)))  
                return disp_props.layer;  
        else  
            return -1;  
    }

```

#### 代码解析

> 这段代码是用于获取NX中对象的图层信息的二次开发代码。具体来说，代码中定义了一个静态函数ask_object_layer，用于查询传入的对象是否具有显示属性，并获取其所在的图层编号。
>
> 函数的主要逻辑包括以下步骤：
>
> 1. 首先调用UF_OBJ_is_displayable函数，判断传入的对象是否具有显示属性。
> 2. 如果对象具有显示属性，则调用UF_OBJ_ask_display_properties函数，获取对象的显示属性结构体disp_props。
> 3. 从disp_props中提取出layer成员，即对象的图层编号。
> 4. 如果对象不具有显示属性，则返回-1表示无法获取图层信息。
> 5. 函数的返回值为对象的图层编号，如果没有获取到图层信息则返回-1。
>
> 这段代码通过NX提供的UF函数，实现了对对象图层信息的查询，从而为后续的显示控制等操作提供了基础。
>
