### 【1808】pull tolerance feature 拉公差特征

#### 代码

```cpp
    /*HEAD PULL_TOLERANCE_FEATURE CCC UFUN */  
    #include <uf.h>  
    #include <uf_assem.h>  
    #include <uf_obj.h>  
    #include <uf_gdt.h>  
    #include <uf_part.h>  
    #include <uf_object_types.h>  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf_ui.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本新增的函数，主要功能是打印系统日志。 */  
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
    static void do_it( void)  
    {  
        tag_t       tol_id = NULL_TAG, pull_id = NULL_TAG;  
        tag_t       part_tag, disp_occ, disp_part, work_occ, work_inst;  
        tag_t*      ch_tags, *n_tags;  
        int         n_prts, typ, sub;  
        UF_initialize();  
        disp_part = UF_PART_ask_display_part();  
        if ( disp_part == NULL_TAG )   
        {   
            printf("disp part NULL\n"); return;   
        }  
        disp_occ = UF_ASSEM_ask_root_part_occ(disp_part);  
        if ( (n_prts = UF_ASSEM_ask_part_occ_children(disp_occ,&ch_tags)) > 0 )  
        {  
            part_tag = ch_tags[0];  
            n_prts =  UF_ASSEM_ask_part_occ_children(ch_tags[0],&n_tags);  
            if ( n_prts > 0 )  
               part_tag = n_tags[0];  
            work_occ = UF_ASSEM_ask_prototype_of_occ(part_tag);  
            work_inst = UF_ASSEM_ask_inst_of_part_occ(part_tag);  
            while (!UF_CALL(UF_OBJ_cycle_objs_in_part(work_occ, UF_tolerance_feature_type,  
                   &tol_id)) && ( tol_id != NULL_TAG ))  
            {  
                UF_CALL(UF_OBJ_ask_type_and_subtype(tol_id,&typ,&sub));  
                if ( typ == UF_tolerance_feature_type &&   
                     sub == UF_tolerance_feature_subtype)  
                    UF_CALL(UF_GDT_pull_tolerance( tol_id, work_inst, &pull_id, TRUE));  
            }  
            if ( n_prts > 0 )  
                 UF_free(n_tags);  
            UF_free(ch_tags);  
        }  
        UF_terminate ();  
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

> 根据代码内容，这是一段用于在NX中拉取公差特征的二次开发代码，其主要功能包括：
>
> 1. 定义了错误报告函数report_error，用于在发生UF函数调用错误时打印错误信息。
> 2. 定义了主函数do_it，首先获取当前显示的部件，然后遍历其所有子装配中的所有公差特征，并调用UF_GDT_pull_tolerance函数进行拉取。
> 3. 定义了ufusr函数，初始化后调用do_it函数，最后进行终止。
> 4. 定义了ufusr_ask_unload函数，用于在卸载用户函数时返回立即卸载的标志。
> 5. 使用宏UF_CALL包装UF函数调用，以调用report_error函数进行错误报告。
> 6. 在do_it函数中，首先获取当前显示的部件和顶层装配，然后获取第一个子装配，继续获取其子装配，直至找到零件级别的装配。然后遍历零件中的所有公差特征，并调用UF_GDT_pull_tolerance函数进行拉取。
> 7. 在遍历公差特征时，使用了UF_OBJ_cycle_objs_in_part函数，并通过UF_OBJ_ask_type_and_subtype函数检查特征类型。
> 8. 使用UF_GDT_pull_tolerance函数拉取公差特征。
> 9. 使用UF_free释放内存。
> 10. 使用UF_print_syslog函数打印系统日志信息。
>
> 总的来说，这段代码的主要目的是遍历当前显示部件的子装配中的所有零件，并拉取其中的公差特征。
>
