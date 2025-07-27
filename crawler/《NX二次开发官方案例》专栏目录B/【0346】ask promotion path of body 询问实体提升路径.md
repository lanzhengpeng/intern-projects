### 【0346】ask promotion path of body 询问实体提升路径

#### 代码

```cpp
    /*HEAD ASK_PROMOTION_PATH_OF_BODY CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_assem.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档，UF_print_syslog是V18版本中新增的函数。 */  
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
    static int mask_for_bodies(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_BODY };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_body(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_bodies, NULL, &resp,  
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
        tag_t   proto, prom_feat, body;  
        tag_t   *inst_path;  
        int     ii, num_insts;  
        char     msg[133];  
        body = select_a_body("Select promoted body");  
        printf("\n\nBody tag: %d\n", body);  
        proto = UF_ASSEM_ask_prototype_of_occ( body);  
        UF_CALL( UF_MODL_ask_prom_feat_of_solid( proto, &prom_feat));  
        sprintf(msg, "Promotion feature id: %d\n", prom_feat);  
        if (!UF_UI_open_listing_window())  
            UF_UI_write_listing_window(msg);  
        UF_CALL( UF_MODL_ask_promotion_path( proto, TRUE, &inst_path, &num_insts));  
        printf("Number of instances: %d\n", num_insts);  
        for( ii = 0; ii < num_insts; ii++)  
        {  
            sprintf(msg, "Instance path: %d\n", inst_path[ii]);  
            UF_UI_write_listing_window(msg);  
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

> 这段代码是一个NX二次开发示例，主要实现了以下功能：
>
> 1. 错误报告函数：实现了report_error函数，用于在发生UF函数调用错误时打印错误信息到日志窗口。
> 2. 体选择函数：实现了mask_for_bodies函数，用于在对话框中只选择实体类型的体，并实现了select_a_body函数，用于弹出一个对话框让用户选择一个体。
> 3. 功能实现：do_it函数实现了主体功能。首先，它调用select_a_body函数获取用户选择的体，然后查询该体的原型，接着查询原型对应的推广特征，并获取推广路径上的所有实例。
> 4. 主函数：ufusr函数是二次开发模块的入口函数，初始化NX环境后调用do_it函数实现主体功能，最后关闭NX环境。
> 5. 卸载函数：ufusr_ask_unload函数用于在卸载模块时立即释放资源。
> 6. 注释说明：代码中使用了UF函数进行NX对象操作，并使用了UF UI函数来弹出对话框和打印信息到日志窗口。代码结构清晰，通过函数封装实现了功能模块化。
>
