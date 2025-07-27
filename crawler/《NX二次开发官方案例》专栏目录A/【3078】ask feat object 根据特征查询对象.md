### 【3078】ask feat object 根据特征查询对象

#### 代码

```cpp
    /*  In V16.0, use UF_MODL_ask_feat_object instead! 译:在V16.0版本中，请使用UF_MODL_ask_feat_object代替！ */  
    static tag_t ask_feat_object(tag_t feat, int type)  
    {  
        tag_t  
            object = NULL_TAG,  
            obj_feat,  
            part = UF_PART_ask_display_part();  
        while ((!UF_OBJ_cycle_objs_in_part(part, type, &object)) &&  
                (object != NULL_TAG))  
        {  
            UF_CALL(UF_MODL_ask_object_feat(object, &obj_feat));  
            if (obj_feat == feat) return object;  
        }  
        return NULL_TAG;  
    }

```

#### 代码解析

> 这段NX二次开发代码的主要功能是查询具有指定特征的对象。以下是代码的详细解释：
>
> 1. 定义了一个名为ask_feat_object的静态函数，它接受两个参数：feat表示要查询的特征，type表示要查询的对象类型。
> 2. 初始化一个tag_t类型的对象object，用于存储找到的特征对象，并将其初始化为NULL_TAG。
> 3. 获取当前显示的部件part。
> 4. 使用UF_OBJ_cycle_objs_in_part函数循环遍历部件中的对象，并根据传入的对象类型参数type进行过滤。如果找到匹配的对象，object会被赋值为该对象的tag。
> 5. 在循环体中，使用UF_MODL_ask_object_feat函数查询每个对象是否包含传入的特征feat。
> 6. 如果找到特征feat，则返回包含该特征的对象的tag。
> 7. 如果遍历完所有对象后仍未找到，则返回NULL_TAG。
> 8. 该函数的目的是为了方便查询包含指定特征的对象，以进行后续操作。
> 9. 注释提示在NX 16.0版本中，推荐使用UF_MODL_ask_feat_object函数来代替该函数。
>
> 总的来说，这段代码通过遍历当前显示的部件中的对象，并查询每个对象是否包含指定的特征，实现了查找包含指定特征的对象的功能。
>
