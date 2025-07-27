### 【1533】list master model part of scenario part 列出场景部件的主模型部件

#### 代码

```cpp
    /*HEAD LIST_MASTER_MODEL_PART_OF_SCENARIO_PART CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_part.h>  
    #include <uf_assem.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档中的信息，UF_print_syslog是V18版本新增的功能。 */  
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
    static void do_it(void)  
    {  
        int  
            ii,  
            n_children;  
        tag_t  
            *children,  
            comp_part = NULL_TAG,  
            part = UF_ASSEM_ask_work_part(),  
            root_part_occ = UF_ASSEM_ask_root_part_occ(part);  
        char  
            scenario_name[MAX_FSPEC_SIZE+1],  
            master_name[MAX_FSPEC_SIZE+1],  
            msg[133];  
        UF_PART_ask_part_name(part, scenario_name);  
        n_children = UF_ASSEM_ask_part_occ_children(root_part_occ, &children);  
        for (ii = 0; ii < n_children; ii++)  
        {  
            comp_part = UF_ASSEM_ask_prototype_of_occ(children[ii]);  
            UF_PART_ask_part_name(comp_part, master_name);  
            sprintf(msg, "Scenario Part: %s\nMaster Model Part: %s\n",   
                scenario_name, master_name);  
        }  
        if (!UF_UI_open_listing_window())  
           UF_UI_write_listing_window(msg);  
        if (n_children > 0) UF_free(children);  
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

> 这段NX Open API代码用于获取装配体中的组件名称和根组件名称，并打印出来。
>
> 主要功能包括：
>
> 1. 定义了一个错误报告函数report_error，用于打印出函数调用失败的错误代码和消息。
> 2. 定义了一个do_it函数，用于获取当前工作部件，然后获取根组件的组件实例，循环遍历每个组件实例，获取其原型组件，并打印出根组件名称和原型组件名称。
> 3. 定义了ufusr函数作为NX的启动函数，调用UF_initialize初始化API，执行do_it函数，最后调用UF_terminate结束API。
> 4. 定义了ufusr_ask_unload函数，用于在卸载NX时立即卸载该程序。
> 5. 使用宏UF_CALL来简化错误处理，调用函数前加上UF_CALL，出错时会自动报告错误。
> 6. 使用了NX Open API的装配、部件、用户界面等模块的相关函数。
>
> 总体来说，这段代码通过NX Open API获取和打印装配体中组件的信息，采用了错误处理、宏定义、函数封装等编程技巧，是一个典型的NX二次开发示例。
>
