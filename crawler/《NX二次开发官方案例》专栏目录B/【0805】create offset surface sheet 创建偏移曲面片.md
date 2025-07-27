### 【0805】create offset surface sheet 创建偏移曲面片

#### 代码

```cpp
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_disp.h>  
    #include <uf_object_types.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsnprintf_s(msg, sizeof(msg), UF_UI_MAX_STRING_LEN, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133];  
            UF_get_fail_message(irc, err);  
            ECHO("*** ERROR code %d at line %d in %s:\n",  
                irc, line, file);  
            ECHO("+++ %s\n", err);  
            ECHO("%s;\n", call);  
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
    static logical prompt_for_text(char *prompt, char *text)  
    {  
        int  
            n,  
            resp;  
        resp = uc1600(prompt, text, &n);  
        if (resp == 3 || resp == 5) return TRUE;  
        return FALSE;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            face,  
            offset,  
            sheet;  
        char  
            dist[UF_MAX_EXP_LENGTH] = { "0.5" };  
        uf_list_p_t  
            feat_list;  
        while (((face = select_a_face("Select face to offset", "Offset surface"))  
            != NULL_TAG) && prompt_for_text("Offset Distance", dist))  
        {  
            UF_CALL(UF_MODL_extract_face(face, 0, &sheet));  
            UF_CALL(UF_MODL_ask_body_feats(sheet, &feat_list));  
            UF_CALL(UF_MODL_create_feature_offset(dist, feat_list, &offset));  
            UF_CALL(UF_MODL_delete_list(&feat_list));  
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

> 这段代码是NX的二次开发代码，主要实现了以下功能：
>
> 1. 定义了一个ECHO函数，用于打印信息到列表窗口和系统日志。
> 2. 定义了一个UF_CALL宏，用于调用NX的函数，并捕获错误。
> 3. 实现了一个mask_for_faces函数，用于设置选择过滤条件，只选择实体特征中的面。
> 4. 实现了一个select_a_face函数，用于弹出一个选择对话框，让用户选择一个面。
> 5. 实现了一个prompt_for_text函数，用于弹出一个提示框，让用户输入文本。
> 6. 实现了一个do_it函数，用于主流程。它会循环提示用户选择一个面，并输入偏移距离，然后创建偏移特征。
> 7. 实现了ufusr函数，作为程序的入口点，初始化NX环境，调用do_it函数，然后终止NX环境。
> 8. 实现了ufusr_ask_unload函数，用于卸载用户程序。
>
> 总的来说，这段代码实现了一个简单的NX二次开发程序，用于在用户选择的面上创建偏移特征。
>
