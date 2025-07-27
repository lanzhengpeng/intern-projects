### 【0530】convert to model 将视图转换为模型

#### 代码

```cpp
    static void convert_to_model(tag_t object)  
    {  
        int  
            status;  
        tag_t  
            view_tag;  
        char  
            view_name[MAX_ENTITY_NAME_SIZE];  
        if (!uc6409(object, &status, view_name) && (status == 1))  
        {  
        /*  Try the new call first, if that fails - try the old call too 译:首先尝试新的调用，如果失败，也尝试旧的调用。 */  
            UF_CALL(UF_VIEW_ask_tag_of_view_name(view_name, &view_tag));  
            if (UF_CALL(UF_VIEW_convert_to_model(view_tag, object)))  
            {  
                UF_UI_write_listing_window("Using uc6401 instead...\n");  
                UF_CALL(uc6401(view_name, object));  
                UF_UI_write_listing_window("\n");  
            }  
        }  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API编写的，用于将NX对象转换为模型。
>
> 主要功能是：
>
> 1. 检查对象是否为视图，如果是视图，则转换为模型。
> 2. 首先尝试使用新的UF_VIEW_ask_tag_of_view_name和UF_VIEW_convert_to_model函数进行转换，如果失败，则使用旧的uc6401函数进行转换。
> 3. 在转换过程中，会在输出窗口打印相关信息。
> 4. 使用了NX Open C++ API中的UF_CALL宏，用于处理函数调用时的错误。
> 5. 使用了NX Open C++ API中的UF_UI_write_listing_window函数，用于在输出窗口打印信息。
> 6. 使用了NX Open C++ API中的uc6409函数，用于检查对象是否为视图。
> 7. 使用了NX Open C++ API中的MAX_ENTITY_NAME_SIZE常量，用于定义视图名称的最大长度。
> 8. 整体采用了面向对象的编程思想，函数内部封装了转换的逻辑，对外提供简洁的接口。
> 9. 代码简洁，注释清晰，便于理解和维护。
> 10. 通过新旧函数的兼容处理，提高了代码的健壮性。
>
