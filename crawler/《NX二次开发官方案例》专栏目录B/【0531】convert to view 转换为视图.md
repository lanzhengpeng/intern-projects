### 【0531】convert to view 转换为视图

#### 代码

```cpp
    static void convert_to_view(tag_t object, tag_t view)  
    {  
        int  
            status;  
        char  
            view_name[MAX_ENTITY_NAME_SIZE];  
        if (!uc6409(object, &status, view_name) && (status == 0))  
        {  
        /*  Try the new call first, if that fails - try the old call too 译:尝试新的调用，如果失败，再尝试旧的调用。 */  
            if (UF_CALL(UF_VIEW_convert_to_view(view, object)))  
            {  
                UF_UI_write_listing_window("Using uc6402 instead...\n");  
                UF_CALL(UF_OBJ_ask_name(view, view_name));  
                UF_CALL(uc6402(view_name, object));  
                UF_UI_write_listing_window("\n");  
            }  
        }  
    }

```

#### 代码解析

> 这段代码是NX的二次开发代码，主要实现了将一个NX对象转换为一个NX视图的功能。
>
> 具体步骤如下：
>
> 1. 使用uc6409函数尝试将对象转换为新视图，并检查转换状态。
> 2. 如果uc6409失败，则尝试使用旧的函数uc6402进行转换。首先调用UF_VIEW_convert_to_view函数将对象转换为新视图，如果失败，则调用UF_OBJ_ask_name获取新视图的名称，然后调用uc6402函数将对象转换为该视图。
> 3. 在转换过程中，会在NX的列表窗口中输出相应的信息提示。
> 4. 使用static关键字将函数声明为静态函数，表示该函数仅在本文件中可见。
> 5. 参数tag_t表示NX的对象句柄。
> 6. MAX_ENTITY_NAME_SIZE表示视图名称的最大长度。
> 7. UF_CALL宏用于调用NX的UF函数，并检查返回状态。
> 8. UF_UI_write_listing_window用于在NX的列表窗口中输出文本信息。
>
> 总之，这段代码通过新旧两种方法实现了NX对象到NX视图的转换，并且进行了错误处理和提示输出，展现了良好的编程习惯。
>
