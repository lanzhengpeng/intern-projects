### 【0280】ask next midsurface sheet body遍历零件中的实体，并找到下一个具有中面特征的面体

#### 代码

```cpp
    static tag_t ask_next_midsurface_sheet_body(tag_t part, tag_t body)  
    {  
        int  
            subtype,  
            type;  
        char  
            *f_type;  
        uf_list_p_t  
            feat_list,  
            tmp;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_solid_type, &body))  
            && (body != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(body, &type, &subtype));  
            if (subtype == UF_solid_body_subtype)  
            {  
                UF_CALL(UF_MODL_ask_body_type(body, &type));  
                if (type == UF_MODL_SHEET_BODY)  
                {  
                    UF_CALL(UF_MODL_ask_body_feats(body, &feat_list));  
                    for (tmp = feat_list; tmp != NULL; tmp = tmp->next)  
                    {  
                        UF_CALL(UF_MODL_ask_feat_type(tmp->eid, &f_type));  
                        if (!strcmp(f_type, "MIDSURFACE"))  
                        {  
                            UF_free(f_type);  
                            UF_CALL(UF_MODL_delete_list(&feat_list));  
                            return body;  
                        }  
                        UF_free(f_type);  
                    }  
                    UF_CALL(UF_MODL_delete_list(&feat_list));  
                }  
            }  
        }  
        return NULL_TAG;  
    }

```

#### 代码解析

> 这段NX二次开发代码的主要功能是遍历指定零件中的实体，以找到下一个中面实体。
>
> 具体步骤如下：
>
> 1. 使用UF_OBJ_cycle_objs_in_part函数循环遍历指定零件中的所有实体。
> 2. 对于每个实体，调用UF_OBJ_ask_type_and_subtype函数获取实体的类型和子类型。
> 3. 如果实体的子类型为UF_solid_body_subtype（实体子类型），则调用UF_MODL_ask_body_type函数获取实体的类型。
> 4. 如果实体类型为UF_MODL_SHEET_BODY（薄板实体），则调用UF_MODL_ask_body_feats函数获取实体上的特征列表。
> 5. 遍历特征列表，使用UF_MODL_ask_feat_type函数获取每个特征类型，并与字符串"MIDSURFACE"进行比较。
> 6. 如果找到类型为"MIDSURFACE"的特征，则释放特征类型字符串和特征列表，并返回该实体。
> 7. 如果遍历完所有实体都没有找到中面实体，则返回NULL_TAG。
>
> 总的来说，这段代码通过遍历零件实体并检查实体类型和特征类型，实现了查找下一个中面实体的功能。
>
