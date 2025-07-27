### 【1020】delete body parameters 删除体参数

#### 代码

```cpp
    static void delete_body_parameters(tag_t body)  
    {  
        uf_list_p_t  
            body_list;  
        UF_CALL(UF_MODL_create_list(&body_list));  
        UF_CALL(UF_MODL_put_list_item(body_list, body));  
        UF_CALL(UF_MODL_delete_body_parms(body_list));  
        UF_CALL(UF_MODL_delete_list(&body_list));  
    }

```

#### 代码解析

> 这段代码是用于NX Open C++ API的二次开发代码，主要功能是删除一个或多个body的参数。
>
> 具体来说，代码首先创建一个UF_MODL类型的列表body_list，然后将需要删除参数的body添加到该列表中。接着，调用UF_MODL_delete_body_parms函数来删除列表中所有body的参数。最后，删除body_list列表。
>
> 通过这段代码，我们可以实现批量删除多个body的参数，提高程序的效率和可维护性。
>
