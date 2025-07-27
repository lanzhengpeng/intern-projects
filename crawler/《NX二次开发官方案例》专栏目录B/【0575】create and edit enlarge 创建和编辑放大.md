### 【0575】create and edit enlarge 创建和编辑放大

#### 代码

```cpp
    /*HEAD CREATE_AND_EDIT_ENLARGE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档，UF_print_syslog在V18版本中是新增的。 */  
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
    static logical prompt_for_four_numbers(char *prompt,  
        char *item1, char *item2, char *item3, char *item4,  
        double *number1, double *number2, double *number3, double *number4)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[4][16];  
        double  
            da[4];  
        strncpy(&menu[0][0], item1, 15);  
        strncpy(&menu[1][0], item2, 15);  
        strncpy(&menu[2][0], item3, 15);  
        strncpy(&menu[3][0], item4, 15);  
        da[0] = *number1;  
        da[1] = *number2;  
        da[2] = *number3;  
        da[3] = *number4;  
        resp = uc1609(prompt, menu, 4, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number1 = da[0];  
            *number2 = da[1];  
            *number3 = da[2];  
            *number4 = da[3];  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static int allocate_string_array(int n_elements, int n_chars, char ***where)  
    {  
        int  
            ii,  
            resp;  
        *where = UF_allocate_memory(n_elements * sizeof(char *), &resp);  
        for (ii = 0; ii < n_elements; ii++)  
        {  
            (*where)[ii] = UF_allocate_memory(n_chars * sizeof(char), &resp);  
            if (resp) return resp;  
        }  
        return 0;  
    }  
    static void strip_trailing_zeros(char *s)  
    {  
        int ii;  
        if (strchr(s, '.'))  
        {  
            for (ii = strlen(s) - 1; s[ii] == '0'; ii--) s[ii] = '\0';  
            if (s[ii] == '.') s[ii] = '\0';  
        }  
    }  
    static void do_it(void)  
    {  
        int  
            ii;  
        tag_t  
            face = NULL_TAG,  
            feat = NULL_TAG;  
        double  
            percents[4] = { 0,0,0,0 },  
            tol;  
        char  
            **edit_percent,  
            **percent_size;  
        UF_MODL_ask_distance_tolerance(&tol);  
        UF_CALL(allocate_string_array(4, UF_MAX_EXP_LENGTH, &percent_size));  
        UF_CALL(allocate_string_array(4, UF_MAX_EXP_LENGTH, &edit_percent));  
        if (((face = select_a_face("Enlarge")) != NULL_TAG) &&  
            prompt_for_four_numbers("Boundaries", "U-min", "U-max", "V-min",  
            "V-max", &percents[0], &percents[1], &percents[2], &percents[3]))  
        {  
            for (ii = 0; ii < 4; ii++)  
            {  
                sprintf(percent_size[ii], "%f", percents[ii]);  
                strip_trailing_zeros(percent_size[ii]);  
            }  
            UF_CALL(UF_MODL_create_enlarge(face, 0, percent_size, tol, &feat));   
            UF_free_string_array(4, percent_size);  
            prompt_for_four_numbers("New Boundaries", "U-min", "U-max", "V-min",  
                "V-max", &percents[0], &percents[1], &percents[2], &percents[3]);  
            for (ii = 0; ii < 4; ii++)  
            {  
                sprintf(edit_percent[ii], "%f", percents[ii]);  
                strip_trailing_zeros(edit_percent[ii]);  
            }  
            UF_CALL( UF_MODL_edit_enlarge(feat, 0, edit_percent, tol));  
            UF_MODL_update();  
            UF_free_string_array(4, edit_percent);  
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

> 这段代码是用于NX软件进行二次开发的UF函数示例。主要实现了以下功能：
>
> 1. 错误报告函数：定义了report_error函数，用于在调用UF函数失败时输出错误信息。
> 2. 面选择：定义了mask_for_faces和select_a_face函数，用于在NX中通过对话框选择一个面。
> 3. 参数输入：定义了prompt_for_four_numbers函数，用于在对话框中提示用户输入四个参数。
> 4. 字符串处理：定义了allocate_string_array和strip_trailing_zeros函数，用于处理字符串数组。
> 5. 面操作：定义了do_it函数，用于选择一个面，输入边界参数，创建和编辑扩面特征。
> 6. 初始化和终止：在ufusr函数中初始化和终止NX环境，并调用do_it函数执行主要功能。
> 7. 卸载请求：定义了ufusr_ask_unload函数，用于在卸载UF时返回立即卸载标志。
>
> 总体而言，这段代码实现了在NX中通过用户交互选择面、输入参数，并创建和编辑扩面特征的功能。
>
