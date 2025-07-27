### 【2010】report body type 报告体类型

#### 代码

```cpp
    static void report_body_type(tag_t object)  
    {  
        int  
            body_type;  
        UF_CALL(UF_MODL_ask_body_type(object, &body_type));  
        printf("tag %d is body type %d\n", object, body_type);  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API的示例，用于查询和打印NX模型中实体的body类型。
>
> 主要功能包括：
>
> 1. 定义了一个静态函数report_body_type，用于报告实体的body类型。
> 2. 参数tag_t object表示要查询的NX实体的tag。
> 3. 调用NX Open API函数UF_MODL_ask_body_type查询实体的body类型，并存储在body_type变量中。
> 4. 打印输出实体的tag和对应的body类型。
> 5. 使用UF_CALL宏来调用NX Open API函数，以确保在函数调用失败时能够输出错误信息。
> 6. 使用printf函数输出结果，格式为"tag %d is body type %d\n"。
>
> 总的来说，这段代码实现了查询NX模型中指定实体的body类型，并通过打印输出结果来报告查询结果。这段代码可作为NX二次开发中查询实体属性的示例代码。
>
