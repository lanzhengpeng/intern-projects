### 【0365】ask udo linked solids 查询与 UDO 关联的实体

#### 代码

```cpp
    static int ask_udo_linked_solids(tag_t udo, tag_t **solids)  
    {  
        int  
            ii,  
            subtype,  
            type;  
        UF_UDOBJ_all_data_t  
            data;  
        uf_list_p_t  
            solid_list;  
        UF_CALL(UF_MODL_create_list(&solid_list));  
        UF_CALL(UF_UDOBJ_ask_udo_data(udo, &data));  
        for (ii = 0; ii < data.num_links; ii++)  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(data.link_defs[ii].assoc_ug_tag,  
                &type, &subtype));  
            if ((type == UF_solid_type) && (subtype == UF_solid_body_subtype))  
                UF_CALL(UF_MODL_put_list_item(solid_list,  
                    data.link_defs[ii].assoc_ug_tag));  
        }  
        UF_CALL(UF_UDOBJ_free_udo_data(&data));  
        return make_an_array(&solid_list, solids);  
    }

```

#### 代码解析

> 这段代码是用于查询NX中UDO关联的实体，并返回一个实体数组。以下是代码的主要逻辑：
>
> 1. 创建一个实体列表，用于存储查询到的实体。
> 2. 获取输入UDO的链接数据，包括链接的类型、子类型和关联的UG实体标签。
> 3. 遍历链接数据，判断链接类型和子类型，如果满足条件(实体类型和体子类型)，则将关联的UG实体标签添加到实体列表中。
> 4. 释放UDO链接数据内存。
> 5. 将实体列表转换为一个实体数组，并返回该数组。
> 6. 如果发生错误，则调用UF_CALL宏进行错误处理。
> 7. make_an_array函数用于从实体列表中提取实体数组。
>
> 综上所述，这段代码实现了查询NX中UDO关联的实体，并返回一个实体数组的功能。
>
