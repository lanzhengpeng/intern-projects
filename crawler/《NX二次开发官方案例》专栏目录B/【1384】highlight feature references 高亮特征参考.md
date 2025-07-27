### 【1384】highlight feature references 高亮特征参考

#### 代码

```cpp
    static void highlight_feature_references(tag_t feat)  
    {  
        int  
            ii,  
            n_parents;  
        tag_t  
            body,  
            *parents;  
        char  
            **parent_names;  
        UF_CALL(UF_MODL_ask_references_of_features(&feat, 1, &parents,  
            &parent_names, &n_parents));  
        if (n_parents > 0)  
        {  
            suppress_from_feat(feat, &body);  
            for (ii = 0; ii < n_parents; ii++)  
            {  
                ECHO(ii);  
                ECHOS(parent_names[ii]);  
                UF_CALL(UF_DISP_set_highlight(parents[ii], TRUE));  
            }  
            uc1601("References highlighted", TRUE);  
            for (ii = 0; ii < n_parents; ii++)  
                UF_CALL(UF_DISP_set_highlight(parents[ii], FALSE));  
            unsuppress_from_feat(feat, body);  
            UF_free(parents);  
            UF_free_string_array(n_parents, parent_names);  
        }  
        else  
            uc1601("Feature has no references", TRUE);  
    }

```

#### 代码解析

> 这段代码是NX二次开发中用于高亮特征引用的功能。其功能是找出给定特征的所有引用，并高亮显示它们。
>
> 具体步骤如下：
>
> 1. 调用UF_MODL_ask_references_of_features函数，获取给定特征的所有引用及其父特征名。
> 2. 检查引用数量，如果大于0，则执行以下操作：调用suppress_from_feat函数，抑制特征，以避免选择错误。遍历所有引用，使用UF_DISP_set_highlight函数高亮显示每个引用。使用uc1601函数在状态栏显示提示信息。再次遍历所有引用，调用UF_DISP_set_highlight函数取消高亮显示。调用unsuppress_from_feat函数，取消特征的抑制。释放内存。
> 3. 调用suppress_from_feat函数，抑制特征，以避免选择错误。
> 4. 遍历所有引用，使用UF_DISP_set_highlight函数高亮显示每个引用。
> 5. 使用uc1601函数在状态栏显示提示信息。
> 6. 再次遍历所有引用，调用UF_DISP_set_highlight函数取消高亮显示。
> 7. 调用unsuppress_from_feat函数，取消特征的抑制。
> 8. 释放内存。
> 9. 如果引用数量为0，则在状态栏显示提示信息“Feature has no references”。
>
> 该函数通过抑制特征、高亮显示引用，然后取消抑制，实现了高亮特征引用的功能。使用状态栏提示信息提示用户当前操作状态。代码结构清晰，逻辑严谨。
>
