### 【0187】ask dwg tag 查询DWG标签

#### 代码

```cpp
    static tag_t ask_dwg_tag(char *dwg_name)  
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

> 这段代码是一个在NX Open中获取特定名称的DWG文件的函数。主要功能是通过循环遍历模型中的所有对象，直到找到名称匹配的DWG文件，然后返回该DWG文件的tag。关键点包括：
>
> 1. 函数名为ask_dwg_tag，接受一个字符指针参数dwg_name，表示要查询的DWG文件名称。
> 2. 定义了tag_t类型的dwg变量，用于存储找到的DWG文件的tag。
> 3. 使用while循环调用UF_OBJ_cycle_by_name函数遍历模型中的所有对象，直到返回错误或找到名称匹配的DWG文件。
> 4. 在循环体内，调用UF_OBJ_ask_type_and_subtype函数获取当前对象的类型和子类型。
> 5. 如果当前对象是DWG文件（type等于UF_drawing_type），则返回该对象的tag。
> 6. 如果遍历完所有对象都没有找到匹配的DWG文件，则返回NULL_TAG。
> 7. 函数整体使用了UF_CALL宏来简化错误处理。
> 8. 该函数的主要作用是在NX模型中根据名称查询并获取特定的DWG文件，返回其tag，以便后续操作。
>
