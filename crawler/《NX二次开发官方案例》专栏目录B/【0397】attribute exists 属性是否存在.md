### 【0397】attribute exists 属性是否存在

#### 代码

```cpp
    static logical attribute_exists(tag_t object, char *title)  
    {  
        int  
            type;  
        UF_CALL(UF_ATTR_find_attribute(object, UF_ATTR_any, title, &type));  
        if (type == 0) return FALSE;  
        return TRUE;  
    }

```

#### 代码解析

> 这段代码是一个NX二次开发函数，其功能是检查NX对象是否存在指定的属性。
>
> 代码逻辑如下：
>
> 1. 定义函数attribute_exists，它接受两个参数：tag_t object（NX对象句柄）和char *title（要检查的属性名）。
> 2. 在函数内部，定义一个整数变量type，用于存储属性的类型。
> 3. 使用UF_CALL宏调用NX API函数UF_ATTR_find_attribute，尝试在传入的对象中查找指定的属性。
> 4. 如果属性不存在，type将被设置为0。
> 5. 检查type的值，如果为0，则返回FALSE（表示属性不存在）；否则，返回TRUE（表示属性存在）。
> 6. 函数结束。
>
> 这个函数的主要作用是，当需要检查一个NX对象是否存在某个属性时，可以直接调用这个函数，传入对象句柄和属性名，然后根据返回的布尔值判断属性是否存在。这样可以避免直接调用NX API时处理返回值和错误码的复杂性，使代码更简洁易读。
>
