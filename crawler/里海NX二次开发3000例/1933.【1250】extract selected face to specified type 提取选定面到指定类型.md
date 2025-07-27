### 【1250】extract selected face to specified type 提取选定面到指定类型

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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog 是在 V18 版本中新增的。 */  
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
            *choice = resp - 4;  /* returns 1 , 2 or 3 里海译:根据文中的描述，函数会返回1、2或3这三个数值之一。 */  
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

> 这段代码是一个用于NX的二次开发UF程序，主要功能是从实体模型中选择一个面，并将其提取为新创建的曲面。以下是对代码的详细解释：
>
> 1. 函数报告错误：定义了report_error函数，用于在调用UF函数失败时打印错误信息。
> 2. 选择三个选项：定义了pick_one_of_three_choices函数，用于在三个选项中选择一个，并返回选择的索引。
> 3. 设置选择掩码：定义了mask_for_faces函数，用于设置选择掩码，以便在选择对话框中只显示面特征。
> 4. 选择一个面：定义了select_a_face函数，用于打开选择对话框，让用户选择一个面，并返回所选面的tag。
> 5. 提取面：定义了do_it函数，用于循环调用select_a_face和pick_one_of_three_choices，选择面并提取为新曲面。
> 6. UFusr函数：定义了ufusr函数，用于初始化UF模块，调用do_it函数执行主体功能，然后终止UF模块。
> 7. 卸载函数：定义了ufusr_ask_unload函数，用于在卸载UF程序时立即卸载。
> 8. 头文件包含：包含了必要的头文件，用于调用UF函数。
> 9. 错误处理宏：定义了UF_CALL宏，用于在UF函数调用前后添加错误处理。
>
> 综上所述，这段代码实现了在NX中选择一个面并提取为新曲面的功能，用户可以在选择界面中选择不同的提取类型。
>
