### 【2761】set feature face color 设置特征面颜色

#### 代码

```cpp
    static void set_feature_face_color(tag_t feat, int color)  
    {  
        uf_list_p_t  
            face_list,  
            temp;  
        UF_CALL(UF_MODL_ask_feat_faces(feat, &face_list));  
        for (temp = face_list; temp != NULL; temp = temp->next)  
            UF_CALL(UF_OBJ_set_color(temp->eid, color));  
        UF_CALL(UF_MODL_delete_list(&face_list));  
    }

```

#### 代码解析

> 这段代码是用于NX二次开发的，其主要功能是设置NX模型中特征面的颜色。
>
> 代码中的关键点如下：
>
> 1. 函数set_feature_face_color接受两个参数，一个是特征feat，另一个是颜色color。
> 2. 通过UF_MODL_ask_feat_faces函数获取该特征的所有面组成的列表face_list。
> 3. 遍历face_list，对每个面调用UF_OBJ_set_color函数设置颜色。
> 4. 最后调用UF_MODL_delete_list删除临时列表face_list。
> 5. 整个函数通过UF_CALL宏包装NX的函数调用，用于处理错误。
>
> 通过这段代码，我们可以方便地批量设置特征面的颜色，从而在NX模型中高亮显示某个特征，以提高可视化效果。
>
