### 【3164】list group attributes 列出选中曲线所在的组的属性它会选择一个曲线，获取该曲线所在的组，然后列出组的所有属性

#### 代码

```cpp
    /*HEAD LIST_GROUP_ATTRIBUTES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_group.h>  
    #include <uf_assem.h>  
    #include <uf_attr.h>  
    #include <uf_cfi.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本中的新功能。 */  
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
    static tag_t select_a_curve(char *prompt)  
    {  
        double  
            cp[3];  
        int  
            resp;  
        tag_t  
            curve,  
            view;  
        UF_UI_selection_options_t  
            opts;  
        UF_UI_mask_t  
            mask[5];  
        opts.num_mask_triples = 5;  
        opts.mask_triples = mask;  
        opts.scope = UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY;  
        mask[0].object_type = UF_line_type;  
        mask[0].object_subtype = 0;  
        mask[0].solid_type = 0;  
        mask[1].object_type = UF_circle_type;  
        mask[1].object_subtype = 0;  
        mask[1].solid_type = 0;  
        mask[2].object_type = UF_conic_type;  
        mask[2].object_subtype = UF_all_subtype;  
        mask[2].solid_type = 0;  
        mask[3].object_type = UF_spline_type;  
        mask[3].object_subtype = 0;  
        mask[3].solid_type = 0;  
        mask[4].object_type = UF_point_type;  
        mask[4].object_subtype = 0;  
        mask[4].solid_type = 0;  
        UF_CALL(UF_UI_select_single(prompt, &opts, &resp, &curve, cp, &view));  
        if (resp == 4 || resp == 5)  
        {  
            UF_CALL(UF_DISP_set_highlight(curve, 0));  
            return curve;  
        }  
        else return NULL_TAG;  
    }  
    static void report_attributes(tag_t object)  
    {  
        int  
            indx = 0;  
        char  
            dstr[21],  
            mstr[UF_UI_MAX_STRING_LEN+1],  
            title[UF_ATTR_MAX_TITLE_LEN+1],  
            tstr[21];  
        UF_ATTR_value_t  
            att;  
        UF_CALL(UF_UI_open_listing_window());  
        while (!UF_CALL(UF_ATTR_cycle(object, &indx, UF_ATTR_any, title, &att))  
            && indx)  
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
    }  
    static void do_it(void)  
    {  
        tag_t  
            group,  
            object;  
        while ((object = select_a_curve("Select group member")) != NULL_TAG)  
        {  
            if (UF_ASSEM_is_occurrence(object))  
                object = UF_ASSEM_ask_prototype_of_occ(object);  
            UF_CALL(UF_GROUP_ask_group_of_tag(object, &group));  
            if (group) report_attributes(group);  
            else uc1601("Not a group member", TRUE);  
        }  
    }  
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

> 这段代码是用于NX的二次开发，其主要功能是选择一个图形对象，检查它是否属于某个组，如果是，则输出该组的属性信息。
>
> 主要功能如下：
>
> 1. 定义了错误报告函数report_error，用于在出错时输出错误信息和调用信息。
> 2. 定义了选择曲线函数select_a_curve，允许用户选择一个曲线对象，并返回该对象的tag。
> 3. 定义了报告属性函数report_attributes，用于输出一个对象的所有属性信息。
> 4. 定义了执行函数do_it，该函数循环调用select_a_curve选择对象，检查对象是否属于某个组，如果属于，则调用report_attributes输出该组的属性信息。
> 5. 定义了ufusr主函数，用于初始化NX，调用do_it执行操作，然后终止NX。
> 6. 定义了卸载函数ufusr_ask_unload，在卸载时调用。
>
> 总的来说，这段代码实现了选择曲线对象，检查是否属于某个组，并输出组的属性信息的功能，可用于二次开发中的相关功能。
>
