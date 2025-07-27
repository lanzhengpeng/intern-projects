### 【1744】plot all drawings in assembly 在装配中绘制所有图形

#### 代码

```cpp
    /*HEAD PLOT_ALL_DRAWINGS_IN_ASSEMBLY CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_modl.h>  
    #include <uf_disp.h>  
    #include <uf_assem.h>  
    #include <uf_part.h>  
    #include <uf_obj.h>  
    #include <uf_plot.h>  
    #include <uf_cfi.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文中的提示，UF_print_syslog是V18版本中新增的函数。翻译为：UF_print_syslog是在V18版本中新增加的。 */  
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
    static int ask_all_components(tag_t part, tag_t **components)  
    {  
        int  
            err,  
            ii,  
            n;  
        tag_t  
            component = NULL_TAG;  
        uf_list_p_t  
            comp_list;  
        UF_CALL(UF_MODL_create_list(&comp_list));  
        while ((component = ask_next_component(part, component)) != NULL_TAG)  
            UF_CALL(UF_MODL_put_list_item(comp_list, component));  
        UF_CALL(UF_MODL_ask_list_count(comp_list, &n));  
        *components = (tag_t *)UF_allocate_memory(n * sizeof(tag_t), &err);  
        if (UF_CALL(err)) return 0;  
        for (ii = 0; ii < n; ii++)  
            UF_CALL(UF_MODL_ask_list_item(comp_list, ii, *components+ii));  
        UF_CALL(UF_MODL_delete_list(&comp_list));  
        return n;  
    }  
    static int allocate_memory(unsigned int nbytes, void **where)  
    {  
        int  
            resp;  
        *where = UF_allocate_memory(nbytes, &resp);  
        return resp;  
    }  
    static int make_an_array(uf_list_p_t *object_list, tag_t **objects)  
    {  
        int  
            ii,  
            n;  
        uf_list_p_t  
            temp;  
        UF_CALL(UF_MODL_ask_list_count(*object_list, &n));  
        UF_CALL(allocate_memory(n * sizeof(tag_t), (void **)objects));  
        for (ii = 0, temp = *object_list; ii < n; temp = temp->next, ii++)  
            (*objects)[ii] = temp->eid;  
        UF_CALL(UF_MODL_delete_list(object_list));  
        return n;  
    }  
    static tag_t ask_next_drawing(tag_t part, tag_t drawing)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_drawing_type, &drawing));  
        return (drawing);  
    }  
    static void plot_all_drawings(void)  
    {  
        int  
            jobid;  
        tag_t  
            drawing = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        char  
            part_name[MAX_FSPEC_SIZE+1],  
            bannerMsg[132+1] = { "" },  
            dname[MAX_ENTITY_NAME_SIZE+1],  
            jobname[16+1] = { "test" },  
            node[132+1] = { "www-gtac" },  
            pauseMsg[44+1] = { "" },  
            plotter[16+1] = { "GTACHP5MX" };  
        UF_PLOT_options_t  
            opts =  
            {  
                1.0, 0.0,               /* scale and rotation 里海译:比例尺和旋转 */  
                { 0.0, 0.0, 0.0 },      /* origin 里海译:origin的翻译是“起源”或“源头”。 */  
                { 0.0, 0.0 },           /* offset 里海译:偏移量 */  
                "", "", "", "", "",     /* temporary pointers 里海译:临时指针 */  
                FALSE,                  /* print banner? 里海译:打印横幅 */  
                UF_PLOT_PLOT_RELEASE,   /* plot option 里海译:翻译plot option为：绘图选项 */  
                1,                      /* media number 里海译:媒体编号 */  
                1,                      /* copies 里海译:根据上下文，这里的"copies"翻译为“副本”。 */  
                1,                      /* pen type ; 1=width 2=color 里海译:笔类型；1=宽度 2=颜色 */  
                { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 }  
            };  
    /*  Assign the pointers to point to the allocated space 里海译:分配指针，使其指向分配的内存空间。 */  
        opts.plotter = plotter;  
        opts.node = node;  
        opts.jobname = jobname;  
        opts.pauseMsg = pauseMsg;  
        opts.bannerMsg = bannerMsg;  
        UF_PART_ask_part_name(part, part_name);  
        printf("Part:\n  %s\n", part_name);  
        while ((drawing = ask_next_drawing(part, drawing)) != NULL_TAG)  
        {  
            UF_CALL(UF_OBJ_ask_name(drawing, dname));  
            UF_CALL(UF_PLOT_drawing(dname, &opts, &jobid));  
            printf("%s - JOBID = %d\n", dname, jobid);  
        }  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            num_comps_on_list,  
            num_parts;  
        tag_t  
            comp_part = NULL_TAG,  
            *comps,  
            *comp_array,  
            part = UF_PART_ask_display_part();  
        uf_list_p_t  
            comp_list;  
        plot_all_drawings();  
        UF_CALL(UF_MODL_create_list(&comp_list));  
        num_parts = ask_all_components(part, &comps);     
        ECHO(num_parts);  
        for (ii = 0; ii < num_parts; ii++)  
        {  
        comp_part = UF_ASSEM_ask_prototype_of_occ(comps[ii]);   
        UF_CALL(UF_MODL_put_list_item(comp_list, comp_part));  
        }  
        if (num_parts > 0) UF_free(comps);  
        num_comps_on_list = make_an_array(&comp_list, &comp_array);  
        ECHO(num_comps_on_list);  
        if (num_comps_on_list > 0)  
        {  
            for (ii = 0; ii < num_comps_on_list; ii++)  
            {  
                UF_PART_set_display_part(comp_array[ii]);  
                plot_all_drawings();  
            }  
        }  
        uc1601("All done", TRUE);  
        UF_PART_set_display_part(part);  
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

> 这段代码是一个用于NX二次开发的程序，其主要功能是遍历装配体中的所有组件，并绘制每个组件中的所有图纸。以下是代码的主要步骤：
>
> 1. 定义了一个错误报告函数report_error，用于打印错误信息。
> 2. 定义了一个函数ask_next_component，用于遍历装配体中的所有组件，并返回下一个组件的tag。
> 3. 定义了一个函数ask_all_components，用于获取装配体中的所有组件，并返回组件数组。
> 4. 定义了一个函数allocate_memory，用于申请内存。
> 5. 定义了一个函数make_an_array，用于将列表转换为数组。
> 6. 定义了一个函数ask_next_drawing，用于遍历组件中的所有图纸，并返回下一个图纸的tag。
> 7. 定义了一个函数plot_all_drawings，用于绘制组件中的所有图纸。
> 8. 定义了一个函数do_it，用于遍历装配体中的所有组件，并绘制每个组件中的所有图纸。
> 9. 定义了ufusr函数，作为程序的入口点。首先初始化NX环境，然后调用do_it函数，最后终止NX环境。
> 10. 定义了一个函数ufusr_ask_unload，返回立即卸载标志。
>
> 总的来说，这段代码实现了遍历装配体，获取每个组件，然后遍历组件中的所有图纸并绘制出来，从而实现了在装配体中绘制所有图纸的功能。
>
