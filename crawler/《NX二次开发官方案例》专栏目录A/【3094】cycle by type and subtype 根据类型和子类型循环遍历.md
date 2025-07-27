### 【3094】cycle by type and subtype 根据类型和子类型循环遍历

#### 代码

```cpp
    static tag_t cycle_by_type_and_subtype(tag_t part, int type,  
        int subtype, tag_t object)  
    {  
        int  
            this_type,  
            this_subtype;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, type,   
            &object)) && (object != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(object, &this_type, &this_subtype));  
            if (this_subtype == subtype) return object;  
        }  
        return NULL_TAG;  
    }

```

#### 代码解析

> 这段代码是NX二次开发中的静态函数，主要用于在NX部件中按照类型和子类型循环查找对象。
>
> 具体来说，代码的输入参数包括：
>
> 该函数首先调用UF_OBJ_cycle_objs_in_part在部件中按照指定类型循环查找对象，如果找到的对象子类型与指定子类型相同，则返回该对象；否则继续循环查找下一个对象。如果循环结束后仍未找到匹配的对象，则返回NULL_TAG。
>
> 总的来说，该函数提供了一个在NX部件中根据类型和子类型快速查找特定对象的方法。
>
