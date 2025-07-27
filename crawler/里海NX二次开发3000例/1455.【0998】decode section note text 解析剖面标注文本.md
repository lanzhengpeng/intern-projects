### 【0998】decode section note text 解析剖面标注文本

#### 代码

```cpp
    /*HEAD DECODE_SECTION_NOTE_TEXT CCC UFUN */  
    /*  
    When a section view is created, the section line will usually have a note at   
    each end with a single or double letter indicating the particular section view   
    to which it is associated.  
    If we pass the tag of one of those notes to UF_DRF_ask_draft_aid_text_info()   
    to get the text, instead of "A", we get a string back that looks like this:  
    <C2.0000><W!287@VWLETTER>  
    From this we can get the tag of the section view, and the actual text that   
    appears on the drawing.  
    In the example above, the "287" is the tag of the section view, and "VWLETTER"   
    is the object attribute of the section view tag, that contains the actual text   
    that is on the drawing.  
    There is a sample program below that yields the following output when one of   
    the letters at the end of a section line is selected at the prompt:  
    Note - line 0: <C2.0000><W!287@VWLETTER>  
    view = 287  
    view_name = SX@6  
    title = VWLETTER  
    attr_value = A  
    */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_attr.h>  
    #include <uf_drf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog 是 V18 版本新增的函数。文档中明确提到：

翻译：UF_print_syslog 是在 V18 版本中新增的函数。 */  
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
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %s\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
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
        UF_CALL(UF_UI_select_with_single_dialog("Select a note", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_notes, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void get_view_tag_and_name( char * n_str,   
                                      tag_t *view_tag, char *v_name )  
    {  
        char  
            *tmpstr;  
        tmpstr = strstr( n_str, "<W!" );  
        tmpstr += sizeof(char) * 3;  
        *view_tag = atoi( tmpstr );  
        UF_CALL(UF_OBJ_ask_name( *view_tag, v_name ));      
    }   
    static void get_attrib_title_and_value( tag_t view,char * n_str,   
                                              char *attr_title, char *att_value )  
    {  
        int  
            inx = 0;  
        char  
            *tmpstr,  
            ch,  
            str[UF_ATTR_MAX_STRING_LEN+1],  
            title_str[UF_ATTR_MAX_TITLE_LEN+1];  
         UF_ATTR_value_t  
             value;  
    /*  Reading a UF_ATTR_string so assign the character array to the pointer 里海译:翻译为：读取一个 UF_ATTR_string，因此将字符数组分配给指针。 */  
        value.value.string = str;  
        tmpstr = strstr( n_str, "@" );  
        tmpstr += sizeof(char);  
        for( inx = 0; inx < strlen(tmpstr); inx++ )  
        {  
            ch = tmpstr[inx];  
            if( ch == '>' ) title_str[inx] = '\0';  
            else title_str[inx] = tmpstr[inx];  
        }  
        strcpy( attr_title, title_str );  
        UF_CALL(UF_ATTR_read_value( view, attr_title, UF_ATTR_string, &value ));  
        strcpy( att_value, value.value.string );  
    }  
    static void do_it(void)  
    {  
        int  
            n_text = 0,  
            inx = 0;  
        tag_t  
            note = NULL_TAG,  
            view = NULL_TAG;  
        char  
            title[UF_ATTR_MAX_TITLE_LEN+1] = { "" },  
            attr_value[UF_ATTR_MAX_STRING_LEN+1],  
            note_str[132] = { "" },  
            view_name[UF_OBJ_NAME_LEN+1] = { "" };  
        UF_DRF_draft_aid_text_info_t  
            *text_info;  
        while( (note = select_a_note("Select Section Letter Note:")) != NULL_TAG )  
        {  
            UF_CALL(UF_DRF_ask_draft_aid_text_info(note, &n_text, &text_info));  
            if (n_text > 0)  
            {  
                for (inx = 0; (inx < n_text); inx++)  
                {  
                    if (strlen(text_info[inx].text[inx].string) > 0)  
                    {  
                        strcpy( note_str, text_info[inx].text[inx].string);  
                        WRITE_S( note_str );  
                        get_view_tag_and_name( note_str, &view, view_name );  
                        WRITE_D( view );  
                        WRITE_S( view_name );  
                        get_attrib_title_and_value( view, note_str,   
                                                    title, attr_value );  
                        WRITE_S( title );  
                        WRITE_S( attr_value );  
                    }   
                }   
                UF_CALL(UF_DRF_free_text(n_text, &text_info));  
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

> 根据代码注释和内容，这段代码主要用于获取NX中截面视图线末端的注释，并解析出与注释关联的视图信息。
>
> 主要步骤如下：
>
> 1. 定义错误处理函数report_error，用于在出错时打印错误信息。
> 2. 定义写入列表窗口的函数write_string_to_listing_window和write_integer_to_listing_window，用于打印字符串和整数信息。
> 3. 定义选择注释的函数select_a_note，使用对话框让用户选择一个注释对象。
> 4. 定义解析注释字符串的函数get_view_tag_and_name和get_attrib_title_and_value，用于提取视图标签、视图名称、属性标题和属性值。
> 5. 定义主函数do_it，循环让用户选择注释，解析注释字符串，提取视图信息，并打印。
> 6. 定义ufusr函数，用于初始化NX，调用主函数，然后终止NX。
> 7. 定义ufusr_ask_unload函数，返回立即卸载。
>
> 总体来说，这段代码利用NX的UF接口，实现了获取截面视图注释信息的功能。
>
