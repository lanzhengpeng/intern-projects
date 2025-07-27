### 【0149】ask and set assembly options 询问和设置装配选项

#### 代码

```cpp
    /*HEAD ASK_AND_SET_ASSEMBLY_OPTIONS CCC UFUN */  
    /*  
       Call UF_ASSEM_ask_assem_options() to get the current load options.  
       Then, you can change one or more structure elements, and call  
       UF_ASSEM_set_assem_options().  
       Any structure elements that you don't reset will remain unchanged  
       when you call the set function.  
       You can build this as either an external or an internal program.  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_assem.h>  
    #include <uf_part.h>  
    #include <uf_obj.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog在V18中是新增的功能，该功能用于在系统日志中打印日志。 */  
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
        UF_ASSEM_options_t  
            assm_opts;  
        UF_CALL( UF_ASSEM_ask_assem_options( &assm_opts ));  
        assm_opts.load_options = UF_ASSEM_load_from_search_dirs;  
                                 /* choices:  
                                       UF_ASSEM_load_as_saved  
                                       UF_ASSEM_load_from_directory  
                                       UF_ASSEM_load_from_search_dirs  
                                    */  
        assm_opts.parts_list = UF_ASSEM_add_all_levels;  
                                 /* choices:  
                                        UF_ASSEM_do_not_add_components  
                                        UF_ASSEM_add_single_level  
                                        UF_ASSEM_add_all_levels  
                                    */  
        assm_opts.update = UF_ASSEM_update_report;  
                                 /* choices:  
                                        UF_ASSEM_no_update_report  
                                        UF_ASSEM_update_report  
                                    */  
        assm_opts.emphasize = UF_ASSEM_emphasize_wp;  
                                 /* choices:  
                                        UF_ASSEM_do_not_emphasize_wp  
                                        UF_ASSEM_emphasize_wp  
                                    */  
        assm_opts.emphasize_color = UF_OBJ_OLIVE;  
                                 /* choices:  
                                        see the colors in uf_obj.h  
                                    */  
        assm_opts.failure_action = UF_ASSEM_dont_abort_on_failure;  
                                 /* choices:  
                                        UF_ASSEM_do_abort_on_failure  
                                        UF_ASSEM_dont_abort_on_failure  
                                    */  
        assm_opts.maintain_work_part = UF_ASSEM_dont_maintain_work_part;  
                                 /* choices:  
                                        UF_ASSEM_dont_maintain_work_part  
                                        UF_ASSEM_do_maintain_work_part  
                                    */  
        assm_opts.load_latest = UF_ASSEM_load_latest_version;  
                                 /* choices:  
                                        UF_ASSEM_load_exact_version  
                                        UF_ASSEM_load_latest_version  
                                    */  
        assm_opts.load_components = UF_ASSEM_do_load_components;  
                                 /* choices:  
                                        UF_ASSEM_dont_load_components  
                                        UF_ASSEM_do_load_components  
                                        UF_ASSEM_use_last_component_set  
                                        UF_ASSEM_use_last_filter  
                                        UF_ASSEM_specify_filter  
                                    */  
        assm_opts.load_fully = UF_ASSEM_load_components_fully;  
                                 /* choices:  
                                        UF_ASSEM_load_components_partially  
                                        UF_ASSEM_load_components_fully  
                                    */  
        assm_opts.load_substitution = UF_ASSEM_do_allow_substitution;  
                                 /* choices:  
                                        UF_ASSEM_dont_allow_substitution  
                                        UF_ASSEM_do_allow_substitution  
                                    */  
        assm_opts.apply_to_all_levels = UF_ASSEM_do_apply_to_all_levels ;  
                                 /* choices:  
                                        UF_ASSEM_dont_apply_to_all_levels  
                                        UF_ASSEM_do_apply_to_all_levels  
                                    */  
        assm_opts.load_wave_data = UF_ASSEM_load_wave_data;  
                                 /* choices:  
                                        UF_ASSEM_load_wave_data  
                                    */  
        assm_opts.load_wave_parents = UF_ASSEM_all;  
                                 /* choices:  
                                        UF_ASSEM_none  
                                        UF_ASSEM_immediate  
                                        UF_ASSEM_all  
                                    */  
        UF_CALL( UF_ASSEM_set_assem_options( &assm_opts ));  
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
    int main( void )  
    {  
        if (!UF_CALL(UF_initialize()))  
        {  
            do_it();  
            UF_CALL(UF_terminate());  
        }  
        return 0;  
    }

```

#### 代码解析

> 这段代码是用于设置和获取NX装配加载选项的示例。主要功能包括：
>
> 1. 定义了错误报告函数report_error，用于在出错时打印错误信息。
> 2. 定义了do_it函数，用于获取当前的装配加载选项，设置新的选项，然后保存设置。
> 3. 在ufusr函数中，首先初始化NX，然后调用do_it函数，最后终止NX。
> 4. 定义了ufusr_ask_unload函数，用于在卸载NX时返回立即卸载的标志。
> 5. 主函数main中调用UF_initialize、do_it和UF_terminate实现初始化、设置选项和终止NX。
>
> 代码中使用了UF_ASSEM_ask_assem_options函数获取装配加载选项，UF_ASSEM_set_assem_options函数设置装配加载选项。同时，使用了UF_print_syslog在系统日志中打印错误信息。整体来说，这段代码提供了设置NX装配加载选项的示例。
>
