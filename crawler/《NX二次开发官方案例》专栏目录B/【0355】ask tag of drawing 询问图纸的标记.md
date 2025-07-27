### 【0355】ask tag of drawing 询问图纸的标记

#### 代码

```cpp
    static tag_t ask_tag_of_drawing(char *dwg_name)  
    {  
        tag_t  
            dwg = NULL_TAG;  
        int  
            subtype,  
            type;  
        while (!UF_CALL(UF_OBJ_cycle_by_name(dwg_name, &dwg)) && dwg)  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(dwg, &type, &subtype));  
            if (type == UF_drawing_type) return dwg;  
        }  
        return NULL_TAG;  
    }

```

#### 代码解析

> 这段代码是用于在NX CAD软件中通过名称查询图纸的二次开发代码。其基本功能包括：
>
> 1. 定义了一个静态函数ask_tag_of_drawing，用于根据图纸名称查询图纸的TAG。
> 2. 在函数中，首先定义了图纸的TAGdwg并初始化为NULL_TAG。
> 3. 使用UF_OBJ_cycle_by_name函数循环查询指定名称的图纸，直到找到图纸或循环结束。
> 4. 查询到图纸后，使用UF_OBJ_ask_type_and_subtype函数获取图纸的类型和子类型。
> 5. 如果图纸的类型为UF_drawing_type，则返回图纸的TAG。
> 6. 如果没有找到图纸，则返回NULL_TAG。
> 7. 该函数通过图纸名称查询图纸的TAG，可用于获取图纸的相关信息，为后续的二次开发提供基础。
> 8. 使用UFUNC调用的方式，符合NX的二次开发规范。
>
> 总的来说，这段代码实现了基于名称查询图纸TAG的基本功能，为后续的开发提供了基础。
>
