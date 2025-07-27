### 【0889】create single offset chamfer on selected edge 在选中的边上创建单个偏移倒角

#### 代码

```cpp
    /*HEAD CREATE_SINGLE_OFFSET_CHAMFER_ON_SELECTED_EDGE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_modl.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:翻译如下：

注意：UF_print_syslog是V18版本新增的。 */  
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
    /* 里海 */  
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
    static tag_t select_an_edge(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select an edge", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_edges, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static logical prompt_for_a_number(char *prompt, char *item, double *number)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[1][16];  
        double  
            da[1];  
        strcpy(&menu[0][0], item);  
        da[0] = *number;  
        resp = uc1609(prompt, menu, 1, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number = da[0];  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            chamfer,  
            edge;  
        double  
            size = 0.25;  
        uf_list_p_t  
            edges;  
        char  
            offset[20];  
        while (((edge = select_an_edge("Chamfer")) != NULL_TAG) &&  
            prompt_for_a_number("Chamfer", "Offset", &size))  
        {  
            UF_CALL(UF_MODL_create_list(&edges));  
            UF_CALL(UF_MODL_put_list_item(edges, edge));  
            sprintf(offset, "%f", size);  
            UF_CALL(UF_MODL_create_chamfer(1, offset, "0", "0", edges, &chamfer));  
            UF_CALL(UF_MODL_delete_list(&edges));  
        }  
    }  
    /* 里海 */  
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

> 这段代码是一个NX Open的UF客户端程序，其主要功能是创建单边倒角特征。下面是对代码的详细解释：
>
> 1. 头文件包含：代码包含了必要的NX Open头文件，如uf.h、uf_ui.h等。
> 2. 错误报告函数：定义了一个名为report_error的函数，用于在发生UF函数调用错误时报告错误信息。
> 3. 选择边界的函数：定义了一个名为mask_for_edges的函数，用于在对话框中选择边界时设置选择掩码。
> 4. 选择边界：定义了一个名为select_an_edge的函数，用于打开对话框，让用户选择一个边界。
> 5. 提示输入数值：定义了一个名为prompt_for_a_number的函数，用于提示用户输入数值，并返回输入结果。
> 6. 执行函数：定义了一个名为do_it的函数，用于执行创建单边倒角的主要逻辑。
> 7. 主函数：定义了名为ufusr的主函数，用于初始化和执行do_it函数。
> 8. 卸载函数：定义了一个名为ufusr_ask_unload的函数，返回立即卸载标志，用于在卸载插件时调用。
>
> 总体来说，这段代码通过自定义的UF客户端程序，实现了在NX中创建单边倒角特征的功能，并采用了NX Open提供的UF函数来实现用户界面交互和几何建模操作。
>
