### 【1704】open component of selected faceted body 打开选定多边形体的组件

#### 代码

```cpp
    /*HEAD OPEN_COMPONENT_OF_SELECTED_FACETED_BODY CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_facet.h>  
    #include <uf_assem.h>  
    #include <uf_part.h>  
    #include <uf_obj.h>  
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
    static int mask_for_faceted_bodies(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_faceted_model_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_faceted_body(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select faceted body", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_faceted_bodies, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void report_load_status(UF_PART_load_status_p_t status)  
    {  
        char  
            msg[133];  
        int  
            ii;  
        if (status->n_parts > 0)  
        {  
            ECHO("Load notes:\n");  
            for (ii = 0; ii < status->n_parts; ii++)  
            {  
                ECHO("  ");  
                ECHO(status->file_names[ii]);  
                ECHO(" - ");  
                UF_get_fail_message(status->statuses[ii], msg);  
                ECHO(msg);  
                ECHO("\n");  
            }  
            ECHO("\n");  
            UF_free(status->statuses);  
            UF_free_string_array(status->n_parts, status->file_names);  
        }  
    }  
    static void do_it(void)  
    {  
        tag_t  
            cset,  
            facet,  
            part = UF_PART_ask_display_part(),  
            solid;  
        UF_PART_load_status_t  
            status;  
        while ((facet = select_a_faceted_body("Load Component")) != NULL_TAG)  
        {  
            UF_CALL(UF_ASSEM_create_cset(part, "xOpen", &cset));  
            if (UF_ASSEM_is_occurrence(facet))  
            {  
                UF_CALL(UF_FACET_ask_solid_of_model(  
                    UF_ASSEM_ask_prototype_of_occ(facet), &solid));  
                UF_CALL(UF_ASSEM_add_to_cset(cset,  
                    UF_ASSEM_ask_part_occ_of_inst(  
                    UF_ASSEM_ask_part_occurrence(facet),  
                    UF_ASSEM_ask_inst_of_part_occ(  
                    UF_ASSEM_ask_part_occurrence(solid))),  
                    FALSE));  
            }  
            else  
            {  
                UF_CALL(UF_FACET_ask_solid_of_model(facet, &solid));  
                UF_CALL(UF_ASSEM_add_to_cset(cset,  
                    UF_ASSEM_ask_part_occurrence(solid), FALSE));  
            }  
            UF_CALL(UF_PART_open_cset(cset, &status));  
            report_load_status(&status);  
            UF_CALL(UF_OBJ_delete_object(cset));  
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

> 这段NX二次开发代码的功能如下：
>
> 1. 定义了一个错误报告函数report_error，用于在调用NX API失败时输出错误信息。
> 2. 定义了一个用于过滤Faceted Body的选择器mask_for_faceted_bodies。
> 3. 定义了一个用于选择Faceted Body的函数select_a_faceted_body，会弹出对话框让用户选择一个Faceted Body，并返回其Tag。
> 4. 定义了一个用于报告组件加载状态的函数report_load_status。
> 5. 定义了主函数do_it，其中：循环调用select_a_faceted_body选择Faceted Body为每个Faceted Body创建一个组件集cset将Faceted Body对应的Solid添加到cset中打开cset加载组件报告加载状态删除cset
> 6. 循环调用select_a_faceted_body选择Faceted Body
> 7. 为每个Faceted Body创建一个组件集cset
> 8. 将Faceted Body对应的Solid添加到cset中
> 9. 打开cset加载组件
> 10. 报告加载状态
> 11. 删除cset
> 12. 定义了ufusr函数，用于初始化NX API，调用do_it执行主体功能，然后终止NX API。
> 13. 定义了ufusr_ask_unload函数，返回立即卸载标志。
>
> 总体来说，该代码实现了选择Faceted Body，将其组件加载到NX中的功能。
>
