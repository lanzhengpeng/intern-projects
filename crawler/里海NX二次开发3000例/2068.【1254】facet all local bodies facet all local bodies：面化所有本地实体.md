### 【1254】facet all local bodies facet all local bodies：面化所有本地实体

#### 代码

```cpp
    /*HEAD FACET_ALL_LOCAL_BODIES CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_assem.h>  
    #include <uf_facet.h>  
    #include <uf_modl.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本中新增的函数。因此，该函数是V18版本特有的，用于打印系统日志信息。 */  
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
    static tag_t ask_next_local_solid(tag_t part, tag_t solid)  
    {  
        int  
            subtype,  
            status,  
            type;  
        while ((solid = UF_OBJ_cycle_all(part, solid)) != NULL_TAG)  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(solid, &type, &subtype));  
            status = UF_OBJ_ask_status(solid);  
            if ((type == UF_solid_type) &&  
                (subtype == UF_solid_body_subtype) &&  
                ((status == 3) || (UF_OBJ_is_object_a_promotion(solid))) &&  
                (!UF_ASSEM_is_occurrence(solid))) return solid;  
        }  
        return NULL_TAG;  
    }  
    static tag_t ask_occurrence_of_promotion(tag_t prom_object)  
    {  
        int  
            ii,  
            np;  
        tag_t  
            part_occ = NULL_TAG,  
           *path;  
        UF_CALL(UF_MODL_ask_promotion_path(prom_object, TRUE, &path, &np));  
        for (ii = np - 1; ii >= 0; ii--)  
            part_occ = UF_ASSEM_ask_part_occ_of_inst(part_occ, path[ii]);  
        UF_free(path);  
        return (UF_ASSEM_find_occurrence(part_occ, prom_object));  
    }  
    static void display_temporary_line(double start[3], double end[3])  
    {  
        UF_OBJ_disp_props_t  
            props = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            start, end, &props));  
    }  
    static int allocate_memory(unsigned int nbytes, void **where)  
    {  
        int  
            resp;  
        *where = UF_allocate_memory(nbytes, &resp);  
        return resp;  
    }  
    static void display_temporary_facets(tag_t model)  
    {  
        int  
            ii,  
            jj,  
            n_facets,  
            n_verts;  
        double  
            (*verts)[3];  
        UF_CALL(UF_FACET_ask_max_facet_verts(model, &n_verts));  
        UF_CALL(allocate_memory(n_verts * 3 * sizeof(double), (void *)&verts));  
        UF_CALL(UF_FACET_ask_n_facets_in_model(model, &n_facets));  
        for (ii = 0; ii < n_facets; ii++)  
        {  
            UF_CALL(UF_FACET_ask_vertices_of_facet(model, ii, &n_verts, verts));  
            for (jj = 0; jj < (n_verts - 1); jj++)  
                display_temporary_line(verts[jj], verts[jj+1]);  
            display_temporary_line(verts[n_verts-1], verts[0]);  
        }  
        UF_free(verts);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            body = NULL_TAG,  
            facet,  
            part = UF_PART_ask_display_part(),  
            solid;  
        UF_FACET_parameters_t  
            parms;  
        UF_CALL(UF_FACET_ask_default_parameters(&parms));  
        while ((body = ask_next_local_solid(part, body)) != NULL_TAG)  
        {  
            solid = body;  
            if (UF_OBJ_is_object_a_promotion(body))  
                solid = ask_occurrence_of_promotion(body);  
            if (solid != NULL_TAG)  
            {  
                UF_CALL(UF_FACET_facet_solid(solid, &parms, &facet));  
                display_temporary_facets(facet);  
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

> 这段代码是一个NX的二次开发示例，其主要功能是遍历当前部件中的所有实体，并对每个实体进行曲面划分，最后在屏幕上显示划分得到的曲面网格线。
>
> 主要步骤如下：
>
> 1. 包含NX Open所需的头文件。
> 2. 定义了一个错误报告函数report_error，用于打印出调用函数失败时的错误信息。
> 3. 定义了一个递归函数ask_next_local_solid，用于遍历当前部件中的所有实体，返回下一个实体的tag。
> 4. 定义了一个函数ask_occurrence_of_promotion，用于获取推广实体的实例。
> 5. 定义了一个函数display_temporary_line，用于在屏幕上显示一条临时线。
> 6. 定义了一个函数allocate_memory，用于申请内存。
> 7. 定义了一个函数display_temporary_facets，用于在屏幕上显示一个曲面的所有临时线。
> 8. 定义了一个主函数do_it，用于遍历所有实体，对每个实体进行曲面划分，并显示划分结果。
> 9. 定义了ufusr函数，这是NX二次开发的入口函数，在这里调用主函数do_it。
> 10. 定义了ufusr_ask_unload函数，用于卸载二次开发程序。
>
> 通过调用UF_FACET_facet_solid函数对每个实体进行曲面划分，并使用display_temporary_facets函数在屏幕上显示划分结果。
>
