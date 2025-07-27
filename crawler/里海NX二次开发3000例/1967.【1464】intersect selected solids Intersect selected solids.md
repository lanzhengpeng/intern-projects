### 【1464】intersect selected solids Intersect selected solids

#### 代码

```cpp
    /*HEAD INTERSECT_SELECTED_SOLIDS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本中新增的函数。

翻译：
UF_print_syslog是V18版本中新添加的函数。 */  
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
    static tag_t select_a_solid(char *prompt)  
    {  
        double  
            cp[3];  
        int  
            solid = UF_MODL_SOLID_BODY,  
            resp;  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog( prompt, "",  
            UF_UI_SEL_SCOPE_WORK_PART, mask_body_type, &solid, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        int  
            num_result;  
        tag_t  
            target,  
            tool,  
            *resulting_bodies;  
        while (((target = select_a_solid("Select target solid")) != NULL_TAG) &&  
               ((tool = select_a_solid("Select tool solid")) != NULL_TAG))  
        {  
            UF_CALL(UF_MODL_intersect_bodies(target, tool, &num_result,  
                &resulting_bodies));  
            UF_free(resulting_bodies);  
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

> 这段代码是NX二次开发代码，主要功能是使用UF_MODL_intersect_bodies函数来求两个实体的交集。
>
> 代码的主要逻辑如下：
>
> 1. 定义了错误报告函数report_error，用于打印UF函数调用失败的错误信息。
> 2. 定义了实体过滤函数filter_body_type，用于在实体选择时过滤出指定类型的实体。
> 3. 定义了设置实体过滤掩码的函数mask_body_type。
> 4. 定义了选择一个实体的函数select_a_solid，它会显示一个选择对话框，让用户选择一个实体，并返回实体标签。
> 5. 在主函数do_it中，循环调用select_a_solid函数，让用户选择目标实体和工具实体。然后调用UF_MODL_intersect_bodies函数求它们的交集，并释放结果实体数组。
> 6. 定义了ufusr函数作为NX的入口函数，初始化环境后调用do_it函数，最后终止环境。
> 7. 定义了ufusr_ask_unload函数，用于设置用户退出时是否立即卸载该代码。
>
> 总的来说，这段代码实现了一个简单的NX二次开发应用，让用户可以交互式地选择两个实体，然后计算它们的交集。
>
