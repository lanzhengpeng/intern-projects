### 【0875】create sewn sheets 创建缝合的板片

#### 代码

```cpp
    /*HEAD CREATE_SEWN_SHEETS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_modl.h>  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[133];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n*** ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本中新增的功能，用于打印系统日志。 */  
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
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    #define WRITENZ(X) if (X != 0) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    /* qq3123197280 */  
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
        UF_CALL(UF_UI_select_with_single_dialog( "Select a sheet", prompt,  
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
        UF_CALL(UF_UI_select_with_class_dialog( "Select sheet bodies", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_body_type, &sheet, &resp, &n, sheets));  
        for (ii = 0; ii < n; ii++)  
            UF_CALL(UF_DISP_set_highlight((*sheets)[ii], FALSE));  
        return n;  
    }  
    static void report_feature_name_and_type(char *title, tag_t feat)  
    {  
        char  
            msg[MAX_LINE_SIZE+1],  
            *name,  
            *type;  
        if (!UF_CALL(UF_MODL_ask_feat_name(feat, &name)) &&  
            !UF_CALL(UF_MODL_ask_feat_type(feat, &type)))  
        {  
            UF_CALL(UF_UI_open_listing_window());  
            sprintf(msg, "%s = %d %s (%s)\n", title, feat, name, type);  
            UF_CALL(UF_UI_write_listing_window(msg));  
            UF_free(name);  
            UF_free(type);  
        }  
    }  
    static void do_it(void)  
    {  
        tag_t  
            target[1],  
            *tools,  
            sew_id;  
        int  
            n_disjoint,  
            num_tools,  
            SEW_GENERAL = 0,  
            SHEET = 0;  
        double  
            toler;  
        uf_list_p_t  
            disjoint_list;  
        UF_MODL_ask_distance_tolerance(&toler);  
        while( ((target[0] = select_a_sheet("Select target sheet")) != NULL_TAG) &&  
               ((num_tools = select_sheets("Select tool sheets", &tools)) > 0))  
        {  
            if (!UF_CALL(UF_MODL_create_sew(SEW_GENERAL, 1, target, num_tools,  
                tools, toler, SHEET, &disjoint_list, &sew_id)))  
            {  
                UF_CALL(UF_MODL_ask_list_count(disjoint_list, &n_disjoint));  
                WRITENZ(n_disjoint);  
                if (n_disjoint > 0) UF_MODL_delete_list(&disjoint_list);  
                report_feature_name_and_type("sew_id", sew_id);  
            }  
            UF_free(tools);  
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

> 这段代码是一个NX Open的UF程序，其主要功能是创建缝合面。
>
> 代码首先定义了一个错误报告函数report_error，用于在调用UF函数出错时输出错误信息。
>
> 接着定义了一个过滤函数filter_body_type，用于在图形选择时只选择面体。
>
> 然后定义了一个函数select_a_sheet，用于选择一个面体作为目标面。
>
> 接着定义了一个函数select_sheets，用于选择多个面体作为工具面。
>
> 再定义了一个函数report_feature_name_and_type，用于报告特征的名字和类型。
>
> 主函数do_it实现了主要逻辑：
>
> 1. 循环选择目标面和工具面。
> 2. 调用UF_MODL_create_sew创建缝合面。
> 3. 如果有不相交面，则删除不相交面列表。
> 4. 报告缝合特征的名字和类型。
>
> 最后，ufusr函数初始化UF系统，调用do_it函数，然后终止UF系统。
>
> 整体来说，这段代码通过选择目标面和工具面，创建缝合面特征，并报告特征信息，实现了缝合面的创建功能。
>
