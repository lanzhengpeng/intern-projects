### 【2309】report sectioned components in all section views 列出所有剖视图中的组件

#### 代码

```cpp
    #define _CRT_SECURE_NO_WARNINGS  
    /*HEAD REPORT_SECTIONED_COMPONENTS_IN_ALL_SECTION_VIEWS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_assem.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_draw.h>  
    #include <uf_obj.h>  
    #include <uf_so.h>  
    #include <uf_disp.h>  
    #include <uf_view.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据提供的信息，UF_print_syslog是V18版本中新增的函数，用于打印系统日志信息。 */  
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
    static void print_component_name(tag_t comp)  
    {  
        double  
            csys_matrix[9],  
            origin[3],  
            transform[4][4];  
        char  
            instance_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            part_name[MAX_FSPEC_SIZE+1],  
            refset_name[MAX_ENTITY_NAME_SIZE+1];  
        UF_CALL(UF_ASSEM_ask_component_data(comp, part_name, refset_name,  
            instance_name, origin, csys_matrix, transform));  
        UF_UI_write_listing_window(instance_name);  
        UF_UI_write_listing_window("\n");  
    }  
    static void do_it(void)  
    {  
    logical  
        is_a_sxview;  
    int  
        ii,  
        jj,  
        n_solids,  
        n_views;  
    tag_t  
        d_tag,  
        sxline,  
        *sxsolids,  
        *v_tags,  
        solid,  
        component;  
    char  
        view[UF_OBJ_NAME_LEN + 1] = { "" },  
        drawing[UF_OBJ_NAME_LEN + 1],  
        msg[128];  
        UF_UI_open_listing_window();  
        UF_CALL(UF_DRAW_ask_current_drawing(&d_tag));  
        if( d_tag == NULL_TAG )  
        {  
            UF_UI_write_listing_window("\nWhere is your open drawing...?!\n");  
            return;  
        }  
        UF_CALL(UF_OBJ_ask_name(d_tag, drawing));  
        sprintf(msg, "Drawing %d = %s\n", d_tag, drawing);  
        UF_UI_write_listing_window(msg);  
        UF_CALL( UF_DRAW_ask_views(d_tag, &n_views, &v_tags) );  
        for (ii = 0; ii < n_views; ii++)  
        {  
            UF_CALL(UF_OBJ_ask_name(v_tags[ii], view));  
            UF_CALL(UF_DRAW_is_sxview(v_tags[ii], &is_a_sxview));  
            if (is_a_sxview)  
            {  
                sprintf(msg, "Sectioned components in view %d = %s:\n", v_tags[ii], view);  
                UF_UI_write_listing_window(msg);  
                UF_CALL(UF_DRAW_ask_sxline_of_sxview(v_tags[ii], &sxline));  
                UF_CALL(UF_DRAW_ask_sxsolids_of_sxview(v_tags[ii], UF_DRAW_sxline_leg1, &n_solids, &sxsolids));  
                for (jj = 0; jj < n_solids; jj++)  
                {  
                    UF_CALL(UF_DRAW_ask_solid_of_section(sxsolids[jj], &solid));  
                    UF_CALL( UF_ASSEM_ask_parent_component (solid, &component ) );  
                    if( component )    print_component_name(component);  
                }  
                if( n_solids ) UF_free(sxsolids);  
            }  
        }  
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

> 这段代码是一个NX的二次开发示例，主要功能是遍历当前图纸中的所有剖视图，并打印出每个剖视图中包含的组件名称。
>
> 主要流程如下：
>
> 1. 首先打开一个Listing窗口，用于输出信息。
> 2. 获取当前打开的图纸对象，并打印图纸名称。
> 3. 遍历图纸中的所有视图对象。
> 4. 判断每个视图是否为剖视图。
> 5. 如果是剖视图，则获取剖视图中的剖切线对象。
> 6. 获取剖切线上的所有剖面体对象。
> 7. 对于每个剖面体，获取其对应的组件对象。
> 8. 打印组件名称。
> 9. 释放剖面体对象数组。
> 10. 最后，结束NX的初始化。
>
> 该代码通过UF_DRAW模块提供的接口，实现了对NX图纸的遍历，并使用UF_ASSEM模块获取组件信息。同时，通过UF_UI模块实现了信息的打印输出。整体结构清晰，注释详细，是一个很好的NX二次开发示例。
>
