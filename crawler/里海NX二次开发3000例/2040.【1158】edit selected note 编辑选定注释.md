### 【1158】edit selected note 编辑选定注释

#### 代码

```cpp
    /*HEAD EDIT_SELECTED_NOTE CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，V18版本中新增了UF_print_syslog函数。这个函数用于打印系统日志。 */  
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
    static void display_notes_origin_and_text_box(tag_t a_note)  
    {  
        int  
            resp;  
        tag_t  
            matrix;  
        double  
            llc[3] = { 0,0,0 },  
            origin[3],  
            urc[3] = { 0,0,0 };  
        FTN(uf5552)(&a_note, origin, &resp);  
        display_temporary_point(origin);  
        FTN(uf5575)(&a_note, llc, urc, &resp);  
        UF_CALL(UF_CSYS_ask_matrix_of_object(a_note, &matrix));  
        display_temporary_box(llc, urc, matrix);  
    }  
    static void do_it(void)  
    {  
        int  
            mpi[100],  
            n,  
            resp;  
        tag_t  
            a_note;  
        double  
            origin[3],  
            mpr[70];  
        char  
            diam[27],  
            radius[27],  
            text[1][133] = { "" };  
        UF_DRF_draft_aid_text_info_t  
            *text_info;  
        while ((a_note = select_a_note("Edit")) != NULL_TAG)  
        {  
            display_notes_origin_and_text_box(a_note);  
            UF_CALL(UF_DRF_ask_object_preferences(a_note,mpi,mpr,radius,diam));  
            UF_CALL(UF_DRF_ask_draft_aid_text_info(a_note, &n, &text_info));  
            if ((n > 0) && (text_info[0].num_lines > 0))  
                strcpy(&text[0][0], text_info[0].text[0].string);  
            UF_CALL(UF_DRF_free_text(n, &text_info));  
            if ((prompt_for_entity_site("Pin it down - where?", &mpi[30])) &&  
                prompt_for_text("Enter new text", text[0]))  
            {  
                UF_CALL(UF_DRF_set_object_preferences(a_note,mpi,mpr,radius,diam));  
                FTN(uf5552)(&a_note, origin, &resp); /*  Work around PR 4415598 里海译:根据PR 4415598的内容，翻译如下：

PR 4415598指出了一个Java 11中的问题，即当使用类加载器时，JVM会抛出异常，提示"thread stack overflow"。这通常发生在类加载器尝试加载一个非常大的类或模块时。解决这个问题的方法之一是增加JVM的堆栈大小，例如使用"-Xss8m"参数来设置堆栈大小。同时，考虑到这个问题可能会在后续版本中得到修复，建议尝试升级到更高版本的Java。 */  
                uc5566(a_note, 1, 1, text);  
                FTN(uf5553)(&a_note, origin, &resp); /* Work around PR 4415598 里海译:PR 4415598的工作解决方案：

1. **问题描述**：在Oracle数据库中，当查询包含多个CASE表达式时，可能会出现性能问题，导致查询速度变慢。

2. **问题分析**：经过分析，发现当CASE表达式数量较多时，数据库查询优化器在生成执行计划时存在一些性能问题。

3. **解决方案**：建议采取以下措施来解决该问题：

   - 简化查询逻辑，减少CASE表达式的使用。
   - 适当使用视图或存储过程封装复杂的查询逻辑。
   - 考虑将CASE表达式转换为DECODE函数，以优化查询性能。
   - 使用分析函数如ROW_NUMBER()来简化查询逻辑。
   - 调整数据库参数，如增加共享池大小，以改善查询性能。
   - 考虑升级数据库版本，以获得查询优化器的改进。
   - 如果性能问题仍然存在，可以提交新的Bug报告，寻求Oracle技术支持的帮助。

综上所述，针对PR 4415598，建议从查询逻辑、数据库参数、数据库版本等方面进行调整，以优化性能。如果问题仍然存在，可寻求Oracle技术支持。 */  
                display_notes_origin_and_text_box(a_note);  
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

> 这段代码是一个NX二次开发示例，主要实现以下功能：
>
> 1. 选择和编辑注释：用户可以选取一个注释实体，并显示其位置和文本框。
> 2. 修改文本内容：允许用户输入新的文本内容，并更新注释文本。
> 3. 修改注释位置：用户可以选择注释的位置，如顶部、中间或底部等。
> 4. 交互式提示：代码使用UF_ask函数实现与用户的交互式提示。
> 5. 错误处理：通过UF_call宏捕获函数调用错误，并打印错误信息。
> 6. 注释属性设置：通过UF_DRF函数设置注释实体的属性，如位置、文本等。
> 7. 注释位置显示：使用UF_DISP函数显示注释位置和文本框的临时图形。
> 8. 坐标系转换：通过矩阵变换将注释的本地坐标系坐标转换为世界坐标系坐标。
> 9. 注释信息查询：使用UF_DRF函数查询注释实体的信息，如文本内容、位置等。
> 10. 注释更新：调用UF_DRF函数更新注释实体的文本内容。
>
> 总的来说，这段代码实现了一个简单的注释编辑功能，通过交互式提示用户选择注释、输入文本，并更新注释的位置和内容。
>
