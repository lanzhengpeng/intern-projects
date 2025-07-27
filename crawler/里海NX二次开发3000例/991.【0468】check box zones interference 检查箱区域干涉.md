### 【0468】check box zones interference 检查箱区域干涉

#### 代码

```cpp
    /*HEAD CHECK_BOX_ZONES_INTERFERENCE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_fltr.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:翻译：

注意：UF_print_syslog是在V18中新增的。 */  
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
        tag_t   *zone_list;  
        tag_t   comp=NULL_TAG, part = UF_PART_ask_display_part();  
        int     ii, num_in_dir[3] = { 3, 3, 3}, num_zones;  
        double  user_vol[3], user_orig[3];  
        char    comp_name[UF_OBJ_NAME_LEN+1];  
        logical result;  
        UF_CALL( UF_FLTR_auto_create_box_zones( part, "GTAC", num_in_dir,  
                 TRUE, user_vol, user_orig, &zone_list, &num_zones));  
        while((comp = ask_next_component( part, comp)) != NULL_TAG)  
        {  
            for( ii = 0; ii < num_zones; ii++)  
            {  
                UF_CALL( UF_OBJ_ask_name( comp, comp_name));  
                UF_CALL(UF_FLTR_is_obj_intsct_zone(0.0,zone_list[ii],comp,&result));  
                if( result == TRUE)  
                    printf("Component %s intersects zone GTAC%d\n", comp_name, ii);  
            }  
        }  
        UF_free( zone_list);  
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

> 这段代码是一个NX Open C++ API的二次开发示例，主要功能是检查组件是否与自动创建的盒体过滤区域相交。
>
> 代码的主要步骤包括：
>
> 1. 包含必要的头文件，定义宏用于错误报告。
> 2. 定义一个静态函数report_error，用于报告函数调用错误。
> 3. 定义一个静态函数ask_next_component，用于循环查询当前部件中的下一个组件。
> 4. 定义一个静态函数do_it，用于执行主体逻辑：调用UF_FLTR_auto_create_box_zones函数，根据部件自动创建盒体过滤区域。循环查询部件中的组件，对每个组件调用UF_FLTR_is_obj_intsct_zone函数，判断是否与过滤区域相交，如果相交则打印组件名称和区域编号。
> 5. 调用UF_FLTR_auto_create_box_zones函数，根据部件自动创建盒体过滤区域。
> 6. 循环查询部件中的组件，对每个组件调用UF_FLTR_is_obj_intsct_zone函数，判断是否与过滤区域相交，如果相交则打印组件名称和区域编号。
> 7. 定义ufusr函数，用于初始化、调用do_it函数，然后终止。
> 8. 定义ufusr_ask_unload函数，用于卸载用户函数。
>
> 总的来说，这段代码通过自动创建盒体过滤区域，然后遍历检查部件中的每个组件是否与这些区域相交，实现了过滤区域与组件的相交检查功能。
>
