### 【0645】create component and make work part 创建组件并设置工作部件

#### 代码

```cpp
    /*HEAD CREATE_COMPONENT_AND_MAKE_WORK_PART CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_assem.h>  
    #include <uf_modl.h>  
    #include <uf_part.h>  
    #include <uf_ui.h>  
    #include <uf_disp.h>  
    #define ECHO(X)    (printf("%s = %d\n", #X, X))  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，我翻译如下：

注意：UF_print_syslog 是 V18 新增的，只回答译文，不要废话。 */  
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
    static logical select_bodies(char *prompt, tag_p_t *bodies, int *cnt)  
    {  
        int  
            i,  
            resp;  
        UF_UI_selection_options_t  
            opts;  
        UF_UI_mask_t  
            mask;  
        opts.num_mask_triples = 1;  
        opts.mask_triples = &mask;  
        opts.scope = UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY;  
        mask.object_type = UF_solid_type;  
        mask.object_subtype = 0;  
        mask.solid_type = UF_UI_SEL_FEATURE_BODY;  
        UF_CALL(UF_UI_select_by_class(prompt, &opts, &resp, cnt, bodies));  
        if (resp == 3)  
        {  
            for (i=0; i<*cnt; i++)  
                UF_CALL(UF_DISP_set_highlight(*(*bodies+i),FALSE));  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        int  
            inches = 2,  
            n_objects;  
        double  
           origin[3] = {0.0, 0.0, 0.0},  
           csys[6] = {1.0, 0.0, 0.0, 0.0, 1.0, 0.0};  
        tag_t  
            part =  UF_PART_ask_display_part(),  
            *objects,  
            instance;  
        if (select_bodies("Select bodies to export", &objects, &n_objects))      
        {  
            UF_CALL(UF_ASSEM_create_component_part(part, "new_component.prt", NULL,   
                NULL, inches, 0, origin, csys, n_objects, objects, &instance));   
            UF_CALL(UF_ASSEM_set_work_occurrence(UF_ASSEM_ask_part_occ_of_inst  
                 (UF_ASSEM_ask_work_occurrence(), instance)));  
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

> 这段代码是一个NX的二次开发示例，主要实现了以下功能：
>
> 1. 包含了NX的UF API头文件，用于调用NX的函数。
> 2. 定义了一个错误报告函数report_error，用于打印出函数调用失败的错误信息。
> 3. 定义了一个选择体的函数select_bodies，可以弹出一个对话框让用户选择要操作的实体。
> 4. 定义了一个执行操作的函数do_it，用于创建组件并将工作部件设置为这个新组件。
> 5. 定义了ufusr函数，这是NX二次开发程序的入口函数。在该函数中，初始化NX环境，调用do_it函数执行操作，然后终止NX环境。
> 6. 定义了ufusr_ask_unload函数，用于设置程序卸载的方式。
>
> 总体来说，这段代码实现了一个简单的二次开发示例，允许用户选择实体，然后创建一个新的组件并将它设置为工作部件。
>
