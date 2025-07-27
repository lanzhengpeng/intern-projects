### 【0538】copy object attributes 复制对象属性

#### 代码

```cpp
    static void copy_object_attributes(tag_t from_object, tag_t to_object)  
    {  
        int  
            ii = 0;  
        char  
            title[UF_ATTR_MAX_TITLE_LEN+1];  
        UF_ATTR_value_t  
            att;  
        while (!UF_CALL(UF_ATTR_cycle(from_object, &ii, UF_ATTR_any, title, &att))  
            && (ii != 0))  
        {  
            UF_CALL(UF_ATTR_assign(to_object, title, att));  
            if (att.type == UF_ATTR_string) UF_free(att.value.string);  
            if (att.type == UF_ATTR_reference) UF_free(att.value.reference);  
        }  
    }

```

#### 代码解析

> 这段NX二次开发代码实现了从一个NX对象复制属性到另一个NX对象的功能。
>
> 代码的主要步骤包括：
>
> 1. 初始化变量ii为0，用于循环遍历源对象的属性。
> 2. 使用循环遍历源对象的属性，获取属性标题和值，直到所有属性遍历完成。
> 3. 在每次循环中，调用UF_ATTR_cycle函数获取源对象的下一个属性，参数ii用于控制遍历。
> 4. 获取属性标题和值后，调用UF_ATTR_assign函数将属性复制到目标对象。
> 5. 如果属性类型为字符串或引用，则释放对应的内存。
> 6. 循环结束后，目标对象将具有与源对象相同的属性值。
>
> 该代码通过遍历源对象的属性，逐个复制到目标对象，实现了NX对象属性的复制功能。释放字符串和引用类型的内存是为了避免内存泄露。
>
