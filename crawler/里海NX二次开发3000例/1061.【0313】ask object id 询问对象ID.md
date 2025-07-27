### 【0313】ask object id 询问对象ID

#### 代码

```cpp
    static int ask_object_id(tag_t object)  
    {  
        unsigned int  
            ID,  
            ver;  
        char  
            *file_data,  
            *handle;  
        handle = UF_TAG_ask_handle_of_tag(object);  
        UF_CALL(UF_TAG_decompose_handle(handle, &file_data, &ID, &ver));  
        UF_free(handle);  
        UF_free(file_data);  
        return ID;  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API的一个示例，用于提取NX对象在NX数据库中的ID号。以下是代码的主要功能和步骤：
>
> 1. 函数名称和参数：函数名为ask_object_id，接受一个NX对象的标签object作为参数。函数的返回类型为int，表示提取到的ID号。
> 2. 函数名为ask_object_id，接受一个NX对象的标签object作为参数。
> 3. 函数的返回类型为int，表示提取到的ID号。
> 4. 定义变量：ID用于存储提取到的ID号。ver用于存储对象的版本号。file_data和handle用于存储中间结果。
> 5. ID用于存储提取到的ID号。
> 6. ver用于存储对象的版本号。
> 7. file_data和handle用于存储中间结果。
> 8. 获取对象句柄：使用UF_TAG_ask_handle_of_tag函数，根据输入的NX对象标签获取其句柄handle。
> 9. 使用UF_TAG_ask_handle_of_tag函数，根据输入的NX对象标签获取其句柄handle。
> 10. 分解句柄：使用UF_TAG_decompose_handle函数，根据句柄handle分解出file_data、ID和ver。
> 11. 使用UF_TAG_decompose_handle函数，根据句柄handle分解出file_data、ID和ver。
> 12. 释放内存：使用UF_free函数释放handle和file_data占用的内存。
> 13. 使用UF_free函数释放handle和file_data占用的内存。
> 14. 返回ID：函数返回提取到的ID号。
> 15. 函数返回提取到的ID号。
>
> 总体来说，这段代码的主要功能是提取NX对象在NX数据库中的ID号。它通过获取对象的句柄，分解句柄来获取ID号，然后释放中间变量占用的内存，最终返回ID号。这是一个典型的NX Open C++ API的使用示例。
>
