### 【1157】edit selected id symbol 编辑选择的标识符号

#### 代码

```cpp
    /*HEAD EDIT_SELECTED_ID_SYMBOL CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_drf.h>  
    #include <uf_csys.h>  
    #include <uf_trns.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog是V18版本中的新功能。 */  
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
    static int mask_for_id_symbols(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_drafting_entity_type, UF_draft_id_symbol_subtype, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_an_id_symbol(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select an ID symbol", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_id_symbols, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
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
    static logical prompt_for_entity_site(char *prompt, int *selection)  
    {  
        int  
            resp;  
        char  
            options[9][38] = { "TOP LEFT",  
                               "TOP CENTER",  
                               "TOP RIGHT",  
                               "MID LEFT",  
                               "MID CENTER",  
                               "MID RIGHT",  
                               "BOTTOM LEFT",  
                               "BOTTOM CENTER",  
                               "BOTTOM RIGHT" };  
        resp = *selection;  
        resp = uc1603(prompt, resp, options, 9);  
        if ((resp > 4) && (resp < 19))  
        {  
            *selection = resp - 4;  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void map_point_from_matrix(tag_t matrix, double *pos)  
    {  
        int  
            irc;  
        double  
            abs_mx[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            csys[12] = { 0,0,0,0,0,0,0,0,0,0,0,0},  
            mx[12];  
        UF_CALL(UF_CSYS_ask_matrix_values(matrix, &csys[3]));  
        FTN(uf5940)(csys, abs_mx, mx, &irc);  
        FTN(uf5941)(pos, mx);  
    }  
    static void display_temporary_box(double llc[3], double urc[3], tag_t matrix)  
    {  
        double  
            ulc[3],  
            lrc[3];  
        UF_OBJ_disp_props_t  
            props = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        ulc[0] = llc[0];  
        ulc[1] = urc[1];  
        ulc[2] = llc[2];  
        lrc[0] = urc[0];  
        lrc[1] = llc[1];  
        lrc[2] = llc[2];  
        map_point_from_matrix(matrix, llc);  
        map_point_from_matrix(matrix, lrc);  
        map_point_from_matrix(matrix, urc);  
        map_point_from_matrix(matrix, ulc);  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            llc, lrc, &props));  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            lrc, urc, &props));  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            urc, ulc, &props));  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            ulc, llc, &props));  
    }  
    static void display_temporary_point(double *coords)  
    {  
        UF_OBJ_disp_props_t  
            attrib = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_DISP_display_temporary_point(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            coords, &attrib, UF_DISP_POINT));  
    }  
    static void display_notes_origin_and_text_box(tag_t an_id_symbol)  
    {  
        int  
            resp;  
        tag_t  
            matrix;  
        double  
            llc[3] = { 0,0,0 },  
            origin[3],  
            urc[3] = { 0,0,0 };  
        FTN(uf5552)(&an_id_symbol, origin, &resp);  
        display_temporary_point(origin);  
        FTN(uf5575)(&an_id_symbol, llc, urc, &resp);  
        UF_CALL(UF_CSYS_ask_matrix_of_object(an_id_symbol, &matrix));  
        display_temporary_box(llc, urc, matrix);  
    }  
    static void do_it(void)  
    {  
        int  
            mpi[100],  
            n,  
            resp;  
        tag_t  
            symbol;  
        double  
            origin[3],  
            mpr[70];  
        char  
            diam[27],  
            radius[27],  
            text[1][133] = { "" };  
        UF_DRF_draft_aid_text_info_t  
            *text_info;  
        while ((symbol = select_an_id_symbol("Edit")) != NULL_TAG)  
        {  
            display_notes_origin_and_text_box(symbol);  
            UF_CALL(UF_DRF_ask_object_preferences(symbol,mpi,mpr,radius,diam));  
            UF_CALL(UF_DRF_ask_draft_aid_text_info(symbol, &n, &text_info));  
            if ((n > 0) && (text_info[0].num_lines > 0))  
                strcpy(&text[0][0], text_info[0].text[0].string);  
            UF_CALL(UF_DRF_free_text(n, &text_info));  
            if ((prompt_for_entity_site("Pin it down - where?", &mpi[30])) &&  
                prompt_for_text("Enter new text", text[0]))  
            {  
                UF_CALL(UF_DRF_set_object_preferences(symbol,mpi,mpr,radius,diam));  
                FTN(uf5552)(&symbol, origin, &resp); /*  Work around PR 4415598 里海译:对不起，我无法直接提供翻译。如果你需要翻译某个文本，请提供原文，我会尽力为你翻译。 */  
                uc5566(symbol, 1, 1, text);  
                FTN(uf5553)(&symbol, origin, &resp); /* Work around PR 4415598 里海译:Work around PR 4415598

为PR 4415598找到变通方法

Work around PR 4415598

为PR 4415598找到变通方法 */  
                display_notes_origin_and_text_box(symbol);  
            }  
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

> 这段NX二次开发代码的主要功能是编辑选中ID符号的文本。它实现了以下功能：
>
> 1. 使用对话框让用户选择一个ID符号对象。
> 2. 在ID符号对象上显示文本框的边界框和原点。
> 3. 获取ID符号的文本信息。
> 4. 让用户选择文本框的位置，并输入新的文本内容。
> 5. 更新ID符号的文本信息。
> 6. 重新显示文本框的边界框和原点。
> 7. 使用错误处理函数，在出错时输出错误信息。
> 8. 提供卸载函数。
>
> 该代码利用了NX的UI、DRF、CSYS、DISP等模块，实现了与用户交互、获取和设置ID符号属性、显示辅助图形等功能。整体而言，它为用户提供了编辑ID符号文本的便利。
>
