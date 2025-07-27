### 【0688】create expression in master model of current drawing 在当前绘图中创建主模型的表达式

#### 代码

```cpp
    /*HEAD CREATE_EXPRESSION_IN_MASTER_MODEL_OF_CURRENT_DRAWING CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_modl.h>  
    #include <uf_assem.h>  
    #include <uf_obj.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:翻译：

注意：UF_print_syslog是在V18中新增加的，只回答翻译，不要添加无关内容。 */  
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
    static tag_t ask_next_component(tag_t part, tag_t comp)  
    {  
        int  
            subtype,  
            type;  
        while  (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_component_type, &comp))  
            && (comp != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(comp, &type, &subtype));  
            if (subtype == UF_part_occurrence_subtype) return comp;  
        }  
        return comp;  
    }  
    static void report_load_status(UF_PART_load_status_p_t status)  
    {  
        char  
            msg[133];  
        int  
            ii;  
        if (status->n_parts > 0)  
        {  
            UF_UI_open_listing_window();  
            UF_UI_write_listing_window("Load notes:\n");  
            for (ii = 0; ii < status->n_parts; ii++)  
            {  
                UF_UI_write_listing_window("  ");  
                UF_UI_write_listing_window(status->file_names[ii]);  
                UF_UI_write_listing_window(" - ");  
                UF_get_fail_message(status->statuses[ii], msg);  
                UF_UI_write_listing_window(msg);  
                UF_UI_write_listing_window("\n");  
            }  
            UF_UI_write_listing_window("\n");  
            UF_free(status->statuses);  
            UF_free_string_array(status->n_parts, status->file_names);  
        }  
    }  
    static void ensure_part_fully_loaded(tag_t part)  
    {  
        char  
            partname[MAX_FSPEC_SIZE+1];  
        UF_PART_load_status_t  
            error_status;  
        if (!UF_CALL(UF_PART_ask_part_name(part, partname))  
            && ((UF_PART_is_loaded(partname)) != 1))  
        {  
            UF_CALL(UF_PART_open_quiet(partname, &part, &error_status));  
            if (error_status.n_parts > 0) report_load_status(&error_status);  
        }  
    }  
    static tag_t set_work_part_quietly_by_occ(tag_t occ)  
    {  
        tag_t  
            old_work_part = NULL_TAG,  
            owning_part,  
            proto = UF_ASSEM_ask_prototype_of_occ(occ);  
        if (!UF_ASSEM_is_occurrence(occ) ||  
            UF_OBJ_is_object_a_promotion(occ)) return NULL_TAG;  
        UF_CALL(UF_OBJ_ask_owning_part(proto, &owning_part));  
        ensure_part_fully_loaded(owning_part);  
        UF_CALL(UF_ASSEM_set_work_part_quietly(owning_part, &old_work_part));  
        return old_work_part;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            comp = NULL_TAG,  
            exp,  
            part = UF_PART_ask_display_part();  
        comp = ask_next_component(part, comp);  
        part = set_work_part_quietly_by_occ(comp);  
        UF_CALL(UF_MODL_create_exp_tag("GTAC_TEST=1", &exp));  
        UF_CALL(UF_ASSEM_set_work_part_quietly(part, &comp));  
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

> 这段代码是NX的二次开发代码，主要功能是在当前绘图中创建表达式。代码主要包括以下部分：
>
> 1. 包含头文件：引入了必要的NX API头文件。
> 2. 错误处理宏：定义了UF_CALL宏，用于调用NX API函数并处理错误。
> 3. 错误报告函数：实现了report_error函数，用于打印错误信息。
> 4. 获取组件函数：实现了ask_next_component函数，用于遍历组件并获取下一个组件。
> 5. 加载状态报告函数：实现了report_load_status函数，用于报告组件加载状态。
> 6. 确保组件完全加载函数：实现了ensure_part_fully_loaded函数，用于确保组件被完全加载。
> 7. 设置工作组件函数：实现了set_work_part_quietly_by_occ函数，用于根据组件实例设置工作组件。
> 8. 主要逻辑实现：在do_it函数中，首先获取当前显示的组件，然后设置工作组件，创建表达式，最后恢复工作组件。
> 9. 入口函数：实现了ufusr函数，作为程序的入口，初始化NX并调用do_it函数，最后终止NX。
> 10. 卸载询问函数：实现了ufusr_ask_unload函数，用于询问是否立即卸载。
>
> 总体来说，这段代码通过遍历组件，创建表达式，实现了在当前绘图中创建表达式功能，具有较好的模块化设计。
>
