### 【1471】is feat of type 是特征类型吗？

#### 代码

```cpp
    static logical is_feat_of_type(tag_t feat, char *type)  
    {  
        logical  
            is_it = TRUE;  
        char  
            *this_type;  
        UF_CALL(UF_MODL_ask_feat_type(feat, &this_type));  
        if (strcmp(type, this_type)) is_it = FALSE;  
        UF_free(this_type);  
        return is_it;  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API的二次开发代码，主要用于判断一个特征是否属于指定类型。代码的主要逻辑包括：
>
> 1. 定义了一个静态逻辑函数is_feat_of_type，用于判断特征是否属于指定类型。函数参数包括特征tag和类型字符串。
> 2. 在函数内部，首先将is_it设置为TRUE，表示默认特征属于指定类型。
> 3. 通过NX Open C++ API中的UF_MODL_ask_feat_type函数，获取特征的类型字符串this_type。
> 4. 使用strcmp函数比较特征的实际类型字符串this_type与指定的类型字符串type，如果不相等，则将is_it设置为FALSE。
> 5. 最后释放通过NX API获取的类型字符串内存，并返回is_it的值。
> 6. 在外部调用此函数时，可以根据返回的布尔值判断特征是否属于指定类型。
> 7. 代码采用了NX Open C++ API的函数，因此只能在NX软件环境中运行。
> 8. 这段代码封装了对NX API的调用，实现了特征类型判断的功能，可以方便地在其他地方调用。
>
