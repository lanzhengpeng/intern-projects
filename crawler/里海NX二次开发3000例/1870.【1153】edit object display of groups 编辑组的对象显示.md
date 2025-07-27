### 【1153】edit object display of groups 编辑组的对象显示

#### 代码

```cpp
    /*HEAD EDIT_OBJECT_DISPLAY_OF_GROUPS CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_group.h>  
    #include <uf_assem.h>  
    #include <uf_view.h>  
    #include <uf_curve.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据注释，UF_print_syslog是在V18版本中新增的函数。因此，翻译为：

注：UF_print_syslog是在V18版本中新增的函数，只需回答翻译，不要添加其他无关内容。 */  
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
    static void report_object_info(char *what, tag_t object)  
    {  
        int  
            status,  
            subtype,  
            type;  
        tag_t  
            owner;  
        char  
            name[UF_OBJ_NAME_LEN + 1];  
        UF_OBJ_disp_props_t  
            disp_props;  
        if (object != NULL_TAG)  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(object, &type, &subtype));  
            printf("%s = %d, type = %d, subtype = %d, ", what, object, type,  
                subtype);  
            if (UF_OBJ_is_object_a_promotion(object)) printf("promotion, ");  
            if (UF_ASSEM_is_occurrence(object)) printf("occurrence, ");  
            if (!UF_CALL(UF_OBJ_ask_owning_part(object, &owner)))  
                printf("owned by %d, ", owner);  
            if (!uc6409(object, &status, name) && (status == 1))  
                printf("view dependent in %s, ", name);  
            if (!UF_OBJ_ask_name(object, name)) printf("named %s, ", name);  
            status = UF_OBJ_ask_status(object);  
            printf("status %d\n", status);  
            UF_CALL(UF_OBJ_ask_display_properties(object, &disp_props));  
            printf("  layer = %d, color = %d, blank = %d, width = %d, "  
                "font = %d, highlight = %d\n", disp_props.layer,  
                disp_props.color, disp_props.blank_status, disp_props.line_width,  
                disp_props.font, disp_props.highlight_status);  
        }  
        else printf("%s = %d (NULL_TAG)\n", what, object);  
    }  
    static void report_group_info(tag_t group)  
    {  
        int  
            ii,  
            n;  
        char  
            name[MAX_ENTITY_NAME_SIZE + 1];  
        tag_t  
            *members;  
        report_object_info("group", group);  
        UF_CALL(UF_GROUP_ask_group_data(group, &members, &n));  
        for (ii = 0; ii < n; ii++)  
        {  
            sprintf(name, "members[%d]", ii);  
            report_object_info(name, members[ii]);  
        }  
        if (n > 0) UF_free(members);  
    }  
    static void edit_group_and_member_objects_display(tag_t group, int operation,  
        int new_value)  
    {  
        int  
            ii,  
            n;  
        tag_t  
            *members;  
        UF_CALL(UF_GROUP_ask_group_data(group, &members, &n));  
        switch (operation)  
        {  
            case 1:  
                UF_OBJ_set_layer(group, new_value);  
                for (ii = 0; ii < n; ii++)  
                    UF_OBJ_set_layer(members[ii], new_value);  
                break;  
            case 2:  
                UF_OBJ_set_color(group, new_value);  
                for (ii = 0; ii < n; ii++)  
                    UF_OBJ_set_color(members[ii], new_value);  
                break;  
            case 3:  
                UF_OBJ_set_blank_status(group, new_value);  
                for (ii = 0; ii < n; ii++)  
                    UF_OBJ_set_blank_status(members[ii], new_value);  
                break;  
            case 4:  
                UF_OBJ_set_line_width(group, new_value);  
                for (ii = 0; ii < n; ii++)  
                    UF_OBJ_set_line_width(members[ii], new_value);  
                break;  
            case 5:  
                UF_OBJ_set_font(group, new_value);  
                for (ii = 0; ii < n; ii++)  
                    UF_OBJ_set_font(members[ii], new_value);  
                break;  
            case 6:  
                UF_DISP_set_highlight(group, new_value);  
                for (ii = 0; ii < n; ii++)  
                    UF_DISP_set_highlight(members[ii], new_value);  
                break;  
            default:  
                break;  
        }  
        if (n > 0) UF_free(members);  
    }  
    static void do_it(void)  
    {  
        int  
            ii;  
        tag_t  
            group,  
            members[4];  
        double  
            pts[6] = { 0.0, 1.0, 2.0, 3.0, 4.0, 5.0 };  
        for (ii = 0; ii < 4; ii++)  
            UF_CALL(UF_CURVE_create_point(&pts[ii], &members[ii]));  
        UF_CALL(UF_GROUP_create_group(members, 4, &group));  
        report_group_info(group);  
        UF_OBJ_set_layer(group, 5);  
        UF_OBJ_set_color(group, UF_OBJ_MAGENTA);  
        UF_OBJ_set_blank_status(group, UF_OBJ_BLANKED);  
        UF_OBJ_set_line_width(group, UF_OBJ_WIDTH_THIN);  
        UF_OBJ_set_font(group, UF_OBJ_FONT_DOTTED);  
        UF_DISP_set_highlight(group, TRUE);  
        report_group_info(group);  
        edit_group_and_member_objects_display(group, 1, 25);  
        edit_group_and_member_objects_display(group, 2, UF_OBJ_YELLOW);  
        edit_group_and_member_objects_display(group, 3, UF_OBJ_BLANKED);  
        edit_group_and_member_objects_display(group, 4, UF_OBJ_WIDTH_THICK);  
        edit_group_and_member_objects_display(group, 5, UF_OBJ_FONT_DASHED);  
        edit_group_and_member_objects_display(group, 6, FALSE);  
        report_group_info(group);  
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

> 根据代码注释和功能，这是一个用于编辑NX组对象及其成员对象显示属性的示例代码。主要功能包括：
>
> 1. 创建4个点对象，并将它们创建成一个组。
> 2. 报告组对象的详细信息，包括成员对象的信息。
> 3. 更改组对象和所有成员对象的图层、颜色、显示状态、线宽、字体和突出显示等属性。
> 4. 报告更改属性后的组对象信息。
> 5. 定义了一个错误报告函数report_error，用于输出函数调用失败的信息。
> 6. 定义了一个报告对象信息的函数report_object_info，用于输出对象的详细信息。
> 7. 定义了一个报告组对象信息的函数report_group_info，用于输出组对象及其成员对象的详细信息。
> 8. 定义了一个编辑组对象及其成员对象显示属性的函数edit_group_and_member_objects_display。
> 9. 定义了一个主函数do_it，用于执行创建组、更改属性等操作。
> 10. 定义了ufusr和ufusr_ask_unload函数，这是NX二次开发必须的函数入口点。
>
> 总体来说，这段代码通过创建点对象、组对象，并设置它们的显示属性，演示了NX对象操作和属性设置的基本用法，对于学习NX二次开发具有一定的参考价值。
>
