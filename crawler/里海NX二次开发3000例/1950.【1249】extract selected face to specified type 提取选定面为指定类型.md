### 【1249】extract selected face to specified type 提取选定面为指定类型

#### 代码

```cpp
    /*HEAD EXTRACT_SELECTED_FACE_TO_SPECIFIED_TYPE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog 是 V18 新增的函数。 */  
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
    static logical pick_one_of_three_choices(char *prompt, char *option_one,  
        char *option_two, char *option_three, int *choice)  
    {  
        int  
            resp;  
        char  
            options[3][38];  
        strncpy(&options[0][0], option_one, 37);  
        strncpy(&options[1][0], option_two, 37);  
        strncpy(&options[2][0], option_three, 37);  
        options[0][37] = '\0';  
        options[1][37] = '\0';  
        options[2][37] = '\0';  
        resp = uc1603(prompt, *choice, options, 3);  
        if ((resp > 4) && (resp < 19))  
        {  
            *choice = resp - 4;  /* returns 1 , 2 or 3 里海译:翻译returns 1, 2 or 3 */  
            return TRUE;  
        }  
        else return FALSE;  
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
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_faces, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            face,  
            sheet;  
        int  
            resp;  
        while (((face = select_a_face("Select face to extract")) != NULL_TAG) &&  
            (pick_one_of_three_choices("Type for New Extracted Face", "Same Type as Original",  
                 "Cubic Polynomial", "B-Surface", &resp)))  
            UF_CALL(UF_MODL_extract_face(face, resp - 1, &sheet));  
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

> 这段代码是一个NX Open的UF程序，其主要功能是从模型中选择一个面，并将其提取为新面，并可以选择新面的类型。
>
> 主要步骤包括：
>
> 1. 包含必要的头文件，并定义了一些错误处理宏。
> 2. 定义了一个错误报告函数report_error，用于输出错误信息。
> 3. 定义了一个选择函数pick_one_of_three_choices，用于从三个选项中选择一个。
> 4. 定义了一个面选择掩码函数mask_for_faces，用于只允许选择面。
> 5. 定义了一个选择一个面的函数select_a_face。
> 6. 定义了一个循环，用于选择一个面并提取为新面，直到用户取消选择。
> 7. 定义了UF程序入口函数ufusr，用于初始化和调用主要函数。
> 8. 定义了一个卸载函数ufusr_ask_unload，用于立即卸载UF程序。
>
> 总体来说，这段代码实现了选择面、提取面并设置面类型的功能，是一个典型的NX二次开发示例。
>
