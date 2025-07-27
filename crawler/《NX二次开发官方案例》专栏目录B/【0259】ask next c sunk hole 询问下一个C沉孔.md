### 【0259】ask next c sunk hole 询问下一个C沉孔

#### 代码

```cpp
    static tag_t ask_next_c_sunk_hole(tag_t part, tag_t feat)  
    {  
        char  
            *type;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_feature_type, &feat))  
            && (feat != NULL_TAG))  
        {  
            if (UF_OBJ_ask_status(feat) == UF_OBJ_ALIVE)  
            {  
                UF_CALL(UF_MODL_ask_feat_type(feat, &type));  
                if (!strcmp(type, "CSUNK_HOLE"))  
                {  
                    UF_free(type);  
                    return feat;  
                }  
                UF_free(type);  
            }  
        }  
        return NULL_TAG;  
    }

```

#### 代码解析

> 这段代码是一个NX Open C++ API的示例，用于遍历NX部件中的特征，并找出下一个类型为“CSUNK_HOLE”的特征。以下是代码的详细解释：
>
> 1. 函数定义：ask_next_c_sunk_hole是一个静态函数，接受两个参数part和feat，分别代表NX部件和特征标签。
> 2. 初始化：定义一个字符指针type用于存储特征类型。
> 3. 循环遍历：使用UF_OBJ_cycle_objs_in_part函数遍历部件中的特征。在循环中，feat会获取下一个特征。
> 4. 状态判断：使用UF_OBJ_ask_status函数判断特征状态，只有当特征状态为UF_OBJ_ALIVE时，才继续后续操作。
> 5. 特征类型获取：调用UF_MODL_ask_feat_type函数获取特征的类型字符串，并将其存储在type指针指向的内存中。
> 6. 类型判断：使用strcmp函数判断特征类型是否为“CSUNK_HOLE”。
> 7. 内存释放：无论特征类型是否匹配，都需要调用UF_free函数释放type指针指向的内存。
> 8. 返回结果：如果找到匹配的特征，则返回该特征的标签。如果遍历结束都没有找到匹配的特征，则返回NULL_TAG。
>
> 总的来说，这段代码实现了NX部件中特征的遍历，并返回下一个类型为“CSUNK_HOLE”的特征。在NX二次开发中，类似的函数可以用于实现特定的特征查找功能。
>
