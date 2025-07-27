### 【2350】report selected note origin 报告所选注释的起源

#### 代码

```cpp
    /*HEAD REPORT_SELECTED_NOTE_ORIGIN CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_drf.h>  
    #include <uf_object_types.h>  
    #include <uf_csys.h>  
    #include <uf_trns.h>  
    #include <uf_vec.h>  
    #include <uf_assem.h>  
    #define ECHO(X) { UF_UI_open_listing_window(); \  
        UF_UI_write_listing_window(X); \  
        UF_print_syslog(X, FALSE); }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[UF_UI_MAX_STRING_LEN];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
            ECHO(msg);  
            ECHO(err);  
            ECHO("\n");  
            ECHO(call);  
            ECHO(";\n");  
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
    #define WRITE3F(X)  (write_double_array_to_listing_window(#X, X, 1, 3))  
    static void write_double_array_to_listing_window(char *name, double *array,  
            int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        char  
            blanks[UF_UI_MAX_STRING_LEN+1],  
            msg[UF_UI_MAX_STRING_LEN+1];  
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (ii == 0) sprintf(msg, "%s = ", name);  
            else sprintf(msg, "%s", blanks);  
            if (cols > 0) ECHO(msg);  
            for (jj = 0; jj < cols; jj++)  
            {  
                sprintf(msg, "%f", array[kk++]);  
                ECHO(msg);  
                if (jj < cols - 1)  
                {  
                    sprintf(msg, ", ");  
                    ECHO(msg);  
                }  
            }  
            if (cols > 0) ECHO("\n");  
        }  
    }  
    static void map_occ2assy(tag_t occ, double pos[3])  
    {  
        tag_t  
            old_work = UF_ASSEM_ask_work_part();  
        UF_CALL(UF_ASSEM_set_work_occurrence(UF_ASSEM_ask_part_occurrence(occ)));  
        UF_CALL(UF_CSYS_map_point(UF_CSYS_WORK_COORDS, pos, UF_CSYS_ROOT_COORDS,  
            pos));  
        UF_CALL(UF_ASSEM_set_work_part(old_work));  
    }  
    static void map_matrix2abs(tag_t input_matrix, double vec[3])  
    {  
        double  
            abs_mx[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            csys[12],  
            mx[12];  
        int  
            irc;  
        UF_CALL(UF_CSYS_ask_matrix_values(input_matrix, &csys[3]));  
        FTN(uf5940)(csys, abs_mx, mx, &irc);  
        UF_CALL(irc);  
        FTN(uf5941)(vec, mx);  
        UF_VEC3_sub(vec, csys, vec);  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_text;  
        tag_t  
            note,  
            matrix,  
            proto;  
        double  
            abs_origin[3],  
            wcs_origin[3];  
        UF_DRF_draft_aid_text_info_t  
            *text_info;  
        while ((note = select_a_note("Report origin")) != NULL_TAG)  
        {  
            proto = UF_ASSEM_ask_prototype_of_occ(note);  
            ECHO("\nObject Origin:\n");  
            UF_CALL(UF_DRF_ask_origin(proto, abs_origin));  
            ECHO("  ");  
            if (UF_ASSEM_is_occurrence(note)) map_occ2assy(note, abs_origin);  
            WRITE3F(abs_origin);  
            UF_CALL(UF_CSYS_map_point(UF_CSYS_WORK_COORDS, abs_origin,  
                UF_CSYS_ROOT_WCS_COORDS, wcs_origin));  
            ECHO("  ");  
            WRITE3F(wcs_origin);  
            ECHO("\nText Origin:\n");  
            UF_CALL(UF_DRF_ask_draft_aid_text_info(proto, &n_text, &text_info));  
            UF_CALL(UF_CSYS_ask_matrix_of_object(proto, &matrix));  
            for (ii = 0; ii < n_text; ii++)  
            {  
                UF_VEC3_copy(text_info[ii].origin, abs_origin);  
                map_matrix2abs(matrix, abs_origin);  
                ECHO("  ");  
                if (UF_ASSEM_is_occurrence(note)) map_occ2assy(note, abs_origin);  
                WRITE3F(abs_origin);  
                UF_CALL(UF_CSYS_map_point(UF_CSYS_WORK_COORDS, abs_origin,  
                    UF_CSYS_ROOT_WCS_COORDS, wcs_origin));  
                ECHO("  ");  
                WRITE3F(wcs_origin);  
            }  
            if (n_text > 0) UF_DRF_free_text(n_text, &text_info);  
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

> 这段代码的主要功能是查询和报告NX中注释的原始位置信息。
>
> 关键功能包括：
>
> 1. 通过对话框选择注释对象。
> 2. 查询注释对象的原型对象，并获取其绝对坐标系下的原点位置。
> 3. 如果注释对象是组件实例，需要将位置映射到组件坐标系。
> 4. 查询原型对象中的文本注释信息，并获取每个文本注释的原始位置。
> 5. 将文本注释的位置映射到绝对坐标系，并同样映射到组件坐标系。
> 6. 将查询到的位置信息输出到日志窗口。
>
> 通过以上功能，该代码实现了查询和报告NX中注释的原始位置信息。
>
