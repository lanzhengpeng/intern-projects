### 【1956】report all sketch dims status 报告所有草图的尺寸状态

#### 代码

```cpp
    /*HEAD REPORT_ALL_SKETCH_DIMS_STATUS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_modl.h>  
    #include <uf_sket.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog 是 V18 的新功能，专门用于打印系统日志。 */  
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
    static tag_t ask_next_sketch(tag_t part, tag_t sketch)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_sketch_type, &sketch));  
        return (sketch);  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_dims,  
            stat;  
        tag_t  
            dim,  
            exp,  
            part = UF_PART_ask_display_part(),  
            sketch = NULL_TAG;  
        double  
            value;  
        char  
            exp_str[256],  
            view_name[16];  
        uf_list_p_t  
            dims;  
        while ((sketch = ask_next_sketch(part, sketch)) != NULL_TAG)  
        {  
            UF_CALL(UF_SKET_ask_info(sketch, 5, view_name, &dims));  
            printf("Sketch %s:\n", view_name);  
            UF_CALL(UF_SKET_ask_info(sketch, 4, view_name, &dims));  
            UF_CALL(UF_MODL_ask_list_count(dims, &n_dims));  
            printf("  %d dims:\n", n_dims);  
            for (ii=0; ii<n_dims; ii++)  
            {  
                UF_CALL(UF_MODL_ask_list_item(dims, ii, &dim));  
                UF_CALL(UF_SKET_ask_dim_status(dim, &exp, exp_str, &value, &stat));  
                printf("    %d. status %d = %s\n", ii+1, stat, exp_str);  
            }  
            UF_CALL(UF_MODL_delete_list(&dims));  
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

> 这段代码是NX Open C++ API编写的，主要功能是遍历当前活动零件中的所有草图，并报告每个草图中的所有尺寸的状态。
>
> 主要步骤如下：
>
> 1. 包含必要的NX Open C++头文件。
> 2. 定义一个错误报告函数report_error，用于在API调用失败时打印错误信息。
> 3. 定义一个函数ask_next_sketch，用于遍历零件中的下一个草图。
> 4. 定义一个函数do_it，用于遍历当前零件中的所有草图，并报告每个草图中的所有尺寸的状态。
> 5. 在do_it函数中，通过循环调用ask_next_sketch获取每个草图，然后调用UF_SKET_ask_info获取草图信息，UF_MODL_ask_list_count获取尺寸数量，循环调用UF_MODL_ask_list_item获取每个尺寸，最后调用UF_SKET_ask_dim_status获取尺寸的状态，并打印出来。
> 6. 定义ufusr函数，用于在NX中加载和调用do_it函数。
> 7. 定义ufusr_ask_unload函数，用于卸载NX Open C++库。
>
> 总的来说，这段代码通过遍历零件中的草图和尺寸，实现了查询并打印每个尺寸的状态的功能。
>
