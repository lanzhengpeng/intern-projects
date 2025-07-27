### 【1028】delete object attribute by title 删除对象属性按标题

#### 代码

```cpp
    /*HEAD DELETE_OBJECT_ATTRIBUTE_BY_TITLE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_disp.h>  
    #include <uf_cfi.h>  
    #include <uf_attr.h>  
    #include <uf_object_types.h>  
    #include <uf_assem.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本新增的函数，用于打印系统日志信息。 */  
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
    static int mask_add_faces_and_edges(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[2] = { { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_FACE },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_EDGE } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_ENABLE_SPECIFIC, 2, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_anything(char *prompt)  
    {  
        double  
            cp[3];  
        int  
            resp;  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_set_cursor_view(0));  
        UF_CALL(UF_UI_select_with_single_dialog( "Select an object", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_add_faces_and_edges, NULL, &resp,  
            &object, cp, &view));  
        UF_CALL(UF_UI_set_cursor_view(1));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void report_attributes(tag_t object)  
    {  
        int  
            indx = 0,  
            n;  
        char  
            dstr[21],  
            mstr[UF_UI_MAX_STRING_LEN+1],  
            title[UF_ATTR_MAX_TITLE_LEN+1],  
            tstr[21];  
        UF_ATTR_value_t  
            att;  
        UF_CALL(UF_UI_open_listing_window());  
        UF_CALL(UF_ATTR_count_attributes(object, UF_ATTR_any, &n));  
        sprintf(mstr, "tag %d has %d attributes\n", object, n);  
        UF_CALL(UF_UI_write_listing_window(mstr));  
        while (!UF_CALL(UF_ATTR_cycle(object, &indx, UF_ATTR_any, title, &att))  
            && (indx != 0))  
        {  
            switch ( att.type )  
            {  
                case UF_ATTR_integer:  
                    sprintf(mstr, "%s = %d (INTEGER)\n", title, att.value.integer);  
                break;  
                case UF_ATTR_real:  
                    sprintf(mstr, "%s = %f (FLOATING POINT)\n", title,  
                        att.value.real);  
                break;  
                case UF_ATTR_time:  
                    UF_CALL(uc4582(att.value.time, 2, dstr, tstr));  
                    sprintf(mstr, "%s = %s %s (DATE & TIME)\n", title, dstr, tstr);  
                break;  
                case UF_ATTR_null:  
                    sprintf(mstr, "%s = (NULL)\n", title);  
                break;  
                case UF_ATTR_string:  
                    sprintf(mstr, "%s = %s (STRING)\n", title, att.value.string);  
                    UF_free(att.value.string);  
                break;  
                case UF_ATTR_reference:  
                    sprintf(mstr, "%s = %s (REFERENCE)\n", title,  
                        att.value.reference);  
                    UF_free(att.value.reference);  
                break;  
                default:  
                break;  
            }  
            UF_CALL(UF_UI_write_listing_window(mstr));  
        }  
        UF_CALL(UF_UI_write_listing_window("\n"));  
    }  
    static logical attribute_exists(tag_t object, char *title)  
    {  
        int  
            type;  
        UF_CALL(UF_ATTR_find_attribute(object, UF_ATTR_any, title, &type));  
        if (type == 0) return FALSE;  
        return TRUE;  
    }  
    static logical prompt_for_text_item(char *prompt, char *item, char *text)  
    {  
        int  
            ia[1],  
            ip[1] = { 301 },  
            resp;  
        double  
            da[1];  
        char  
            ca[1][31],  
            menu[1][16];  
        strncpy(&menu[0][0], item, 15);  
        menu[0][16] = '\0';  
        strncpy(&ca[0][0], text, 30);  
        ca[0][31] = '\0';  
        resp = uc1613(prompt, menu, 1, ia, da, ca, ip);  
        if (resp == 3 || resp == 4)  
        {  
            if (!strcmp(&ca[0][0], text)) return FALSE;  
            strcpy(text, &ca[0][0]);  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            object,  
            proto;  
        char  
            title[UF_ATTR_MAX_TITLE_LEN + 1] = { "" };  
        while ((object = select_anything("Delete attributes")) != NULL_TAG)  
        {  
            if (UF_ASSEM_is_occurrence(object))  
                proto = UF_ASSEM_ask_prototype_of_occ(object);  
            else  
                proto = object;  
            UF_CALL(UF_DISP_set_highlight(object, TRUE));  
            report_attributes(proto);  
            while (prompt_for_text_item("Delete attribute", "Title", title))  
                if (attribute_exists(proto, title))  
                    UF_CALL(UF_ATTR_delete(proto, UF_ATTR_any, title));  
            report_attributes(proto);  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
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

> 这段代码是一个NX Open C++ API开发的ufusr函数，主要用于删除对象的属性。
>
> 主要功能包括：
>
> 1. 错误报告函数：定义了一个错误报告函数report_error，用于输出错误信息。
> 2. 选择任意对象：定义了一个函数select_anything，用于让用户选择一个对象。
> 3. 报告属性信息：定义了一个函数report_attributes，用于输出对象的所有属性信息。
> 4. 判断属性是否存在：定义了一个函数attribute_exists，用于判断对象是否存在某个属性。
> 5. 提示输入文本：定义了一个函数prompt_for_text_item，用于提示用户输入文本。
> 6. 删除属性：在函数do_it中，先选择一个对象，然后输出其所有属性，提示用户输入要删除的属性标题，并调用UF_ATTR_delete函数进行删除。
> 7. ufusr主函数：初始化NX Open C++ API，调用do_it函数执行删除属性操作，最后关闭NX Open C++ API。
> 8. 卸载函数：定义了一个卸载函数ufusr_ask_unload，返回立即卸载。
>
> 总体来说，这段代码实现了在NX中通过用户交互方式选择对象，并删除指定属性的功能。
>
