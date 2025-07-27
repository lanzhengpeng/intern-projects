### 【0774】create multiple sews 创建多个缝合

#### 代码

```cpp
    /*HEAD CREATE_MULTIPLE_SEWS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_modl.h>  
    #include <uf_disp.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，UF_print_syslog是V18版本新增的函数。

因此，我的回答是：UF_print_syslog是V18版本新增的函数。 */  
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
    static int filter_body_type(tag_t object, int type[3], void *data,  
        UF_UI_selection_p_t select)  
    {  
        int  
            body_type;  
        UF_CALL(UF_MODL_ask_body_type(object, &body_type));  
        if (body_type == *(int *)data)  
            return (UF_UI_SEL_ACCEPT);  
        else  
            return (UF_UI_SEL_REJECT);  
    }  
    static int mask_body_type( UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_BODY };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)) &&  
            !UF_CALL(UF_UI_set_sel_procs(select, filter_body_type, NULL, type)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_sheet(char *prompt)  
    {  
        double  
            cp[3];  
        int  
            sheet = UF_MODL_SHEET_BODY,  
            resp;  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog( prompt, "",  
            UF_UI_SEL_SCOPE_WORK_PART, mask_body_type, &sheet, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static int select_sheets(char *prompt, tag_t **sheets)  
    {  
        int  
            ii,  
            n,  
            sheet = UF_MODL_SHEET_BODY,  
            resp;  
        tag_t  
            *objs;  
        UF_CALL(UF_UI_select_with_class_dialog( prompt, "",  
            UF_UI_SEL_SCOPE_WORK_PART, mask_body_type, &sheet, &resp, &n, sheets));  
        objs = *sheets;  
        for (ii = 0; ii < n; ii++)  
            UF_CALL(UF_DISP_set_highlight(objs[ii], FALSE));  
        return n;  
    }  
    static void report_tag_type_and_subtype(char *name, tag_t object)  
    {  
        int  
            subtype,  
            type;  
        if (object != NULL_TAG)  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(object, &type, &subtype));  
            printf("%s = %d, type = %d, subtype = %d\n", name,object,type,subtype);  
        }  
        else printf("%s = %d (NULL_TAG)\n", name, object);  
    }  
    static void report_list_contents(char *name, uf_list_p_t list)  
    {  
        int  
            ii,  
            n;  
        tag_t  
            item;  
        char  
            item_name[15];  
        UF_CALL(UF_MODL_ask_list_count(list, &n));  
        printf("%s has %d items\n", name, n);  
        for (ii = 0; ii < n; ii++)  
        {  
            sprintf(item_name, "item %d", ii);  
            UF_CALL(UF_MODL_ask_list_item(list, ii, &item));  
            report_tag_type_and_subtype(item_name, item);  
        }  
    }  
    static void do_it(void)  
    {  
        int  
            n_tools;  
        tag_t  
            target,  
            *tools;  
        double  
            tol;  
        uf_list_p_t  
            disjoint_list,  
            sew_list;  
        UF_MODL_ask_distance_tolerance(&tol);  
        while (((target = select_a_sheet("Select target sheet")) != NULL_TAG) &&  
                (n_tools = select_sheets("Select sheets to sew", &tools)) > 0)  
        {  
            UF_CALL(UF_MODL_create_multiple_sews(target, n_tools, tools, tol,  
                &disjoint_list, &sew_list));  
            if (disjoint_list)  
            {  
                report_list_contents("disjoint_list", disjoint_list);  
                UF_MODL_delete_list(&disjoint_list);  
            }  
            if (sew_list)  
            {  
                report_list_contents("sew_list", sew_list);  
                UF_MODL_delete_list(&sew_list);  
            }  
            UF_free(tools);  
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

> 这段代码是一个NX的二次开发示例，主要实现了以下功能：
>
> 1. 错误报告函数：定义了一个错误报告函数report_error，用于在函数调用失败时打印错误信息。
> 2. 选择过滤函数：定义了一个选择过滤函数filter_body_type，用于在选择对话框中选择特定类型的实体。
> 3. 选择蒙版设置函数：定义了一个选择蒙版设置函数mask_body_type，用于设置选择蒙版以过滤选择。
> 4. 选择单个薄片实体函数：定义了一个选择单个薄片实体的函数select_a_sheet。
> 5. 选择多个薄片实体函数：定义了一个选择多个薄片实体的函数select_sheets。
> 6. 打印标签类型和子类型函数：定义了一个打印标签类型和子类型的函数report_tag_type_and_subtype。
> 7. 打印列表内容函数：定义了一个打印列表内容的函数report_list_contents。
> 8. 创建多重缝合并打印结果函数：定义了一个函数do_it，用于选择目标薄片和工具薄片，然后调用UF_MODL_create_multiple_sews函数创建多重缝合并打印结果。
> 9. 用户自定义函数：定义了一个用户自定义函数ufusr，用于初始化、执行主要功能，然后终止。
> 10. 卸载请求函数：定义了一个卸载请求函数ufusr_ask_unload。
>
> 通过这些函数，该代码实现了选择薄片实体，创建多重缝合并打印结果的功能，为NX的二次开发提供了示例。
>
