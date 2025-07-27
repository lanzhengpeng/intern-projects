### 【0617】create bplane 创建B平面

#### 代码

```cpp
    /*HEAD CREATE_BPLANE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_modl.h>  
    #include <uf_disp.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据注释，UF_print_syslog是V18版本中新增的功能。 */  
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
    static tag_t select_single_curve(char *prompt)  
    {  
        double  
            cp[3];  
        int  
            resp;  
        tag_t  
            curve,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog( prompt, "",  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_curves, NULL, &resp, &curve, cp,  
            &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
            return curve;  
        else  
            return NULL_TAG;  
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
            curves[++kk] = select_single_curve(prompt);  
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
        tag_t  
            bplane;  
        double  
            tol[3];  
        UF_STRING_t  
            s_section;  
        UF_MODL_ask_distance_tolerance(&tol[0]);  
        UF_MODL_ask_angle_tolerance(&tol[1]);  
        while (specify_strings("Bplane", &s_section))  
        {  
            UF_CALL(UF_MODL_create_bplane(&s_section, tol, &bplane));  
            UF_MODL_free_string_list(&s_section);  
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

> 这段代码是一个NX Open API二次开发示例，主要用于创建边界平面(bplane)。
>
> 主要步骤包括：
>
> 1. 定义了错误报告函数report_error，用于打印错误信息。
> 2. 定义了选择曲线的函数mask_for_curves，用于设置选择掩码。
> 3. 定义了选择单条曲线的函数select_single_curve，用于通过对话框选择一条曲线。
> 4. 定义了指定字符串的函数specify_strings，用于循环选择曲线，构建字符串结构体。
> 5. 定义了执行创建边界平面的函数do_it，其中循环调用specify_strings指定字符串，然后调用UF_MODL_create_bplane创建边界平面。
> 6. 定义了ufusr主函数，用于初始化并调用do_it函数。
> 7. 定义了卸载函数ufusr_ask_unload，用于卸载用户自定义函数。
>
> 总的来说，这段代码通过NX Open API实现了创建边界平面的功能，包括错误处理、选择曲线、创建边界平面等步骤。
>
