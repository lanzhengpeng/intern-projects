### 【0630】create chamfer 创建倒角

#### 代码

```cpp
    /*HEAD CREATE_CHAMFER CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_curve.h>  
    #include <uf_modl.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档，UF_print_syslog是V18版本新增的函数。 */  
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
    static logical ask_method(char *prompt, int *method)  
    {  
        int  
            resp = 0;  
        char  
            options[3][38] = { "Single Offset", "Double Offset", "Offset Angle"};  
        resp = uc1603(prompt, resp, options, 3);  
        *method = resp - 4;  
        return *method;  
    }  
    static int select_edges(char *prompt, tag_t **edges)  
    {  
        int  
            ii,  
            irc,  
            n;  
        UF_UI_selection_options_t  
            opts;  
        UF_UI_mask_t  
            mask;  
        opts.num_mask_triples = 1;  
        opts.mask_triples = &mask;  
        opts.scope = UF_UI_SEL_SCOPE_WORK_PART;  
        mask.object_type = UF_solid_type;  
        mask.object_subtype = 0;  
        mask.solid_type = UF_UI_SEL_FEATURE_ANY_EDGE;  
        UF_CALL(UF_UI_select_by_class(prompt, &opts, &irc, &n, edges));  
        for (ii=0; ii<n; ii++)  
            UF_CALL(UF_DISP_set_highlight(*(*edges + ii), FALSE));  
        return n;  
    }  
    static logical prompt_for_a_number(char *prompt, char *item, char *number)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[1][16];  
        double  
            value[1];  
        strcpy(&menu[0][0], item);  
        sscanf(number, "%f", &value);  
        resp = uc1609(prompt, menu, 1, value, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            sprintf(number, "%f", value[0]);  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            method,  
            num_edges;  
        char  
            offst1[20] = { "0.000" },  
            offst2[20] = { "0.000"},  
            theta[20] = { "0.000" };  
        tag_t  
            feat_id,  
            *edges;  
        uf_list_p_t  
            edge_list;  
        while (ask_method( "Choose Chamfer Method", &method) && method > 0)  
        {  
            num_edges = select_edges("select edges to blend", &edges);  
            UF_CALL(UF_MODL_create_list(&edge_list));  
            for(ii = 0; ii < num_edges; ii ++)  
            {  
                UF_CALL(UF_MODL_put_list_item(edge_list, edges[ii]));  
            }  
            prompt_for_a_number( "Enter Parameter", "First Offset" , offst1 );  
            if(method==2)  
                prompt_for_a_number( "Enter Parameter", "Second Offset" , offst2);  
            if(method==3)  
                prompt_for_a_number( "Enter Parameter", "Offset Angle" , theta);  
            UF_CALL(UF_MODL_create_chamfer( method, offst1, offst2, theta,  
                edge_list, &feat_id));  
            UF_CALL(UF_MODL_delete_list(&edge_list));  
        }  
    }  
    /* 里海 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (!UF_CALL(UF_initialize()))  
        {  
            do_it();  
            UF_CALL(UF_terminate());  
        }  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段代码是一个用于NX的二次开发代码示例，主要实现了以下功能：
>
> 1. 错误报告函数：使用宏定义UF_CALL来捕获函数调用返回的错误码，并报告错误信息。
> 2. 询问功能：使用ask_method函数来询问用户选择倒角方法，包括单边、双边和角度。
> 3. 选择边线：使用select_edges函数让用户选择要倒角的边线。
> 4. 输入参数：使用prompt_for_a_number函数让用户输入倒角参数。
> 5. 创建倒角特征：使用UF_MODL_create_chamfer函数根据用户输入的参数创建倒角特征。
> 6. NX环境初始化：在ufusr函数中初始化和终止NX环境，并调用do_it函数实现主体功能。
> 7. 卸载要求：通过ufusr_ask_unload函数要求立即卸载用户自定义函数。
> 8. 辅助函数：使用UF_MODL_create_list和UF_MODL_delete_list来管理边线列表。
>
> 总的来说，这段代码通过交互式方式让用户选择倒角方法、边线，并输入参数，最终创建NX的倒角特征。它提供了NX二次开发的一个典型示例，演示了NX API的使用方式。
>
