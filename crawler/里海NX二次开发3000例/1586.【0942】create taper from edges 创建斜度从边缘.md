### 【0942】create taper from edges 创建斜度从边缘

#### 代码

```cpp
    /*HEAD CREATE_TAPER_FROM_EDGES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_modl.h>  
    #include <uf_disp.h>  
    #include <uf_so.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本新增的函数。 */  
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
    static int mask_for_edges(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_EDGE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static int select_edges(char *prompt, tag_t **edges)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        UF_CALL(UF_UI_select_with_class_dialog("Select edges", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_edges, NULL, &resp, &cnt, edges));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*edges)[ii], FALSE));  
        return cnt;  
    }  
    /* qq3123197280 */  
    static int mask_for_linear_objects(UF_UI_selection_p_t select,  
        void *type)  
    {  
        UF_UI_mask_t  
            mask[3] = { { UF_line_type, 0, 0},  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_LINEAR_EDGE },  
                        { UF_datum_axis_type, 0, 0 }};  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 3, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_linear_object(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select edge/datum axis", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_linear_objects,  
            NULL, &resp, &object, cp, &view));  
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
            n_edges;  
        tag_t  
            direction_tag,  
            *edges,  
            line,  
            taper;  
        while (((n_edges = select_edges("Taper From", &edges)) > 0) &&  
            ((line = select_a_linear_object("Taper direction")) != NULL_TAG))  
        {  
            UF_CALL(UF_SO_create_dirr_line(line, UF_SO_update_after_modeling,  
                line, FALSE, &direction_tag));  
            UF_CALL(UF_MODL_create_taper_from_edges(direction_tag, "10", FALSE,  
                n_edges, edges, &taper));  
            UF_free(edges);  
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

> 这段代码是用于NX CAD软件的二次开发，其主要功能是创建一个锥度特征。代码的流程如下：
>
> 1. 包含NX提供的头文件，定义了需要使用的函数和数据类型。
> 2. 定义了一个错误处理函数report_error，用于在函数调用失败时打印错误信息。
> 3. 定义了一个选择边界的函数select_edges，用于提示用户选择创建锥度特征所需的边界。
> 4. 定义了一个选择线性对象的函数select_a_linear_object，用于提示用户选择锥度特征的方向。
> 5. 定义了一个主函数do_it，循环执行以下步骤：调用select_edges选择边界调用select_a_linear_object选择方向根据边界和方向创建锥度特征
> 6. 调用select_edges选择边界
> 7. 调用select_a_linear_object选择方向
> 8. 根据边界和方向创建锥度特征
> 9. 定义了ufusr函数，用于初始化和调用do_it函数，然后清理资源。
> 10. 定义了ufusr_ask_unload函数，用于卸载当前加载的二次开发模块。
>
> 总体来说，这段代码实现了一个简单的锥度特征创建功能，利用NX提供的API实现边界选择、方向选择、特征创建等功能。代码结构清晰，实现了锥度特征创建的流程。
>
