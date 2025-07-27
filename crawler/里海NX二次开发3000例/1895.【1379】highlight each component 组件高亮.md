### 【1379】highlight each component 组件高亮

#### 代码

```cpp
    /*HEAD HIGHLIGHT_EACH_COMPONENT CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_assem.h>  
    #include <uf_part.h>  
    #include <uf_disp.h>  
    #include <uf_cfi.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本新增的功能，用于打印系统日志。 */  
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
    static logical highlight_component(tag_t comp, void *user_data)  
    {  
        double  
            csys_matrix[9],  
            origin[3],  
            transform[4][4];  
        char  
            instance_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            part_name[MAX_FSPEC_SIZE+1],  
            refset_name[MAX_ENTITY_NAME_SIZE+1];  
        if (!UF_CALL(UF_ASSEM_ask_component_data(comp, part_name, refset_name,  
            instance_name, origin, csys_matrix, transform)))  
        {  
            UF_CALL(UF_DISP_set_highlight(comp, TRUE));  
            uc1601(instance_name, TRUE);  
            UF_CALL(UF_DISP_set_highlight(comp, FALSE));  
            return TRUE;  
        }  
        return FALSE;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            cset,  
            part = UF_PART_ask_display_part();  
        UF_CALL(UF_ASSEM_ask_all_comp_cset(part, &cset));  
        UF_CALL(UF_ASSEM_apply_to_cset(cset, highlight_component, NULL));  
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

> 这段NX二次开发代码的主要功能是高亮显示当前显示部件的所有组件。
>
> 代码首先包含所需的NX Open头文件，并定义了一个错误报告函数report_error，用于打印出调用NX API时的错误信息。
>
> 接着，定义了一个高亮组件的函数highlight_component。该函数接收组件的tag作为参数，通过NX API查询组件的名称和位置信息，然后设置该组件的高亮显示。
>
> 在主函数do_it中，首先获取当前显示的部件，然后获取该部件的所有组件集合，最后调用UF_ASSEM_apply_to_cset函数，遍历组件集合，并对每个组件调用highlight_component函数进行高亮显示。
>
> 最后，定义了ufusr函数作为程序的入口，在其中初始化NX，调用do_it函数执行高亮组件，最后结束NX。
>
> 总体来说，这段代码实现了对当前显示部件的所有组件进行高亮显示的功能，具有一定的二次开发参考价值。
>
