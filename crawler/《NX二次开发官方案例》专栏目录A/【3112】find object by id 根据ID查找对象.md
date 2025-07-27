### 【3112】find object by id 根据ID查找对象

#### 代码

```cpp
    static tag_t find_object_by_id(int id)  
    {  
        unsigned int  
            sub_id,  
            ver;  
        tag_t  
            obj = NULL_TAG,  
            part = UF_ASSEM_ask_work_part();  
        char  
            *file_data,  
            *handle;  
        do  
        {  
            obj = UF_OBJ_cycle_all(part, obj);  
            if (obj == NULL_TAG) return NULL_TAG;  
            handle = UF_TAG_ask_handle_of_tag(obj);  
            UF_CALL(UF_TAG_decompose_handle(handle, &file_data, &sub_id, &ver));  
            UF_free(handle);  
            UF_free(file_data);  
            if (sub_id == id) return obj;  
        } while (obj != NULL_TAG);  
        return NULL_TAG;  
    }

```

#### 代码解析

> 这段NX Open API代码的主要功能是遍历当前工作部件中的所有对象，并找到具有特定ID的对象。
>
> 具体步骤如下：
>
> 1. 定义函数find_object_by_id，用于根据ID查找对象。
> 2. 初始化变量：part表示当前工作部件，obj表示当前遍历到的对象，id表示需要查找的ID。
> 3. 使用循环遍历当前工作部件中的所有对象，每次循环调用UF_OBJ_cycle_all获取下一个对象。
> 4. 如果遍历到的对象为空，则返回空对象。
> 5. 获取当前对象的句柄，并使用UF_TAG_decompose_handle获取对象的数据和ID。
> 6. 检查当前对象的ID是否等于要查找的ID，如果相等则返回该对象。
> 7. 释放对象句柄和数据，继续遍历下一个对象。
> 8. 如果遍历完所有对象后仍未找到，则返回空对象。
> 9. 函数结束。
>
> 通过这段代码，可以根据给定的ID找到工作部件中对应的对象，并返回该对象的tag。
>
