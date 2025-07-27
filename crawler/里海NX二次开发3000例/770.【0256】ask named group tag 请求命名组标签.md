### 【0256】ask named group tag 请求命名组标签

#### 代码

```cpp
    static tag_t ask_named_group_tag(char *name)  
    {  
        tag_t  
            object = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        UF_CALL(UF_OBJ_cycle_by_name_and_type(part, name, UF_group_type,  
            FALSE, &object));  
        return object;  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API的二次开发代码，其主要功能是查询一个名为“name”的组(group)对象。
>
> 代码的主要逻辑如下：
>
> 1. 定义一个tag_t类型的变量object，用于存储查询到的组对象，初始值为NULL_TAG。
> 2. 获取当前显示部件的part对象，用于后续查询。
> 3. 调用UF_OBJ_cycle_by_name_and_type函数，按名称和类型在part中查询名为“name”的组对象。
> 4. 如果查询成功，object将被赋值为查询到的组对象，否则为NULL_TAG。
> 5. 返回查询到的组对象。
> 6. 使用UF_CALL宏来捕获函数调用可能抛出的异常。
> 7. 使用static关键字将函数作用域限定在当前文件内部。
>
> 这段代码实现了按名称查询组对象的功能，可以用于获取特定名称的组对象，以便进行后续的二次开发操作。
>
