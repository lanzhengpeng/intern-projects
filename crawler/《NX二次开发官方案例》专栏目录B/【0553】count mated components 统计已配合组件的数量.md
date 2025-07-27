### 【0553】count mated components 统计已配合组件的数量

#### 代码

```cpp
    /*HEAD COUNT_MATED_COMPONENTS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_assem.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档中的描述，UF_print_syslog是V18版本新增的函数。因此，翻译为：UF_print_syslog是V18版本新增的函数。 */  
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
    static void do_it(void)  
    {  
        logical  
            mated;  
        int  
            cnt = 0,  
            ii;  
        tag_t  
            comp = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        UF_ASSEM_mating_condition_t  
            mc_data;  
        char  
            mc_name[MAX_ENTITY_NAME_SIZE+1],  
            constraint_names[UF_ASSEM_MAX_CONSTRAINTS][MAX_ENTITY_NAME_SIZE+1];  
        /* Set up the space for the names in the structures 译:设置结构中名字的空间 */  
        mc_data.name = mc_name;  
        for (ii = 0; ii < UF_ASSEM_MAX_CONSTRAINTS; ii++)  
            mc_data.constraints[ii].name = &constraint_names[ii][0];  
        while ((comp = ask_next_component(part, comp)) != NULL_TAG)  
        {  
            UF_CALL(UF_ASSEM_ask_mc_data_of_compnt(comp, &mated, &mc_data));  
            if (mated) cnt++;  
        }  
        ECHO(cnt);  
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

> 这段代码是NX的二次开发代码，用于统计装配体中所有组件的配合数量。以下是代码的主要功能：
>
> 1. 错误报告函数：使用宏定义了错误报告函数report_error，用于输出错误信息，包括文件名、行号、函数名和错误码。
> 2. 组件遍历函数：使用ask_next_component函数遍历装配体中的所有组件，并返回下一个组件的tag。
> 3. 组件配合信息查询：使用do_it函数遍历组件，并使用UF_ASSEM_ask_mc_data_of_compnt函数查询组件的配合信息，包括是否配合以及配合名称和约束名称。若组件配合，则计数器加1。
> 4. 统计结果输出：使用宏ECHO输出最终配合组件的数量。
> 5. 初始化和终止：在ufusr函数中进行初始化和终止，调用do_it函数，并处理返回码。
> 6. 卸载函数：定义了卸载函数ufusr_ask_unload，返回立即卸载标志。
>
> 这段代码通过遍历组件并查询配合信息，实现了统计装配体中所有组件的配合数量的功能。代码结构清晰，使用错误报告和组件遍历等功能，展示了NX二次开发的典型用法。
>
