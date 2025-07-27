### 【0270】ask next feature of type 寻找下一个指定类型的特征

#### 代码

```cpp
    static tag_t ask_next_feature_of_type(tag_t part, char *type, tag_t feat)  
    {  
        char  
            *this_type;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_feature_type, &feat))  
            && (feat != NULL_TAG))  
        {  
            if (UF_OBJ_ask_status(feat) == UF_OBJ_ALIVE)  
            {  
                UF_CALL(UF_MODL_ask_feat_type(feat, &this_type));  
                if (!strcmp(this_type, type))  
                {  
                    UF_free(this_type);  
                    return feat;  
                }  
                UF_free(this_type);  
            }  
        }  
        return NULL_TAG;  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API编写的，主要功能是在NX部件中遍历指定类型的特征，并返回下一个特征。
>
> 代码的基本逻辑如下：
>
> 1. 定义了函数ask_next_feature_of_type，该函数接受三个参数：part表示要遍历的部件，type表示要查找的特征类型，feat表示当前特征。
> 2. 使用循环遍历部件中的特征，直到遍历结束或找到下一个指定类型的特征。
> 3. 在循环中，首先检查当前特征的状态是否为UF_OBJ_ALIVE，表示该特征是激活的。
> 4. 然后获取当前特征的类型，并与指定的类型进行比较。
> 5. 如果找到匹配的特征，则释放特征类型字符串并返回该特征。
> 6. 如果没有找到匹配的特征，则释放特征类型字符串，继续遍历下一个特征。
> 7. 如果遍历结束仍未找到匹配的特征，则返回NULL_TAG。
> 8. 使用UF_CALL宏来调用NX API函数，并处理错误。
>
> 总的来说，这段代码实现了在NX部件中查找指定类型特征的功能，并提供了简单的错误处理机制。
>
