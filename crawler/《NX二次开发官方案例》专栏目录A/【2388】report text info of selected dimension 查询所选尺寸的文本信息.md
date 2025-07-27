### 【2388】report text info of selected dimension 查询所选尺寸的文本信息

#### 代码

```cpp
    /*HEAD REPORT_DRAFT_AID_TEXT_INFO_OF_DIMENSION CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_drf.h>  
    #define ECHO(X)    (printf("%s = %d\n", #X, X))  
    #define ECHOF(X)   (printf("%s = %f\n", #X, X))  
    #define ECHO2F(X)  (printf("%s = %f, %f\n", #X, *X,*(X+1)))  
    #define ECHO3F(X)  (printf("%s = %f, %f, %f\n",#X,*(X),*(X+1),*(X+2)))  
    #define ECHO6F(X)  (printf("%s = %f, %f, %f, %f, %f, %f\n", #X, \  
                            *X,*(X+1),*(X+2),*(X+3),*(X+4),*(X+5)))  
    #define ECHOS(X)   (printf("%s = %s\n", #X, X))  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，V18版本中新增了UF_print_syslog函数。该函数用于将系统日志信息打印输出到控制台。 */  
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
    static void display_temporary_point(double *coords)  
    {  
        UF_OBJ_disp_props_t  
            attrib = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_DISP_display_temporary_point(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            coords, &attrib, UF_DISP_POINT));  
    }  
    /* qq3123197280 */  
    static int mask_for_dimensions(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_dimension_type, UF_all_subtype, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_dimension(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a dimension", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_dimensions, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void report_drafting_aid_text_info(tag_t draft_aid)  
    {  
        int  
            ii,  
            jj,  
            n_strings;  
        UF_DRF_draft_aid_text_info_t  
            *text_info;  
        UF_CALL(UF_DRF_ask_draft_aid_text_info(draft_aid, &n_strings, &text_info));  
        ECHO(n_strings);  
        for (ii = 0; ii < n_strings; ii++)  
        {  
            ECHO(ii);  
            ECHO(text_info[ii].text_type);  
            ECHO(text_info[ii].text_font);  
            ECHOF(text_info[ii].size);  
            ECHOF(text_info[ii].angle);  
            ECHO3F(text_info[ii].origin);  
            display_temporary_point(text_info[ii].origin);  
            ECHOF(text_info[ii].length);  
            ECHOF(text_info[ii].height);  
            ECHOF(text_info[ii].distance);  
            ECHOF(text_info[ii].aspect_ratio);  
            ECHOF(text_info[ii].gap);  
            ECHOF(text_info[ii].line_spacing);  
            ECHO(text_info[ii].num_lines);  
            for (jj=0; jj<text_info[ii].num_lines; jj++)  
            {  
                ECHO(jj);  
                ECHOS(text_info[ii].text[jj].string);  
            }  
        }  
        UF_CALL(UF_DRF_free_text(n_strings, &text_info));  
    }  
    static void do_it(void)  
    {  
        tag_t  
            draft_aid;  
        while ((draft_aid = select_a_dimension("Select Dimension object")) != NULL_TAG)  
        {  
            report_drafting_aid_text_info(draft_aid);  
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

> 根据代码内容，这是一段用于NX的二次开发代码，主要功能是获取和显示NX草图中标注的文本信息。以下是代码的主要功能和流程：
>
> 1. 头文件：代码包含了必要的NX头文件，用于进行二次开发。
> 2. 宏定义：定义了ECHO系列宏，用于打印变量值，以及UF_CALL宏，用于检查NX函数调用是否成功。
> 3. 错误处理函数：report_error函数用于处理NX函数调用失败时的错误信息。
> 4. 临时点显示函数：display_temporary_point用于在图形窗口中显示一个临时点。
> 5. 选择过滤器：mask_for_dimensions函数用于设置选择过滤器，仅允许选择标注类型的对象。
> 6. 选择标注对象：select_a_dimension函数用于弹出对话框，让用户选择一个标注对象。
> 7. 获取标注文本信息：report_drafting_aid_text_info函数用于获取指定标注对象中的文本信息，并打印出来。
> 8. 主函数：do_it函数中循环调用select_a_dimension和report_drafting_aid_text_info，实现连续选择多个标注对象并获取其文本信息。
> 9. UFusr函数：ufusr是NX二次开发的入口函数，在这里调用do_it函数。
> 10. 卸载函数：ufusr_ask_unload用于设置二次开发的卸载模式。
>
> 综上，该代码的主要功能是获取和显示NX草图中标注的文本信息，通过选择过滤器只允许选择标注对象，并获取标注中的文本信息并打印，实现了一个简单的NX标注文本信息查看器。
>
