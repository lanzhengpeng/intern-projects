### 【0218】ask feat first face 问特征第一个面

#### 代码

```cpp
    static tag_t ask_feat_first_face(tag_t feat)  
    {  
        int  
            n;  
        tag_t  
            face;  
        uf_list_p_t  
            face_list;  
        UF_CALL(UF_MODL_ask_feat_faces(feat, &face_list));  
        UF_CALL(UF_MODL_ask_list_count(face_list, &n));  
        if (n > 0)  
        {  
            UF_CALL(UF_MODL_ask_list_item(face_list, 0, &face));  
            UF_CALL(UF_MODL_delete_list(&face_list));  
            return face;  
        }  
        else  
            return NULL_TAG;  
    }

```

#### 代码解析

> 这段代码是NX的二次开发代码，主要功能是查询特征（feat）的第一个面（face）。
>
> 代码流程如下：
>
> 1. 定义变量：n表示面的数量，face表示查询到的面，face_list表示面的列表。
> 2. 调用UF_MODL_ask_feat_faces函数，查询特征feat的所有面，结果保存在face_list列表中。
> 3. 调用UF_MODL_ask_list_count函数，查询face_list列表中的面数量，结果保存在变量n中。
> 4. 判断面数量n是否大于0，如果大于0，则调用UF_MODL_ask_list_item函数，查询face_list列表中的第1个面（索引为0），结果保存在变量face中。
> 5. 调用UF_MODL_delete_list函数，删除face_list列表。
> 6. 如果n大于0，返回查询到的第一个面face；否则返回NULL_TAG。
>
> 综上，这段代码实现了查询特征feat的第一个面的功能。
>
