### 【0659】create curve mesh sheet 创建曲线网格片

#### 代码

```cpp
    /*HEAD CREATE_CURVE_MESH_THRU_SELECTED_CURVES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_modl.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据提供的信息，UF_print_syslog是在V18版本中新增的。在翻译中，只需回答UF_print_syslog是V18版本新增的功能即可，无需添加其他无关内容。 */  
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
    static int mask_for_curves(UF_UI_selection_p_t select, void *user_data)  
    {  
        UF_UI_mask_t  
            mask[5] = { UF_line_type, 0, 0,  
                        UF_circle_type, 0, 0,  
                        UF_conic_type, 0, UF_all_subtype,  
                        UF_spline_type, 0, 0,  
                        UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_EDGE };  
        if (UF_CALL(UF_UI_set_sel_mask(select,  
            UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 5, mask)))  
            return (UF_UI_SEL_FAILURE);  
        else  
            return (UF_UI_SEL_SUCCESS);  
    }  
    extern tag_t select_a_curve(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a curve", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_curves, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static logical specify_strings(char *which, UF_STRING_t *string)  
    {  
        int  
            ii = 0,  
            jj = 0,  
            kk = -1,  
            n,  
            n_segs[10];  
        char  
            prompt[81];  
        tag_t  
            curves[50];  
        do  
        {  
            if ((kk > -1) && (curves[kk] == NULL_TAG))  
            {  
                n_segs[ii] = jj-1;  
                ii++;  
                jj = 0;  
                kk--;  
            }  
            if (jj > 0)  
            {  
                sprintf(prompt, "OK to finish selecting string #%d", ii+1);  
                UF_CALL(UF_UI_set_status(prompt));  
            }  
            else if (ii > 0)  
            {  
                sprintf(prompt, "OK to finish selecting %s strings", which);  
                UF_CALL(UF_UI_set_status(prompt));  
            }  
            else  
                UF_CALL(UF_UI_set_status(""));  
            sprintf(prompt, "Select %s String #%d - curve #%d", which, ii+1, ++jj);  
            curves[++kk] = select_a_curve(prompt);  
        } while ((curves[kk] != NULL_TAG) || jj > 1);  
        UF_CALL(UF_UI_set_status(""));  
        if (kk > 0)  
        {  
            n_segs[ii] = jj;  
            n = ii;  
            UF_MODL_init_string_list(string);  
            UF_MODL_create_string_list(n, kk, string);  
            for (ii = 0; ii < n; ii++)  
            {  
                string->string[ii] = n_segs[ii];  
                string->dir[ii] = 1;  
            }  
            for (ii = 0; ii < kk; ii++)  
            {  
                string->id[ii] = curves[ii];  
                UF_CALL(UF_DISP_set_highlight(curves[ii], 0));  
            }  
            return TRUE;  
        }  
        return FALSE;  
    }  
    static void do_it(void)  
    {  
        int  
            c_flag[4] = { 0, 0, 0, 0 },  
            end_point = 0,  
            emphasis = 3,  
            body_type = 0,  
            spln_pt = 0;  
        double  
            tol[3] = { .001, 0.5, 0.02 };  
        tag_t  
            body_eid,  
            c_face[4] = { 0, 0, 0, 0 };  
        UF_STRING_t  
            cross,  
            primary,  
            spine;  
        specify_strings("Primary", &primary);  
        specify_strings("Cross", &cross);  
        UF_MODL_init_string_list(&spine);  
        UF_CALL(UF_MODL_create_curve_mesh(&primary, &cross, &spine, &end_point,  
            &emphasis,&body_type,&spln_pt,UF_NULLSIGN,tol,c_face,c_flag,&body_eid));  
        UF_MODL_free_string_list(&primary);  
        UF_MODL_free_string_list(&cross);  
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

> 这段代码是用于创建通过所选曲线的曲面网格的NX二次开发代码。主要功能包括：
>
> 1. 定义错误处理函数report_error，用于输出错误信息。
> 2. 定义曲线选择掩码函数mask_for_curves，用于设置选择掩码以允许选择各种类型的曲线。
> 3. 定义选择曲线函数select_a_curve，用于让用户选择一个曲线并返回其tag。
> 4. 定义字符串指定函数specify_strings，用于让用户指定主字符串和交叉字符串。
> 5. 定义创建曲面网格函数do_it，初始化字符串列表，让用户指定主字符串和交叉字符串，然后调用UF_MODL_create_curve_mesh函数创建曲面网格。
> 6. 定义ufusr函数，用于初始化NX环境，调用do_it函数，然后终止NX环境。
> 7. 定义卸载函数ufusr_ask_unload，用于立即卸载当前加载的库。
>
> 整体而言，这段代码实现了通过用户选择的主曲线和交叉曲线创建曲面网格的功能。
>
