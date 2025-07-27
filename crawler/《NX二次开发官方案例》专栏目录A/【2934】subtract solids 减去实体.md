### 【2934】subtract solids 减去实体

#### 代码

```cpp
    /*HEAD SUBTRACT_SOLIDS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_modl.h>  
    #define ECHO(X)    (printf("\t%s = %d\n", #X, X))  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档描述，UF_print_syslog 是一个新函数，在 V18 版本中首次引入。该函数用于打印系统日志。 */  
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
    #define MAX 10  
    static void do_it(void)  
    {  
        int  
            ii,  
            one = 1,  
            max = MAX,  
            n_res,  
            resp,  
            subtract = 2;  
        tag_t  
            results[MAX],  
            target,  
            tool;  
        char  
            name[MAX_ENTITY_NAME_SIZE+1];  
        while (((target = select_a_solid("Select target")) != NULL_TAG) &&  
               ((tool = select_a_solid("Select tool")) != NULL_TAG))  
        {  
            ECHO(target);  
            ECHO(tool);  
            FTN(uf6520)(&subtract, &target, &tool, &one, &max, &resp, &n_res,  
                results);  
            for (ii = 0; (ii < n_res) && (ii < max); ii++)  
            {  
                sprintf(name, "RES_%d", ii+1);  
                UF_CALL(UF_OBJ_set_name(results[ii], name));  
            }  
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

> 这段代码是NX的二次开发代码，主要实现了从目标体中减去工具体的功能。以下是主要步骤的介绍：
>
> 1. 头文件包含：引入了NX API相关的头文件。
> 2. 错误处理宏：定义了ECHO和UF_CALL两个宏，用于打印变量值和封装NX API调用。
> 3. 错误报告函数：实现了report_error函数，用于在API调用失败时打印错误信息。
> 4. 筛选函数：实现了filter_body_type和mask_body_type函数，用于在UI选择时筛选出特定类型的实体。
> 5. 选择函数：实现了select_a_solid函数，用于弹出选择对话框，让用户选择一个实体。
> 6. 执行函数：实现了do_it函数，用于循环让用户选择目标和工具实体，然后调用NX的UF_MODL_subtract_solids函数执行减去操作，并将结果重命名。
> 7. UFusr函数：NX二次开发程序的入口函数，初始化NX环境，调用do_it函数，然后终止NX环境。
> 8. 卸载函数：实现了ufusr_ask_unload函数，用于处理二次开发程序的卸载。
>
> 这段代码实现了简单的NX减去操作，通过选择目标和工具实体，并调用NX的API进行减去操作。错误处理和用户交互界面设计得比较完善。
>
