### 【0216】ask feat first cylindrical face 查询第一个圆柱面特征

#### 代码

```cpp
    static tag_t ask_feat_first_cylindrical_face(tag_t feat)  
    {  
        int  
            type;  
        tag_t  
            face = NULL_TAG;  
        uf_list_p_t  
            face_list,  
            temp;  
        UF_CALL(UF_MODL_ask_feat_faces(feat, &face_list));  
        for (temp = face_list; temp != NULL; temp = temp->next)  
        {  
            UF_CALL(UF_MODL_ask_face_type(temp->eid, &type));  
            if (type == UF_MODL_CYLINDRICAL_FACE)  
            {  
                face = temp->eid;  
                break;  
            }  
        }  
        UF_CALL(UF_MODL_delete_list(&face_list));  
        return face;  
    }

```

#### 代码解析

> 这段代码用于在NX Open中查找特征中的第一个圆柱面。代码的主要步骤包括：
>
> 1. 获取特征的所有面，存储在一个链表中。
> 2. 遍历链表，检查每个面的类型。如果面是圆柱面，则将其标记为第一个圆柱面。
> 3. 删除链表以释放内存。
> 4. 返回找到的第一个圆柱面。
>
> 这段代码的关键点包括：
>
> 代码实现了在特征中查找第一个圆柱面的功能，这可以用于后续的二次开发操作，例如基于圆柱面进行对称或阵列等。
>
