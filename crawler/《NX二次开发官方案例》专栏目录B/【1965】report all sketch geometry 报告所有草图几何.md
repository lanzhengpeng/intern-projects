### 【1965】report all sketch geometry 报告所有草图几何

#### 代码

```cpp
    /*HEAD REPORT_ALL_SKETCH_GEOMETRY CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_modl.h>  
    #include <uf_sket.h>  
    #include <uf_sket_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据上下文，这句话的意思是：

UF_print_syslog 是在 V18 版本中新增的。

因此，翻译为：

UF_print_syslog 是在 V18 版本中新增的。 */  
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
    static void report_types_from_tag_array(char *name, tag_t *array, int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0,  
            type,  
            subtype;  
        printf("%s\n", name);  
        for (ii = 0; ii < rows; ii++)  
        {  
            printf("\t");  
            for (jj = 0; jj < cols; jj++)  
            {  
                UF_CALL(UF_OBJ_ask_type_and_subtype(array[kk], &type, &subtype));  
                printf("%d. \t%d\t (%d/%d)", kk+1, array[kk], type, subtype);  
                kk++;  
                if (jj < cols - 1) printf(", ");  
            }  
            printf("\n");  
        }  
    }  
    static void do_it(void)  
    {  
        int  
            num_geoms;  
        tag_t  
            part = UF_PART_ask_display_part(),  
            *geom_tags,  
            sketch = NULL_TAG;  
        UF_SKET_info_t  
            sket_info;  
        while ((sketch = ask_next_sketch(part, sketch)) != NULL_TAG)  
        {  
            UF_CALL(UF_SKET_ask_sketch_info(sketch, &sket_info));  
            ECHOS(sket_info.name);  
            UF_SKET_ask_geoms_of_sketch(sketch, &num_geoms, &geom_tags);  
            ECHO(num_geoms);  
            report_types_from_tag_array("Geom Item\tTag\t Type/Subtype", geom_tags,  
                num_geoms, 1);  
            UF_free(geom_tags);  
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

> 这段代码是一个NX Open的二次开发示例，主要功能是遍历当前显示部件中的所有草图，并报告每个草图中的几何图形信息。
>
> 主要步骤包括：
>
> 1. 初始化NX Open API。
> 2. 获取当前显示的部件。
> 3. 循环遍历部件中的所有草图，通过UF_OBJ_cycle_objs_in_part函数。
> 4. 对于每个草图，调用UF_SKET_ask_sketch_info获取草图信息，如名称。
> 5. 调用UF_SKET_ask_geoms_of_sketch获取草图中的所有几何图形信息，包括几何图形的tag、类型和子类型。
> 6. 打印输出每个草图中的几何图形信息。
> 7. 释放几何图形信息的内存。
> 8. 终止NX Open API。
>
> 此外，代码中还定义了错误报告函数report_error，用于在API调用出错时打印错误信息。整体上，该示例展示了如何遍历NX部件中的草图和几何图形，并获取相关信息，为NX二次开发提供了基础。
>
