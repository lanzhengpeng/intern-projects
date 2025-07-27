### 【2467】rollback for edit 撤销编辑

#### 代码

```cpp
    static void suppress_a_feature(tag_t feature)  
    {  
        uf_list_p_t  
            feat_list;  
        UF_CALL(UF_MODL_create_list(&feat_list));  
        UF_CALL(UF_MODL_put_list_item(feat_list, feature));  
        UF_CALL(UF_MODL_suppress_feature(feat_list));  
        UF_CALL(UF_MODL_delete_list(&feat_list));  
    }  
    static UF_UNDO_mark_id_t rollback_for_edit(tag_t feat)  
    {  
        tag_t  
            next_feat = feat,  
            part = UF_PART_ask_display_part();  
        UF_UNDO_mark_id_t  
            mark_id;  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_invisible, NULL, &mark_id));  
        UF_CALL(UF_MODL_ask_previous_feature(feat, &next_feat));  
        if ((next_feat == NULL_TAG) || (next_feat == feat)) /* see PR 2041495 译:翻译：请查看PR 2041495，仅回复翻译内容，不要发表任何评论。 */  
        {  
            UF_CALL(UF_MODL_set_current_feature(feat));  
            suppress_a_feature(feat);  
        }  
        else  
            UF_CALL(UF_MODL_set_current_feature(next_feat));  
        return mark_id;  
    }

```

#### 代码解析

> 这段NX二次开发代码主要包含两个函数：
>
> 1. suppress_a_feature：用于抑制一个特征。它会创建一个特征列表，将传入的特征添加到列表中，然后调用UF_MODL_suppress_feature来抑制列表中的所有特征，最后删除特征列表。
> 2. rollback_for_edit：用于在编辑特征时记录一个撤销标记，以便后续可以撤销操作。它会获取当前特征和下一个特征，然后根据特征是否为最后一个来设置当前特征，并调用suppress_a_feature来抑制当前特征。在函数开始和结束时，会调用UF_UNDO_set_mark来记录撤销标记，并返回该标记的ID。
>
> 总体来说，这段代码实现了在NX中抑制特征，并在编辑特征时记录撤销标记的功能，以便于后续的撤销操作。
>
