### 【0806】create offset surface 创建偏移曲面

#### 代码

```cpp
    /*HEAD CREATE_OFFSET_SURFACE CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_disp.h>  
    #include <uf_object_types.h>  
    #include <uf_undo.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据提供的信息，UF_print_syslog是V18版本中新增的函数。 */  
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
    static tag_t select_a_face(char *prompt, char *title)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, title,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_faces, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void report_feature_name(tag_t feat)  
    {  
        char  
            *feature;  
        if (!UF_CALL(UF_MODL_ask_feat_name(feat, &feature)))  
        {  
            WRITE_S(feature);  
            UF_free(feature);  
        }  
    }  
    static void do_it(void)  
    {  
        tag_t  
            face,  
            offset;  
        uf_list_p_t  
            feat_list;  
        UF_UNDO_mark_id_t  
                mark;  
        while ((face = select_a_face("Select surface to offset", "Offset surface"))  
            != NULL_TAG)  
        {  
            UF_CALL(UF_MODL_ask_face_feats(face, &feat_list));  
            UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, "Offset Surface", &mark));  
            if (!UF_CALL(UF_MODL_create_feature_offset("2.0", feat_list, &offset)))  
                report_feature_name(offset);  
            else  
            {  
            /*  Work around for PR 5544670 译:对于PR 5544670的解决方案，请提供翻译。 */  
                UF_CALL(UF_UNDO_undo_to_mark(mark, NULL));  
                UF_CALL(UF_UNDO_delete_mark(mark, NULL));  
            }  
            UF_CALL(UF_MODL_delete_list(&feat_list));  
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

> 这段代码是NX的二次开发代码，主要实现了以下功能：
>
> 1. 定义了一个错误报告函数report_error，用于打印错误信息。
> 2. 定义了一个用于选择面的函数select_a_face，通过对话框让用户选择一个面，并返回面的tag。
> 3. 定义了一个写字符串到列表窗口的函数write_string_to_listing_window。
> 4. 定义了一个报告特征名称的函数report_feature_name。
> 5. 定义了一个主函数do_it，在循环中不断选择面，然后偏移该面，并报告偏移特征的名称。如果在偏移时出错，会撤销操作。
> 6. 定义了ufusr函数，用于初始化和执行do_it。
> 7. 定义了ufusr_ask_unload函数，用于卸载用户函数。
>
> 整体来说，这段代码实现了在NX中偏移面并报告偏移特征名称的功能。在编写时，使用了UF函数进行NX的二次开发，并且利用了错误处理和撤销功能来提高代码的健壮性。
>
