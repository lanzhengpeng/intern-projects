### 【2050】report customer default 报告客户默认值

#### 代码

```cpp
    static void report_customer_default(char *default_name)  
    {  
        char  
            *default_value = NULL;  
        if (!UF_CALL(UF_get_customer_default(default_name, 0, &default_value)))  
        {  
            ECHO("Customer Default %s = %s\n", default_name, default_value);  
            UF_free(default_value);  
        }  
    }

```

#### 代码解析

> 这段NX Open C++代码是用于获取NX的用户默认设置的功能。具体来说，代码实现的功能包括：
>
> 1. 定义一个静态函数report_customer_default，用于报告指定名称的用户默认设置。
> 2. 在函数内部，定义一个字符指针default_value，用于存储获取到的默认值。
> 3. 使用UF_get_customer_default函数，传入默认设置的名称和长度限制，尝试获取该默认设置对应的值，存储在default_value指向的内存中。
> 4. 如果获取成功，使用ECHO宏输出默认设置的名称和值。
> 5. 释放通过UF_get_customer_default函数分配的内存。
> 6. 通过调用report_customer_default函数，传入要查询的默认设置名称，可以获取并输出该默认设置。
>
> 综上所述，这段代码通过UF函数实现了对NX用户默认设置的查询功能，并以静态函数的形式提供，便于在其他代码中调用。
>
