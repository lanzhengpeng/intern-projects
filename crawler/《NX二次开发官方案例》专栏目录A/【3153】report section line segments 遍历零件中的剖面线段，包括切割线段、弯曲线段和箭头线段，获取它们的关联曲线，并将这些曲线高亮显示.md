### 【3153】report section line segments 遍历零件中的剖面线段，包括切割线段、弯曲线段和箭头线段，获取它们的关联曲线，并将这些曲线高亮显示

#### 代码

```cpp
    /*HEAD REPORT_SECTION_LINE_SEGMENTS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_draw.h>  
    #include <uf_draw_types.h>  
    #include <uf_drf.h>  
    #include <uf_obj.h>  
    #include <uf_disp.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog 是 V18 新增的功能。 */  
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
    static tag_t cycle_by_type_and_subtype(tag_t part, int type,  
        int subtype, tag_t object)  
    {  
        int  
            this_type,  
            this_subtype;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, type,  
            &object)) && (object != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(object, &this_type, &this_subtype));  
            if (this_subtype == subtype) return object;  
        }  
        return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            part = UF_PART_ask_display_part(),  
            curve_tag = NULL_TAG,  
            section_cut_segment = NULL_TAG,  
            section_bend_segment = NULL_TAG,  
            section_arrow_segment = NULL_TAG;  
        UF_DRAW_sxseg_info_t  
            sxseg_info;  
        UF_DRF_object_p_t  
            object_tag;  
        while ((section_cut_segment = cycle_by_type_and_subtype(part,  
            UF_section_segment_type, UF_cut_segment_subtype, section_cut_segment))  
            != NULL_TAG)  
        {  
            UF_CALL(UF_DRAW_ask_sxline_sxseg(section_cut_segment, &sxseg_info,  
                &curve_tag, &object_tag ));  
            ECHO(section_cut_segment);  
            ECHO(curve_tag);  
            printf("--------------------------\n");  
            UF_CALL(UF_DISP_set_highlight(curve_tag, TRUE));  
            UF_free(object_tag);  
        }  
        while ((section_bend_segment = cycle_by_type_and_subtype( part,  
            UF_section_segment_type, UF_bend_segment_subtype, section_bend_segment))  
            != NULL_TAG)  
        {  
            UF_CALL(UF_DRAW_ask_sxline_sxseg(section_bend_segment, &sxseg_info,  
                &curve_tag, &object_tag ));  
            ECHO(section_bend_segment);  
            ECHO(curve_tag);  
            printf("--------------------------\n");  
            UF_CALL(UF_DISP_set_highlight(curve_tag, TRUE));  
            UF_free(object_tag);  
        }  
        while ((section_arrow_segment = cycle_by_type_and_subtype( part,  
            UF_section_segment_type, UF_arrow_segment_subtype,  
            section_arrow_segment)) != NULL_TAG)  
        {  
            UF_CALL(UF_DRAW_ask_sxline_sxseg(section_arrow_segment, &sxseg_info,  
                &curve_tag, &object_tag ));  
            ECHO(section_arrow_segment);  
            ECHO(curve_tag);  
            printf("--------------------------\n");  
            UF_CALL(UF_DISP_set_highlight(curve_tag, TRUE));  
            UF_free(object_tag);  
        }  
        uc1601("section segment curves highlighted",TRUE);  
        UF_CALL(UF_PART_cleanup(UF_PART_cleanup_highlight));  
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

> 这段代码是用于NX的二次开发，主要功能是高亮显示当前部件中的所有剖面线段曲线。代码的主要步骤包括：
>
> 1. 包含了必要的NX开发库头文件。
> 2. 定义了错误报告函数report_error，用于在出错时打印错误信息。
> 3. 定义了cycle_by_type_and_subtype函数，用于遍历当前部件中指定类型和子类型的对象。
> 4. 定义了do_it函数，用于高亮显示当前部件中的所有剖面线段曲线。具体步骤包括：
>
> 1. ufusr函数是NX二次开发的入口函数，负责初始化、调用do_it函数、以及终止NX。
> 2. ufusr_ask_unload函数用于在卸载NX时立即卸载二次开发。
>
> 总的来说，这段代码通过遍历当前部件中的剖面线段，获取每个线段关联的曲线，并高亮显示，实现了剖面线段曲线的高亮显示功能。
>
