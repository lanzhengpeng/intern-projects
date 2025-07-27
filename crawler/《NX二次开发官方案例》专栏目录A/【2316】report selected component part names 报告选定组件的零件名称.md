### 【2316】report selected component part names 报告选定组件的零件名称

#### 代码

```cpp
    /*HEAD REPORT_SELECTED_COMPONENT_PART_NAMES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_assem.h>  
    #include <uf_part.h>  
    #include <uf_object_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本中新增的功能。文档中提到UF_print_syslog是V18版本中新增的。 */  
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
    static int mask_for_components(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_component_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static int select_components(char *prompt, tag_t **comps)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        tag_t  
            *objs;  
        UF_CALL(UF_UI_select_with_class_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_components, NULL, &resp,  
            &cnt, comps));  
        objs = *comps;  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight(objs[ii], FALSE));  
        return cnt;  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n;  
        tag_t  
            *comps,  
            part;  
        char  
            part_fspec[MAX_FSPEC_SIZE+3];  
        if ((n = select_components("Select components", &comps)) > 0)  
        {  
            UF_CALL(UF_UI_open_listing_window());  
            for (ii = 0; ii < n; ii++)  
            {  
                part = UF_ASSEM_ask_prototype_of_occ(comps[ii]);  
                UF_PART_ask_part_name(part, part_fspec);  
                strcat(part_fspec, "\n");  
                UF_CALL(UF_UI_write_listing_window(part_fspec));  
            }  
            UF_free(comps);  
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

> 这段代码是一个NX的二次开发示例，其主要功能是选择组件并报告所选组件的名称。
>
> 代码的主要步骤包括：
>
> 1. 包含必要的NX API头文件。
> 2. 定义了一个错误报告函数report_error，用于输出错误信息。
> 3. 定义了一个函数mask_for_components，用于设置选择组件时的过滤条件。
> 4. 定义了一个函数select_components，用于使用对话框选择组件，并返回选中的组件数组。
> 5. 定义了一个函数do_it，用于执行选择组件并报告名称的逻辑。
> 6. 在ufusr函数中初始化NX，调用do_it函数，然后终止NX。
> 7. 定义了一个函数ufusr_ask_unload，用于卸载用户函数。
>
> 总体来说，这段代码通过NX的UI和装配体相关API实现了选择组件并报告组件名称的功能，是一个典型的NX二次开发示例。
>
