### 【0443】blend all edges of selected body 对选中的实体进行边缘融合

#### 代码

```cpp
    /*HEAD BLEND_ALL_EDGES_OF_SELECTED_BODY CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_disp.h>  
    #include <uf_object_types.h>  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[133];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，UF_print_syslog是V18版本新增的函数。 */  
            UF_print_syslog(msg, FALSE);  
            UF_print_syslog(err, FALSE);  
            UF_print_syslog("\n", FALSE);  
            UF_print_syslog(call, FALSE);  
            UF_print_syslog(";\n", FALSE);  
            if (!UF_UI_open_listing_window())  
            {  
                UF_UI_write_listing_window(msg);  
                UF_UI_write_listing_window(err);  
                UF_UI_write_listing_window("\n");  
                UF_UI_write_listing_window(call);  
                UF_UI_write_listing_window(";\n");  
            }  
        }  
        return(irc);  
    }  
    /* 里海 */  
    static int filter_body_type(tag_t object, int type[3], void *data,  
        UF_UI_selection_p_t select)  
    {  
        int  
            body_type;  
        UF_CALL(UF_MODL_ask_body_type(object, &body_type));  
        if (body_type == *(int *)data)  
            return (UF_UI_SEL_ACCEPT);  
        else  
            return (UF_UI_SEL_REJECT);  
    }  
    static int mask_body_type( UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_BODY };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)) &&  
            !UF_CALL(UF_UI_set_sel_procs(select, filter_body_type, NULL, type)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_solid(char *prompt)  
    {  
        double  
            cp[3];  
        int  
            solid = UF_MODL_SOLID_BODY,  
            resp;  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog( prompt, prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_body_type, &solid, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static logical prompt_for_text(char *prompt, char *text)  
    {  
        int  
            n,  
            resp;  
        resp = uc1600(prompt, text, &n);  
        if (resp == 3 || resp == 5) return TRUE;  
        return FALSE;  
    }  
    static void highlight_body_edges(tag_t body)  
    {  
        int  
            n_edges;  
        char  
            msg[133];  
        uf_list_p_t  
            ii,  
            edge_list;  
        UF_CALL(UF_MODL_ask_body_edges(body, &edge_list));  
        for (ii = edge_list; ii != NULL; ii = ii->next)  
            UF_CALL(UF_DISP_set_highlight(ii->eid, TRUE));  
        UF_CALL(UF_MODL_ask_list_count(edge_list, &n_edges));  
        sprintf(msg, "Body has %d edges", n_edges);  
        uc1601(msg, TRUE);  
        for (ii = edge_list; ii != NULL; ii = ii->next)  
            UF_CALL(UF_DISP_set_highlight(ii->eid, FALSE));  
        UF_CALL(UF_MODL_delete_list(&edge_list));  
    }  
    static void highlight_feat_edges(tag_t feat)  
    {  
        int  
            n_edges;  
        char  
            msg[133];  
        uf_list_p_t  
            ii,  
            edge_list;  
        UF_CALL(UF_MODL_ask_feat_edges(feat, &edge_list));  
        for (ii = edge_list; ii != NULL; ii = ii->next)  
            UF_CALL(UF_DISP_set_highlight(ii->eid, TRUE));  
        UF_CALL(UF_MODL_ask_list_count(edge_list, &n_edges));  
        sprintf(msg, "Feature has %d edges", n_edges);  
        uc1601(msg, TRUE);  
        for (ii = edge_list; ii != NULL; ii = ii->next)  
            UF_CALL(UF_DISP_set_highlight(ii->eid, FALSE));  
        UF_CALL(UF_MODL_delete_list(&edge_list));  
    }  
    static void do_it(void)  
    {  
        int  
            n_edges;  
        tag_t  
            body,  
            blend;  
        uf_list_p_t  
            edge_list;  
        char  
            msg[133],  
            radius[UF_MAX_EXP_LENGTH + 1] = { "0.25" };  
        while ((body = select_a_solid("Select solid to blend")) != NULL_TAG)  
        {  
            UF_CALL(UF_MODL_ask_body_edges(body, &edge_list));  
            UF_CALL(UF_MODL_ask_list_count(edge_list, &n_edges));  
            sprintf(msg, "Blending %d edges - Enter radius", n_edges);  
            if ((prompt_for_text(msg, radius)) &&  
                !UF_CALL(UF_MODL_create_blend(radius, edge_list, FALSE, FALSE,  
                    FALSE, 0.001, &blend)))  
            {  
                highlight_body_edges(body);  
                highlight_feat_edges(blend);  
            }  
            UF_CALL(UF_MODL_delete_list(&edge_list));  
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

> 这段代码是一个NX的二次开发示例，主要功能是选择一个实体，然后创建一个倒圆角特征，将实体所有边进行倒圆角处理。
>
> 主要步骤包括：
>
> 1. 包含NX的二次开发头文件，定义错误处理函数report_error。
> 2. 定义一个过滤函数filter_body_type，用于选择实体类型。
> 3. 定义一个函数select_a_solid，用于通过对话框选择一个实体。
> 4. 定义一个函数prompt_for_text，用于提示用户输入文本。
> 5. 定义一个函数highlight_body_edges，用于高亮显示实体的所有边。
> 6. 定义一个函数highlight_feat_edges，用于高亮显示特征的所有边。
> 7. 定义一个函数do_it，用于选择实体，创建倒圆角特征，并高亮显示。
> 8. 定义ufusr函数，用于初始化NX环境，调用do_it函数，然后终止NX环境。
> 9. 定义一个函数ufusr_ask_unload，用于卸载用户自定义函数。
>
> 总体来看，这段代码通过对话框选择实体，创建倒圆角特征，并高亮显示实体和特征的所有边。
>
