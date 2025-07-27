### 【0544】copy paste selected features 复制粘贴选定特征

#### 代码

```cpp
    /*HEAD COPY_PASTE_SELECTED_FEATURES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_disp.h>  
    #include <uf_object_types.h>  
    #define ECHO(X) { UF_UI_open_listing_window(); \  
        UF_UI_write_listing_window(X); \  
        UF_print_syslog(X, FALSE); }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[UF_UI_MAX_STRING_LEN];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
            ECHO(msg);  
            ECHO(err);  
            ECHO("\n");  
            ECHO(call);  
            ECHO(";\n");  
        }  
        return(irc);  
    }  
    static tag_t select_a_feature(char *prompt)  
    {  
        int  
            cnt,  
            resp;  
        tag_t  
            first,  
            *feats;  
        UF_CALL(UF_UI_select_feature(prompt, NULL, &cnt, &feats, &resp));  
        if (cnt)  
        {  
            first = feats[0];  
            UF_free(feats);  
            return (first);  
        }  
        else return (NULL_TAG);  
    }  
    /* 里海 */  
    static int mask_add_faces_and_edges(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[2] = { { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_FACE },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_EDGE } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_ENABLE_SPECIFIC, 2, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_anything(char *prompt)  
    {  
        double  
            cp[3];  
        int  
            resp;  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_set_cursor_view(0));  
        UF_CALL(UF_UI_select_with_single_dialog( "Select an object", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_add_faces_and_edges, NULL, &resp,  
            &object, cp, &view));  
        UF_CALL(UF_UI_set_cursor_view(1));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static int allocate_memory(unsigned int nbytes, void **where)  
    {  
        int  
            resp;  
        *where = UF_allocate_memory(nbytes, &resp);  
        return resp;  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_parents;  
        tag_t  
            feat,  
            *new_feats,  
            *new_parents,  
            *parents;  
        char  
            **parent_names;  
        while ((feat = select_a_feature("Copy feature")) != NULL_TAG)  
        {  
            UF_CALL(UF_MODL_ask_references_of_features(&feat, 1, &parents,  
                &parent_names, &n_parents));  
            UF_CALL(allocate_memory(n_parents * sizeof(tag_t),  
                (void **)&new_parents));  
            for (ii = 0; ii < n_parents; ii++)  
            {  
                UF_CALL(UF_DISP_set_highlight(parents[ii], TRUE));  
                if ((new_parents[ii] = select_anything(parent_names[ii]))  
                    == NULL_TAG) new_parents[ii] = parents[ii];  
                UF_CALL(UF_DISP_set_highlight(parents[ii], FALSE));  
            }  
            if (!UF_CALL(UF_MODL_copy_paste_features(&feat, 1, parents,  
                new_parents, n_parents, 0, 0, &new_feats)))  
            {  
                UF_CALL(UF_MODL_update());  
                UF_free(new_feats);  
            }  
            if (n_parents > 0)  
            {  
                UF_free(parents);  
                UF_free(new_parents);  
                UF_free_string_array(n_parents, parent_names);  
            }  
        }  
    }  
    /* 里海 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        do_it();  
        UF_terminate();  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段代码是NX的二次开发示例，实现了选择一个特征，然后复制粘贴该特征到另一个位置的功能。
>
> 主要步骤如下：
>
> 1. 定义了一个宏ECHO，用于在列表窗口和日志中输出信息。
> 2. 定义了一个错误报告函数report_error，用于在调用UF函数出错时输出错误信息。
> 3. 定义了一个选择特征的函数select_a_feature，用于通过对话框让用户选择一个特征。
> 4. 定义了一个设置选择掩码的函数mask_add_faces_and_edges，允许用户选择面和边。
> 5. 定义了一个选择任意对象的函数select_anything，用户可以在视图中选择一个对象。
> 6. 定义了一个分配内存的函数allocate_memory，用于分配内存。
> 7. 定义了主函数do_it，实现了以下逻辑：循环让用户选择要复制的特征查询特征的所有父特征，并获取父特征的名字为父特征分配内存循环让用户选择每个父特征的替换目标，如果用户未选择，则使用原父特征调用UF_MODL_copy_paste_features函数复制特征，如果成功则更新模型释放内存
> 8. 循环让用户选择要复制的特征
> 9. 查询特征的所有父特征，并获取父特征的名字
> 10. 为父特征分配内存
> 11. 循环让用户选择每个父特征的替换目标，如果用户未选择，则使用原父特征
> 12. 调用UF_MODL_copy_paste_features函数复制特征，如果成功则更新模型
> 13. 释放内存
> 14. 定义了ufusr主函数，初始化后调用do_it，然后终止。
> 15. 定义了卸载函数ufusr_ask_unload，立即卸载用户函数。
>
> 整体上，这段代码通过对话框交互，实现了选择特征、选择位置、复制粘贴特征的功能，是NX二次开发中一个典型的示例。
>
