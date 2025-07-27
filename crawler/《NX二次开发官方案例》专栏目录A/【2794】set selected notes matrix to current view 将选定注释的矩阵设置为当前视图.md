### 【2794】set selected notes matrix to current view 将选定注释的矩阵设置为当前视图

#### 代码

```cpp
    /*HEAD SET_SELECTED_NOTES_MATRIX_TO_CURRENT_VIEW CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_csys.h>  
    #include <uf_modl.h>  
    #include <uf_view.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本新增的功能。 */  
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
    static int mask_for_notes(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_drafting_entity_type, UF_draft_note_subtype, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_note(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_set_cursor_view(0));  
        UF_CALL(UF_UI_select_with_single_dialog("Select a note", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_notes, NULL, &resp,  
            &object, cp, &view));  
        UF_CALL(UF_UI_set_cursor_view(1));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            matobj,  
            note;  
        double  
            matrix[9];  
        while ((note = select_a_note("Orient to current view")) != NULL_TAG)  
        {  
            UF_CALL(uc6433("", matrix));  
            UF_CALL(UF_CSYS_create_matrix(matrix, &matobj));  
            UF_CALL(UF_CSYS_edit_matrix_of_object(note, matobj));  
            UF_CALL(UF_MODL_update());  
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

> 这段代码是一个NX的二次开发代码，主要用于将选定的注释旋转到当前视图。以下是代码的主要功能：
>
> 1. 错误报告函数：定义了一个错误报告函数report_error，用于在调用UF函数出错时输出错误信息。
> 2. 选择注释：定义了一个函数mask_for_notes，用于设置选择集过滤器，仅选择注释实体。然后定义了一个函数select_a_note，用于弹出选择对话框，让用户选择一个注释，并返回注释的tag。
> 3. 注释旋转：在do_it函数中，循环调用select_a_note函数，获取用户选择的注释tag。然后通过UF函数获取当前视图的变换矩阵，创建一个变换矩阵对象，并将注释对象与该变换矩阵关联，从而实现注释的旋转。
> 4. 用户函数：定义了ufusr和ufusr_ask_unload两个用户函数，分别在启动和退出时调用。
> 5. 错误处理：使用UF_CALL宏封装UF函数调用，并在出错时调用report_error函数输出错误信息。
> 6. 注释和交互：通过注释和交互式选择，使用户能够选择要旋转的注释，并实现注释的旋转。
>
> 总的来说，这段代码通过用户交互选择注释，并利用当前视图的变换矩阵来旋转注释，实现了注释与视图对齐的功能。
>
