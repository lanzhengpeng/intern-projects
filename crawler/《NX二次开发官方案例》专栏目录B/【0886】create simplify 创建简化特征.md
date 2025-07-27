### 【0886】create simplify 创建简化特征

#### 代码

```cpp
    /*HEAD CREATE_SIMPLIFY CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_modl.h>  
    #define ECHO(X)    (printf("\t%s = %d\n", #X, X))  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档中的信息，UF_print_syslog是V18版本新增的功能。因此，翻译为：UF_print_syslog是V18版本新增的。 */  
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
    static int mask_for_faces(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_FACE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static int select_faces(char *prompt, tag_t **faces)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        tag_t  
            *objs;  
        UF_CALL(UF_UI_select_with_class_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_faces, NULL, &resp, &cnt, faces));  
        objs = *faces;  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight(objs[ii], FALSE));  
        return cnt;  
    }  
    static void do_it(void)  
    {  
        int  
            n_boundary_faces,  
            n_failing_wound_edges,  
            n_removed_faces,  
            n_retained_faces;  
        tag_t  
            *boundary_faces,  
            feature_tag,  
            *failing_wound_edges,  
            *retained_faces,  
            zero;  
        UF_MODL_simpl_data_t  
            simpl_parms;  
        UF_CALL(UF_MODL_create_exp_tag("zero=0.0", &zero));  
        simpl_parms.n_removed_faces = 0;  
        simpl_parms.removed_faces = NULL;  
        simpl_parms.n_boundary_edges = 0;  
        simpl_parms.boundary_edges = NULL;  
        simpl_parms.n_non_boundary_edges = 0;  
        simpl_parms.non_boundary_edges = NULL;  
        simpl_parms.n_imprint_features = 0;  
        simpl_parms.imprint_features = NULL;  
        simpl_parms.max_hole_dia_expression = zero;  
        while (((n_retained_faces = select_faces("Select faces to retain",  
                    &retained_faces)) > 0) &&  
               ((n_boundary_faces = select_faces("Select boundary faces",  
                    &boundary_faces)) > 0))  
        {  
            UF_UI_set_status("Creating simplify feature...");  
            ECHO(n_retained_faces);  
            simpl_parms.n_retained_faces = n_retained_faces;  
            simpl_parms.retained_faces = retained_faces;  
            ECHO(n_boundary_faces);  
            simpl_parms.n_boundary_faces = n_boundary_faces;  
            simpl_parms.boundary_faces = boundary_faces;  
            if (!UF_CALL(UF_MODL_create_simplify(&simpl_parms, &feature_tag,  
                &n_failing_wound_edges, &failing_wound_edges,  
                &n_retained_faces, &n_removed_faces)))  
            {  
                ECHO(n_failing_wound_edges);  
                ECHO(n_retained_faces);  
                ECHO(n_removed_faces);  
                if (n_failing_wound_edges > 0) UF_free(failing_wound_edges);  
            }  
            UF_free(retained_faces);  
            UF_free(boundary_faces);  
        }  
    }  
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

> 这是段用于创建NX简化特征的代码。
>
> 主要功能包括：
>
> 1. 提示用户选择需要保留的面和边界面。
> 2. 根据用户选择，创建简化特征。
> 3. 输出简化特征的参数，如保留面数量、移除面数量等。
> 4. 重复上述步骤，直到用户不再选择面为止。
> 5. 使用UF_print_syslog函数输出错误信息。
> 6. 使用UF_UI_set_sel_mask函数设置选择掩码，以便用户只能选择面。
> 7. 使用UF_MODL_create_simplify函数创建简化特征。
> 8. 使用UF_free函数释放内存。
> 9. 使用UF_print_syslog函数输出提示信息。
>
> 代码中包含的错误处理、内存管理、选择管理等都是NX二次开发的标准做法。
>
