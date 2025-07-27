### 【3021】unite selected solids 统一选择的实体

#### 代码

```cpp
    /*HEAD UNITE_SELECTED_SOLIDS CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据提供的信息，可以得出以下结论：

1. UF_print_syslog 是在 V18 版本中新增的。

2. 对于这个新功能，需要回答关于它的译文，而不是其他无关内容。

3. UF_print_syslog 的具体用途和功能需要进一步了解。

4. 对于新功能，需要重点关注其新增的功能，以及如何使用和配置它。 */  
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
        tag_t  
            target,  
            tool;  
        while (((target = select_a_solid("Select target solid")) != NULL_TAG) &&  
               ((tool = select_a_solid("Select tool solid")) != NULL_TAG))  
            UF_CALL(UF_MODL_unite_bodies(target, tool));  
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

> 这段代码是一个NX Open C++的二次开发示例，主要实现了选择两个实体进行合并的功能。以下是代码的主要功能介绍：
>
> 1. 包含了必要的NX Open C++头文件，定义了UF_CALL宏用于错误报告，并实现了report_error函数用于打印错误信息。
> 2. 实现了filter_body_type函数，用于过滤选择实体类型。通过UF_MODL_ask_body_type函数获取实体的类型，与传入参数比较，返回是否接受该实体。
> 3. 实现了mask_body_type函数，设置选择过滤器，仅接受实体类型为UF_MODL_SOLID_BODY的实体。
> 4. 实现了select_a_solid函数，使用UF_UI_select_with_single_dialog函数弹出选择对话框，用户选择一个实体，并返回实体标签。
> 5. 实现了do_it函数，循环调用select_a_solid函数，让用户选择目标实体和工具实体，然后使用UF_MODL_unite_bodies函数合并这两个实体。
> 6. 定义了ufusr函数作为程序的入口，初始化UF库，调用do_it函数，完成实体合并功能，最后清理UF库。
> 7. 定义了ufusr_ask_unload函数，返回UF_UNLOAD_IMMEDIATELY表示卸载程序时立即清理资源。
>
> 总体来说，这段代码通过NX Open C++ API实现了实体选择和合并的功能，展示了NX二次开发的基本流程。
>
