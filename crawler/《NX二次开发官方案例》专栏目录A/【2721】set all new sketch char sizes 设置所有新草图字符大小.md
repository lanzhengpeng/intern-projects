### 【2721】set all new sketch char sizes 设置所有新草图字符大小

#### 代码

```cpp
    /*HEAD SET_ALL_NEW_SKETCH_CHAR_SIZES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_sket.h>  
    #include <uf_drf.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本新增的功能。

在V18版本中，新增了UF_print_syslog函数，用于打印系统日志。 */  
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
    static tag_t ask_next_new_sketch(tag_t part, tag_t sketch)  
    {  
        int  
            subtype,  
            type;  
        while  
            (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_sketch_type, &sketch)) &&  
            (sketch != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(sketch, &type, &subtype));  
            if (subtype == UF_v13_sketch_subtype) return (sketch);  
        }  
        return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        int  
            dec_places,  
            dim_label,  
            flags[10] = { 0, 0, 0, 0, 0, 0, 0, 1, 0, 0 },  
            ii,  
            mpi[100],  
            n_dims;  
        tag_t  
            *dims,  
            part = UF_PART_ask_display_part(),  
            sketch = NULL_TAG;  
        double  
            char_size,  
            mpr[70],  
            snap_angle;  
        char  
            arc_prefix[UF_OBJ_NAME_LEN],  
            conic_prefix[UF_OBJ_NAME_LEN],  
            dia[27],  
            line_prefix[UF_OBJ_NAME_LEN],  
            name_prefix[UF_OBJ_NAME_LEN],  
            rad[27],  
            spline_prefix[UF_OBJ_NAME_LEN],  
            vertex_prefix[UF_OBJ_NAME_LEN];  
        while ((sketch = ask_next_new_sketch(part, sketch)) != NULL_TAG)  
        {  
            UF_CALL(UF_SKET_ask_preferences(sketch, &snap_angle, name_prefix,  
                vertex_prefix, line_prefix, arc_prefix, conic_prefix,  
                spline_prefix, &char_size, &dec_places, &dim_label));  
            char_size = 0.5;  
            UF_CALL(UF_SKET_set_preferences(sketch, flags, snap_angle, name_prefix,  
                vertex_prefix, line_prefix, arc_prefix, conic_prefix,  
                spline_prefix, char_size, dec_places, dim_label));  
        /*  The following is a work around for PR 4164872 里海译:翻译如下：

以下是解决PR 4164872的变通方法。 */  
            UF_CALL(UF_SKET_ask_dimensions_of_sketch(sketch, &n_dims, &dims));  
            for (ii = 0; ii < n_dims; ii++)  
            {  
                UF_CALL(UF_DRF_ask_object_preferences(dims[ii],mpi,mpr,rad,dia));  
                mpr[32] = 0.5;  
                UF_CALL(UF_DRF_set_object_preferences(dims[ii],mpi,mpr,rad,dia));  
            }  
            UF_free(dims);  
        /*  End work around 里海译:End work around. */  
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

> 根据代码内容，这是一段用于修改NX草图尺寸的二次开发代码。主要功能包括：
>
> 1. 遍历当前部件中的所有草图，找出最新版本的草图。
> 2. 读取每个草图的标注尺寸设置，并将标注字符大小修改为0.5。
> 3. 读取草图中的每个标注尺寸对象，并将它们的字符大小也修改为0.5。
> 4. 使用UF_print_syslog函数打印错误信息到系统日志。
> 5. 定义了一个report_error函数，用于处理函数调用失败时打印错误信息。
> 6. 使用UF_SKET_set_preferences函数设置草图的标注尺寸设置。
> 7. 使用UF_DRF_set_object_preferences函数设置标注尺寸对象的标注尺寸设置。
> 8. 使用UF_free释放内存。
> 9. 定义了一个ask_next_new_sketch函数，用于遍历草图并返回最新版本的草图。
> 10. 定义了一个do_it函数，用于遍历所有草图并修改它们的标注尺寸设置。
> 11. 实现了ufusr和ufusr_ask_unload标准函数，用于启动和卸载NX Open应用程序。
>
> 总的来说，这段代码通过遍历草图并修改标注尺寸设置，实现了统一修改草图标注字符大小的功能。
>
