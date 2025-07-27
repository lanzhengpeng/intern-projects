### 【1248】extract region 提取区域

#### 代码

```cpp
    /*HEAD EXTRACT_REGION CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_wave.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本新增的功能，用于打印系统日志。 */  
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
    /* qq3123197280 */  
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
    static tag_t select_a_face(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a face", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_faces, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static int select_faces(char *prompt, tag_t **faces)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        UF_CALL(UF_UI_select_with_class_dialog("Select faces", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_faces, NULL, &resp,  
            &cnt, faces));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*faces)[ii], FALSE));  
        return cnt;  
    }  
    static void do_it(void)  
    {  
        int  
            n_boundary_faces;  
        tag_t  
            *boundary_faces,  
            linked_feature,  
            seed_face;  
        if (((seed_face = select_a_face("Select seed face for region")) != NULL_TAG)  
            && ((n_boundary_faces = select_faces(  
                "Select boundary faces for region", &boundary_faces)) > 0))  
        {  
            UF_CALL(UF_WAVE_create_linked_region(1, &seed_face, n_boundary_faces,  
                boundary_faces, NULL_TAG, seed_face, TRUE, FALSE, FALSE,  
                &linked_feature));  
            UF_free(boundary_faces);  
        }  
    }  
    /* qq3123197280 */  
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

> 这段代码是一个NX Open C++二次开发程序，实现了以下功能：
>
> 1. 定义了一个错误报告函数report_error，用于打印UF函数调用失败的错误信息。
> 2. 定义了一个选择面时使用的遮罩函数mask_for_faces，用于限制选择为实体面。
> 3. 定义了一个选择单个面的函数select_a_face，使用单选对话框选择一个面作为种子面。
> 4. 定义了一个选择多个面的函数select_faces，使用多选对话框选择多个面作为边界面。
> 5. 定义了一个执行主体函数do_it，首先选择种子面，然后选择边界面，最后调用UF_WAVE_create_linked_region接口创建一个关联区域特征。
> 6. 定义了ufusr函数，在NX中作为程序的入口点，初始化并执行主体函数，最后终止NX Open。
> 7. 定义了ufusr_ask_unload函数，用于卸载程序。
>
> 该程序的主要逻辑是选择一个种子面，再选择一些边界面，然后创建一个关联区域特征。使用NX Open C++的UF函数进行实体选择和特征创建。
>
