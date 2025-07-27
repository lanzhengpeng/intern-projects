### 【0329】ask parts list tag 查询零件列表标签

#### 代码

```cpp
    /*  See PR 6747880  
    Contrary to the documentation UF_PLIST_ask_tags will NOT return the default  
    non-visible parts list object (as UF_PLIST_ask_tag does) and it will not create  
    a parts list.  It will return num=0 if the parts list has not been inserted yet  
    and num=1 if it has.  
    */  
    static tag_t ask_parts_list_tag(void)  
    {  
        int  
            n_plists;  
        tag_t  
            plist,  
            &plists;  
        UF_CALL(UF_PLIST_ask_tags(&plists, &n_plists));  
        if (n_plists == 0) return NULL_TAG;  
        plist = plists[0];  
        UF_free(plists);  
        return plist;  
    }  
    /*  The method below is obsolete and may not work.  
    static int ask_object_layer(tag_t object)  
    {  
        logical  
            is_displayable;  
        UF_OBJ_disp_props_t  
            disp_props;  
        UF_CALL(UF_OBJ_is_displayable(object, &is_displayable));  
        if (is_displayable &&  
            !UF_CALL(UF_OBJ_ask_display_properties(object, &disp_props)))  
                return disp_props.layer;  
        else  
            return -1;  
    }  
    static tag_t ask_parts_list_tag(tag_t part, tag_t plist)  
    {  
        int  
            subtype,  
            type;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_tabular_note_type,  
            &plist)) && (plist != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(plist, &type, &subtype));  
            if ((subtype == UF_parts_list_subtype) &&  
                (ask_object_layer(plist) > 0)) return plist;  
        }  
        return NULL_TAG;  
    }

```

#### 代码解析

> 这段NX Open C++代码用于获取NX部件的部件列表对象。下面是代码的功能和流程介绍：
>
> 1. ask_parts_list_tag函数：用于获取NX部件的默认部件列表对象。它通过调用UF_PLIST_ask_tags获取所有部件列表对象，并返回第一个对象作为默认的部件列表对象。如果没有创建任何部件列表，则返回NULL_TAG。
> 2. ask_object_layer函数：用于判断NX对象是否在可见层。它首先检查对象是否可显示，如果是，则获取对象的显示属性，并返回对象的层号。如果对象不可显示或获取属性失败，则返回-1。
> 3. ask_parts_list_tag重载函数：用于在给定部件和部件列表的情况下，获取该部件的默认部件列表对象。它通过遍历部件中的所有表格注释对象，判断对象类型是否为部件列表，并且是否在可见层，如果是，则返回该对象。如果遍历结束没有找到符合条件的对象，则返回NULL_TAG。
> 4. 代码注释：代码注释详细解释了函数的功能和流程，并指出了UF_PLIST_ask_tags函数的一个已知问题，即它不会返回默认不可见的部件列表对象，而是返回0或1表示部件列表对象的数量。
> 5. 使用注意：ask_object_layer函数可能不再适用，因为获取对象显示属性的函数可能已经改变。
> 6. 总结：这段代码实现了在NX部件中获取默认部件列表对象的功能，包括兼容性问题，并提供了两种不同参数的函数实现。代码注释详细，有助于理解代码的功能和用法。
>
